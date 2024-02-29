import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.time.Duration;
import java.time.Instant;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveAction;
import java.util.concurrent.atomic.AtomicIntegerFieldUpdater;
import java.util.concurrent.atomic.AtomicReferenceFieldUpdater;
import java.util.function.Consumer;

public class Main {
    private static final String INPUT_FILE_PATH = "input.txt";
    private static final String OUTPUT_FILE_PATH = "output.txt";
    private static final int NUM_THREADS_TO_USE = Runtime.getRuntime().availableProcessors();
    private static final long MAX_CHUNK_SIZE = 1024 * 1024 * 10; // 10MB

    private static final Map<Slot, ConcurrentHashMap<Slot, Boolean>> CITY_PRODUCTS = new ConcurrentHashMap<>();

    public static void main(String[] args) {
        Instant start = Instant.now();

        List<ByteBuffer> chunks;

        try (RandomAccessFile raf = new RandomAccessFile(INPUT_FILE_PATH, "r")) {
            long fileSize = raf.length();

            long chunkSize = fileSize < MAX_CHUNK_SIZE ? fileSize : MAX_CHUNK_SIZE;

            chunks = splitIntoChunks(raf, chunkSize);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }


        var cityMap = new TreeMap<Slot, Double>();
        try (var fjp = new ForkJoinPool(NUM_THREADS_TO_USE)) {
            Dictionary cityDictionary = new Dictionary();
            Dictionary productDictionary = new Dictionary();
            fjp.submit(new ChunkProcessor(cityDictionary, productDictionary, chunks)).join();
            cityDictionary.forEach(slot -> cityMap.put(slot, slot.getCityResult()));
        }

        double minSum = Collections.min(cityMap.values());
        Slot minCity = cityMap
                .entrySet()
                .stream()
                .filter(entry -> minSum == entry.getValue())
                .map(Map.Entry::getKey)
                .findFirst()
                .get();

        ConcurrentHashMap<Slot, Boolean> cityProducts = CITY_PRODUCTS.get(minCity);

        List<Map.Entry<Slot, Boolean>> entryList = new ArrayList<>(cityProducts.entrySet());

        Collections.sort(entryList, (o1, o2) -> {
            int valueComparison = Double.compare(o1.getKey().getProductResult(), o2.getKey().getProductResult());
            if (valueComparison != 0) {
                return valueComparison;
            } else {
                return o1.getKey().toKey().compareTo(o2.getKey().toKey());
            }
        });

        Map<Slot, Boolean> sortedMap = new LinkedHashMap<>();
        for (Map.Entry<Slot, Boolean> entry : entryList) {
            sortedMap.put(entry.getKey(), entry.getValue());
        }

        try (BufferedWriter writer = new BufferedWriter(new FileWriter(OUTPUT_FILE_PATH, false))) {
            String formattedCity = String.format(Locale.US, "%s %.2f", minCity.toKey(), minSum);
            writer.write(formattedCity);

            sortedMap.keySet()
                    .stream()
                    .limit(5)
                    .forEach(e -> {
                        String formattedProduct = String.format(Locale.US, "%s %.2f", e.toKey(), e.getProductResult());

                        try {
                            writer.newLine();
                            writer.write(formattedProduct);
                        } catch (IOException ex) {
                            throw new RuntimeException(ex);
                        }
                    });

        } catch (IOException ex) {
            throw new RuntimeException(ex);
        }

        Instant finish = Instant.now();
        long timeElapsed = Duration.between(start, finish).toMillis();

        System.out.println("Time elapsed: " + timeElapsed + " ms");
    }

    private static List<ByteBuffer> splitIntoChunks(RandomAccessFile raf, long chunkSize) throws IOException {
        long fileSize = raf.length();
        long cursor = 0L;

        List<ByteBuffer> chunks = new ArrayList<>();
        while (cursor < fileSize) {
            long currentIndex = cursor + chunkSize;
            if (currentIndex < fileSize) {
                // go to current index in file
                raf.seek(currentIndex);

                // search for next new line character to ensure a valid chunk
                while (raf.read() != '\n') {
                    currentIndex += 1;
                }

                // adjust for actual new line character index for next chunk
                currentIndex += 1;
            }
            // handle last chunk in file
            else {
                currentIndex = fileSize;
            }

            ByteBuffer buffer = raf.getChannel()
                    .map(FileChannel.MapMode.READ_ONLY, cursor, currentIndex - cursor)
                    .order(ByteOrder.nativeOrder());

            chunks.add(buffer);
            cursor = currentIndex;
        }

        return chunks;
    }

