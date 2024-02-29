import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteOrder;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.function.Function;
import java.util.stream.Collectors;


public class Main {
    public static final int AVAILABLE_PROCESSORS = 16;
    private static final String FILE = "input.txt";
    public static final String OUTPUT_FILE = "output.txt";

    private static final int MAX = 99900;
    private static final int MAX_CITIES = 101;
    private static final int HASH_TABLE_SIZE = 1 << 17;
    public static final long DELIMITER = 0x2C2C2C2C2C2C2C2CL;
    public static final long NEW_LINE = 0x0A0A0A0A0A0A0A0AL;
    public static final long DOT = 0x2E2E2E2E2E2E2E2EL;

    public static void main(String[] args) throws IOException, InterruptedException {
       // long start = System.currentTimeMillis();
        process();
        //long end = System.currentTimeMillis();
        //System.out.println("end = " + (end - start));
    }

    private static List<MappedByteBuffer> buildChunks() throws IOException {
        var file = new RandomAccessFile(FILE, "r");
        var fileChannel = file.getChannel();
        var fileSize = fileChannel.size();
        var chunkSize = Math.min(Integer.MAX_VALUE - 512, fileSize / (AVAILABLE_PROCESSORS));
        if (chunkSize <= 0) {
            chunkSize = fileSize;
        }
        var chunks = new ArrayList<MappedByteBuffer>((int) (fileSize / chunkSize) + 1);
        var start = 0L;
        while (start < fileSize) {
            var pos = start + chunkSize;
            if (pos < fileSize) {
                file.seek(pos);
                while (file.read() != '\n') {
                    pos += 1;
                }
                pos += 1;
            } else {
                pos = fileSize;
            }

            var buf = fileChannel.map(FileChannel.MapMode.READ_ONLY, start, pos - start);
            buf.order(ByteOrder.nativeOrder());
            chunks.add(buf);
            start = pos;
        }
        return chunks;
    }

    public static void process() throws IOException, InterruptedException {
        List<MappedByteBuffer> chunks = buildChunks();

        ExecutorService executorService = Executors.newFixedThreadPool(AVAILABLE_PROCESSORS);
        List<CityResult>[] cityResuls = new List[chunks.size()];
        List<ProductResult>[] productsResuls = new List[chunks.size()];
        for (int i = 0; i < chunks.size(); ++i) {
            final int index = i;
            executorService.execute(() -> {
                List<CityResult> cityResults = new ArrayList<>(MAX_CITIES);
                List<ProductResult> productResults = new ArrayList<>(MAX_CITIES);
                parseLoop(chunks.get(index), cityResults, productResults);
                cityResuls[index] = cityResults;
                productsResuls[index] = productResults;
            });
        }

        executorService.shutdown();
        executorService.awaitTermination(1, TimeUnit.MINUTES);

        CityResult minSumCity = Arrays.stream(cityResuls).parallel().flatMap(Collection::stream).collect(Collectors.toMap(
                        Result::calcName,
                        Function.identity(),
                        (cityResult, cityResult2) -> {
                            cityResult.accumulate(cityResult2);
                            return cityResult;
                        }
                )).entrySet()
                .stream()
                .parallel()
                .min(Comparator.comparing(entry -> entry.getValue().sum))
                .get()
                .getValue();

        List<ProductResult> min5Products = Arrays.stream(productsResuls).parallel().flatMap(Collection::stream).collect(Collectors.toMap(
                        Result::calcName,
                        Function.identity(),
                        (cityResult, cityResult2) -> {
                            cityResult.accumulate(cityResult2);
                            return cityResult;
                        }
                )).entrySet()
                .stream()
                .parallel()
                .sorted(Comparator.<Map.Entry<String, ProductResult>, Integer>comparing(entry -> entry.getValue().min).thenComparing(enty -> enty.getKey()))
                .limit(5)
                .map(Map.Entry::getValue)
                .collect(Collectors.toList());

        List<String> finalResult = new ArrayList<>();
        finalResult.add(minSumCity.toString());
        min5Products.forEach(productResult -> finalResult.add(productResult.toString()));

        writeOutput(finalResult);

    }