    private static long separatorBytes(long piece) {
        // flip bits where there is a comma character
        long flipped = piece ^ 0x2C2C2C2C_2C2C2C2CL; // 2C == ','

        long clearedHighestBits = flipped - 0x01010101_01010101L;

        long bitwiseComplement = ~flipped;

        long test = bitwiseComplement & 0x80808080_80808080L;

        flipped = clearedHighestBits & test;

        return flipped;
    }

    private static int trailingZerosCount(long bitsToTail) {
        return Long.numberOfTrailingZeros(bitsToTail >>> 7);
    }

    private static int valueOffset(int value) {
        return (value >>> 3) + 1;
    }

    private static double decimalValue(int decimalPointIndex, int newLineIndex, long value) {
        int leftShift = (3 - decimalPointIndex) * Byte.SIZE;

        int shift = (Byte.SIZE - newLineIndex) * Byte.SIZE;

        long adjustedNumber = (((value << shift) >> shift) << leftShift) & 0x0F0F000F0F0FL;

        final byte integerPart100 = (byte) (adjustedNumber);
        final byte integerPart10 = (byte) (adjustedNumber >> 8);
        final byte integerPart1 = (byte) (adjustedNumber >> 16);

        final byte fractionPart10 = (byte) (adjustedNumber >> 32);
        final byte fractionPart1 = (byte) (adjustedNumber >> 40);


        return ((integerPart100 * 100 + integerPart10 * 10 + integerPart1) + (fractionPart10 * 10 + fractionPart1) * 0.01);
    }

    private static class ChunkProcessor extends RecursiveAction {
        private final int startIndex;
        private final int endIndex;

        private final Dictionary cityDictionary;
        private final Dictionary productDictionary;

        private final List<ByteBuffer> chunks;

        public ChunkProcessor(Dictionary cityDictionary, Dictionary productDictionary, List<ByteBuffer> chunks) {
            this(cityDictionary, productDictionary, chunks, 0, chunks.size() - 1);
        }

        private ChunkProcessor(Dictionary cityDictionary, Dictionary productDictionary, List<ByteBuffer> chunks, int startIndex, int endIndex) {
            this.cityDictionary = cityDictionary;
            this.productDictionary = productDictionary;
            this.chunks = chunks;
            this.startIndex = startIndex;
            this.endIndex = endIndex;
        }

        @Override
        protected void compute() {
            // parallel process
            if (startIndex == endIndex) {
                processChunk(startIndex);
            } else {
                int middle = (startIndex + endIndex) / 2;
                var low = new ChunkProcessor(cityDictionary, productDictionary, chunks, startIndex, middle);
                var high = new ChunkProcessor(cityDictionary, productDictionary, chunks, middle + 1, endIndex);
                var fork = high.fork();
                low.compute();
                fork.join();
            }
        }

        private void processChunk(int chunkIndex) {
            ByteBuffer chunk = chunks.get(chunkIndex);

            int chunkSize = chunk.limit();

            // Rewind cursor until before last line
            // the last line will be processed separately
            int cursor = Math.max(chunkSize - 16, -1);
            while (cursor >= 0 && chunk.get(cursor) != '\n') {
                cursor--;
            }
            cursor++;

            // if chunk has more than one line
            if (cursor > 0) {
                processChunk(chunkIndex, chunk, cursor);
            }

            int lastLinesLength = chunkSize - cursor;
            ByteBuffer lastLinesChunk = ByteBuffer.allocate(lastLinesLength + Long.BYTES).order(ByteOrder.LITTLE_ENDIAN);
            chunk.get(cursor, lastLinesChunk.array(), 0, lastLinesLength);
            processChunk(chunkIndex, lastLinesChunk, lastLinesLength);
        }

        private void processChunk(int chunkIndex, ByteBuffer chunk, int readLength) {
            int cursor = 0;

            int foundDelimiters = 0;
            int[] delimiterIndexes = new int[2];

            int lastNewLineIndex = 0;

            while (cursor < readLength && readLength - cursor > Long.BYTES) {
                while (foundDelimiters < 2) {
                    // read in 8 byte pieces
                    long piece = chunk.getLong(cursor);

                    long separatorBytes = separatorBytes(piece);

                    // if separatorBytes still contains bits
                    // it means that the current piece has a comma character
                    if (separatorBytes != 0) {
                        int trailingZeros = trailingZerosCount(separatorBytes);
                        int delimiterIndex = valueOffset(trailingZeros) - 1;
                        delimiterIndexes[foundDelimiters] = cursor + delimiterIndex;
                        foundDelimiters++;
                        cursor += delimiterIndex + 1;
                        continue;
                    }

                    cursor += Long.BYTES;
                }

                long pricePiece = chunk.getLong(delimiterIndexes[1] + 1);

                int decimalPointIndex = Long.numberOfTrailingZeros(~pricePiece & 0x10101000L) >> 3;

                long nlTmp = chunk.getLong(delimiterIndexes[1] + 1 + decimalPointIndex + 1);

                int newLineIndex = (Long.numberOfTrailingZeros(~nlTmp & 0x101000L) >> 3) + decimalPointIndex + 1;

                double price = decimalValue(decimalPointIndex, newLineIndex, pricePiece);


                int cityLength = delimiterIndexes[0] - lastNewLineIndex;
                int productLength = delimiterIndexes[1] - delimiterIndexes[0] - 1;

                ByteBuffer cityTmp = ByteBuffer.allocate(cityLength).order(ByteOrder.LITTLE_ENDIAN);
                ByteBuffer productTmp = ByteBuffer.allocate(productLength).order(ByteOrder.LITTLE_ENDIAN);
                chunk.get(lastNewLineIndex, cityTmp.array(), 0, cityLength);
                chunk.get(delimiterIndexes[0] + 1, productTmp.array(), 0, productLength);

                Slot citySlot = cityDictionary.lookup(hash(cityTmp.array(), cityLength), cityTmp.array(), cityLength, chunks.size());
                citySlot.updateCity(chunkIndex, price);
                Slot productSlot = productDictionary.lookup(hash(productTmp.array(), productLength), productTmp.array(), productLength, chunks.size());

                productSlot.updateProduct(chunkIndex, price);

                ConcurrentHashMap<Slot, Boolean> productsMap = CITY_PRODUCTS.getOrDefault(citySlot, new ConcurrentHashMap<>());
                productsMap.putIfAbsent(productSlot, true);

                CITY_PRODUCTS.putIfAbsent(citySlot, productsMap);

                lastNewLineIndex = cursor + newLineIndex + 1;
                cursor = delimiterIndexes[1] + Long.BYTES;
                foundDelimiters = 0;
            }
        }
    }

    private static int hash(byte[] bytes, int limit) {
        int hash = 1;
        for (int i = 0; i < limit; i++) {
            hash += hash * 129 + bytes[i];
        }
        return hash;
    }

    public static class Dictionary {

        private static final int ROW_BITS = 11;
        private static final int ROWS = (1 << ROW_BITS);
        private static final int TABLE_CAPACITY = ROWS;

        private final Table table = new Table(this, nextBaseIndex());

        private static final AtomicIntegerFieldUpdater<Dictionary> BASE_INDEX_UPDATER = AtomicIntegerFieldUpdater.newUpdater(Dictionary.class, "baseIndex");
        volatile int baseIndex;

        private void forEach(Table table, Consumer<Slot> consumer) {
            for (var row : table.rows) {
                var slot = row.slot;
                if (slot != null) {
                    consumer.accept(slot);
                }
                if (row.next != null) {
                    forEach(row.next, consumer);
                }
            }
        }

        public void forEach(Consumer<Slot> consumer) {
            forEach(this.table, consumer);
        }