    public static void writeOutput(List<String> result) {
        try {
            Files.write(Paths.get(OUTPUT_FILE), result.stream().collect(Collectors.joining("\n")).getBytes());
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }


    private static void parseLoop(MappedByteBuffer mappedByteBuffer, List<CityResult> collectedCityResults, List<ProductResult> collectedProductResults) {
        CityResult[] cityResults = new CityResult[HASH_TABLE_SIZE];
        ProductResult[] productResults = new ProductResult[HASH_TABLE_SIZE];

        while (mappedByteBuffer.position() < mappedByteBuffer.limit()) {
            //long start = System.nanoTime();
            FindSeparatorResult citySeparatorResult = Scanner.nextSeparator(mappedByteBuffer);
            mappedByteBuffer.position(citySeparatorResult.nameAddress + citySeparatorResult.nameLength + 1);

            int cityIndex = hashToIndex(citySeparatorResult.hash);
            CityResult cityResult = cityResults[cityIndex];
            if (cityResult == null) {
                cityResult = newCityEntry(cityResults, citySeparatorResult.nameAddress, cityIndex, citySeparatorResult.nameLength, collectedCityResults, mappedByteBuffer);
            }
            FindSeparatorResult productSeparatorResult = Scanner.nextSeparator(mappedByteBuffer);
            mappedByteBuffer.position(productSeparatorResult.nameAddress + productSeparatorResult.nameLength + 1);
            int productIndex = hashToIndex(productSeparatorResult.hash);
            ProductResult productResult = productResults[productIndex];
            if (productResult == null) {
                productResult = newProductEntry(productResults, productSeparatorResult.nameAddress, productIndex, productSeparatorResult.nameLength, collectedProductResults, mappedByteBuffer);
            }
            int number = Scanner.scanNumber(mappedByteBuffer);
            cityResult.sum += number;
            if (productResult.min > number) {
                productResult.min = number;
            }
            //long end = System.nanoTime();
            // System.out.println("end parse = " + (end - start));
        }

    }

    private static int hashToIndex(long hash) {
        long hashAsInt = hash ^ (hash >>> 33) ^ (hash >>> 15);
        return (int) (hashAsInt & (HASH_TABLE_SIZE - 1));
    }


    private static CityResult newCityEntry(CityResult[] cityResults, int nameAddress, int hash, int nameLength, List<CityResult> collectedCityResults, MappedByteBuffer mappedByteBuffer) {
        CityResult r = new CityResult();
        cityResults[hash] = r;
        fillResult(nameAddress, nameLength, r, mappedByteBuffer);
        collectedCityResults.add(r);
        return r;
    }

    private static void fillResult(int nameAddress, int nameLength, Result r, MappedByteBuffer mappedByteBuffer) {
        r.mappedByteBuffer = mappedByteBuffer;
        r.nameAddress = nameAddress;
        r.nameLength = nameLength;
    }

    private static ProductResult newProductEntry(ProductResult[] productResults, int nameAddress, int hash, int nameLength, List<ProductResult> collectedCityResults, MappedByteBuffer mappedByteBuffer) {
        ProductResult r = new ProductResult();
        productResults[hash] = r;
        fillResult(nameAddress, nameLength, r, mappedByteBuffer);
        collectedCityResults.add(r);
        return r;
    }


    private static abstract class Result {
        MappedByteBuffer mappedByteBuffer;
        int nameAddress;
        int nameLength;
        String name;

        public String calcName() {
            if (name == null) {
                byte[] array = new byte[nameLength];
                for (int i = 0; i < nameLength; ++i) {
                    array[i] = mappedByteBuffer.get(nameAddress + i);
                }
                name = new String(array, java.nio.charset.StandardCharsets.UTF_8);
            }
            return name;
        }

        abstract void accumulate(Result other);
    }

    private static final class FindSeparatorResult {
        private final long hash;
        private final int nameLength, nameAddress;

        public FindSeparatorResult(long hash, int nameAddress, int nameLength) {
            this.hash = hash;
            this.nameAddress = nameAddress;
            this.nameLength = nameLength;
        }
    }

    private static final class ProductResult extends Result {
        int min;

        private ProductResult() {
            this.min = MAX;
        }

        public String toString() {
            return String.format("%s %.2f", calcName(), min / (double) 100);
        }


        public void accumulate(Result other) {
            ProductResult otherProductResult = (ProductResult) other;
            if (min > otherProductResult.min) {
                min = otherProductResult.min;
            }
        }


    }

    private static final class CityResult extends Result {
        long sum;

        private CityResult() {
        }

        public String toString() {
            return String.format("%s %.2f", calcName(), sum / (double) 100);
        }

        public void accumulate(Result other) {
            sum += ((CityResult) other).sum;
        }
    }

    private static final class Scanner {

        static int scanNumber(MappedByteBuffer mappedByteBuffer) {
            int numberAddress = mappedByteBuffer.position();
            while (mappedByteBuffer.position() < mappedByteBuffer.limit()) {
                if (mappedByteBuffer.get() == '.') {
                    break;
                }
            }
            int numberLengthD = mappedByteBuffer.position() - numberAddress - 1;
            mappedByteBuffer.position(numberAddress);

            byte priceD = getNumberFromBytes(mappedByteBuffer, numberLengthD);

            numberAddress = mappedByteBuffer.position() + numberLengthD + 1;
            mappedByteBuffer.position(numberAddress);

            while (mappedByteBuffer.position() < mappedByteBuffer.limit()) {
                if (mappedByteBuffer.get() == '\n') {
                    break;
                }
            }
            int numberLengthF = mappedByteBuffer.position() - numberAddress - 1;
            mappedByteBuffer.position(numberAddress);
            byte priceF = getNumberFromBytes(mappedByteBuffer, numberLengthF);

            mappedByteBuffer.position(mappedByteBuffer.position() + numberLengthF + 1);

            priceF *= numberLengthF < 2 ? 10 : 1;

            int price = priceD * 100 + priceF;
            return price;
        }

        static byte[][] multipliers = {
                {0, 1, 10, 100},   // Multipliers for the first digit
                {0, 0, 1, 10},    // Multipliers for the second digit
                {0, 0, 0, 1},  // Multipliers for the third digit
        };
        static int[] asciiZeroOffset = {0, '0', 11 * '0', 111 * '0'};

        static byte getNumberFromBytes(MappedByteBuffer mappedByteBuffer, int length) {
            byte result = (byte) (multipliers[0][length] * getByteFromMAt(mappedByteBuffer, 0, length) +
                    multipliers[1][length] * getByteFromMAt(mappedByteBuffer, 1, length) +
                    multipliers[2][length] * getByteFromMAt(mappedByteBuffer, 2, length) - asciiZeroOffset[length]);
            return result;
        }

        static byte getByteFromMAt(MappedByteBuffer mappedByteBuffer, int index, int length) {
            return index < length ? mappedByteBuffer.get(mappedByteBuffer.position() + index) : 0;
        }

        static long findDelimiter(long word) {
            return find(word, DELIMITER);
        }

        static long findNewLine(long word) {
            return find(word, NEW_LINE);
        }

        static long findDot(long word) {
            return find(word, DOT);
        }

        private static long find(long word, long delimiter) {
            long input = word ^ delimiter;
            return (input - 0x0101010101010101L) & ~input & 0x8080808080808080L;
        }

        static FindSeparatorResult nextSeparator(MappedByteBuffer mappedByteBuffer) {
            //long start = System.nanoTime();

            int nameAddress = mappedByteBuffer.position();
            long result;
            int nameLength = 0;
            while ((result = findDelimiter(read(mappedByteBuffer))) == 0) {
                nameLength += 8;
            }
            int tailingsZeros = Long.numberOfTrailingZeros(result) >>> 3;
            nameLength += tailingsZeros;
            int hash = hash(mappedByteBuffer, nameAddress, nameLength);
            FindSeparatorResult findSeparatorResult = new FindSeparatorResult(hash, nameAddress, nameLength);
            // long end = System.nanoTime();
            // System.out.println("end findSeparator = " + (end - start));
            return findSeparatorResult;
        }

        private static final int GOLDEN_RATIO = 0x9E3779B9;
        private static final int HASH_LROTATE = 5;

        static int hash(MappedByteBuffer mappedByteBuffer, int start, int len) {
            int x, y;
            if (len >= Integer.BYTES) {
                x = mappedByteBuffer.getInt(start);
                y = mappedByteBuffer.getInt(start + len - Integer.BYTES);
            } else {
                x = mappedByteBuffer.get(start) & 0xFF;
                y = mappedByteBuffer.get(start + len - Byte.BYTES) & 0xFF;
            }
            return (Integer.rotateLeft(x * GOLDEN_RATIO, HASH_LROTATE) ^ y) * GOLDEN_RATIO;
        }

        private static long read(MappedByteBuffer mappedByteBuffer) {
            int remainingBytes = mappedByteBuffer.limit() - mappedByteBuffer.position();
            return remainingBytes >= 8 ? mappedByteBuffer.getLong() : (remainingBytes >= 4 ? mappedByteBuffer.getInt() : (remainingBytes >= 2 ? mappedByteBuffer.getChar() : mappedByteBuffer.get()));
        }
    }
}