        public Slot lookup(int hash, byte[] key, int length, int stripes) {
            Table table = this.table;
            while (true) {
                int rowIndex = Math.abs(hash) % ROWS;
                Row row = table.rows[rowIndex];
                var storedSlot = row.slot;
                if (storedSlot == null) {
                    Slot slot = new Slot(Arrays.copyOf(key, length), stripes);
                    if (row.compareAndSet(null, slot)) {
                        return slot;
                    } else {
                        storedSlot = row.slot;
                        if (Arrays.equals(key, 0, length, storedSlot.key, 0, storedSlot.key.length)) {
                            return storedSlot;
                        }
                    }
                } else if (Arrays.equals(key, 0, length, storedSlot.key, 0, storedSlot.key.length)) {
                    return storedSlot;
                }
                table = row.getOrCreateNextTable();
                hash = Integer.rotateRight(hash, ROW_BITS);
            }
        }

        private int nextBaseIndex() {
            return BASE_INDEX_UPDATER.addAndGet(this, TABLE_CAPACITY);
        }

        private static final class Row {

            private static final AtomicReferenceFieldUpdater<Row, Table> NEXT_TABLE_UPDATER = AtomicReferenceFieldUpdater.newUpdater(Row.class, Table.class, "next");
            private static final AtomicReferenceFieldUpdater<Row, Slot> SLOT_UPDATER = AtomicReferenceFieldUpdater.newUpdater(Row.class, Slot.class, "slot");
            private volatile Slot slot = null;
            private final Dictionary dictionary;
            volatile Table next;

            private Row(Dictionary dictionary) {
                this.dictionary = dictionary;
            }

            public Table getOrCreateNextTable() {
                Table next = this.next;
                if (next == null) {
                    Table newTable = new Table(dictionary, dictionary.nextBaseIndex());
                    if (NEXT_TABLE_UPDATER.compareAndSet(this, null, newTable)) {
                        next = newTable;
                    } else {
                        next = this.next;
                    }
                }
                return next;
            }

            public boolean compareAndSet(Slot expected, Slot newSlot) {
                return SLOT_UPDATER.compareAndSet(this, expected, newSlot);
            }
        }

        private static final class Table {

            final Row[] rows;
            final int baseIndex;

            private Table(Dictionary dictionary, int baseIndex) {
                this.baseIndex = baseIndex;
                this.rows = new Row[ROWS];
                Arrays.setAll(rows, i -> new Row(dictionary));
            }
        }
    }


    record Slot(byte[] key, double[] aggregates) implements Comparable {
        private static final int WIDTH = 8;

        private static double[] newAggregates(int stripes) {
            var aggregates = new double[stripes * WIDTH];
            for (int i = 0; i < aggregates.length; i += WIDTH) {
                aggregates[i] = Integer.MAX_VALUE;
                aggregates[i + 1] = Integer.MIN_VALUE;
            }
            return aggregates;
        }

        Slot(byte[] key, int stripes) {
            this(key, newAggregates(stripes));
        }

        void updateCity(int stripe, double value) {
            int i = stripe * WIDTH;
            aggregates[i] = Math.min(value, aggregates[i]);
            aggregates[i + 1] = Math.max(value, aggregates[i + 1]);
            aggregates[i + 2] += value;
            aggregates[i + 3]++;
        }

        void updateProduct(int stripe, double value) {
            int i = stripe * WIDTH;
            aggregates[i] = Math.min(value, aggregates[i]);
            aggregates[i + 1] = Math.max(value, aggregates[i + 1]);
            aggregates[i + 2] += value;
            aggregates[i + 3]++;
        }


        public double getCityResult() {
            double sum = 0;
            for (int i = 0; i < aggregates.length; i += WIDTH) {
                sum += aggregates[i + 2];
            }
            return sum;
        }

        public double getProductResult() {
            double min = Integer.MAX_VALUE;
            for (int i = 0; i < aggregates.length; i += WIDTH) {
                min = Math.min(min, aggregates[i]);
            }
            return min;
        }


        public String toKey() {
            return new String(key, StandardCharsets.UTF_8);
        }

        @Override
        public int compareTo(Object o) {
            Slot incoming = (Slot) o;
            return Double.compare(this.getProductResult(), incoming.getProductResult());
        }
    }


    private static String toBinaryString(long num) {
        String binaryString = Long.toBinaryString(num);

        return String.format("%64s", binaryString).replace(' ', '0');
    }
}
