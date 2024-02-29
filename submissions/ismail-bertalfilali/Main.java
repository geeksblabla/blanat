
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.time.Duration;
import java.time.LocalDateTime;
import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;


public class Main {

    private static final int THREAD_COUNT = 8;
    private static final String INPUT_FILE = "input.txt";
    private static final String OUTPUT_FILE = "./output.txt";
    private static final Map<Long, Map<CharArrayKey, City>> globalMapByThreadId = new HashMap<>();
    public static final byte RETURN_CHAR = '\n';
    public static final byte COMMA_CHAR = ',';
    public static final byte DOT_CHAR = '.';

    public static final byte ZERO_CHAR = '0';

    public static final byte NUL_CHAR = '\u0000';
    public static final byte MAX_CHAR_PER_LINE = 42;
    public static final int BUFFER_CAPACITY = 10_024_000;
    public static final int CALCULATION_FREQUENCY = 1_000_000;

    public static final byte ZERO = 0b00000000;

    public static void main(String[] args) throws Exception {
        //Thread.sleep(30_000);
        LocalDateTime start = LocalDateTime.now();


        ExecutorService executor = Executors.newFixedThreadPool(THREAD_COUNT);

        partitioning(executor);

        executor.shutdown();
        executor.awaitTermination(Long.MAX_VALUE, TimeUnit.NANOSECONDS);

        LocalDateTime end = LocalDateTime.now();
        System.out.println(Duration.between(start,end).toString());
        start = LocalDateTime.now();


        Map<CharArrayKey, City> mergeCitiesAndProducts = mergeCitiesAndProducts();

        Map.Entry<CharArrayKey, City> cheapestCity = findCheapestCity(mergeCitiesAndProducts);

        String cheapestCityName = new String(cheapestCity.getKey().data,StandardCharsets.US_ASCII);
        TreeSet<Product> sortedProducts = sortProductsByPriceAscAndNameAsc(cheapestCity);
        writeResult(cheapestCity, cheapestCityName, sortedProducts);


        end = LocalDateTime.now();
        System.out.println(Duration.between(start,end).toString());
    }

    private static void writeResult(Map.Entry<CharArrayKey, City> cheapestCity, String cheapestCityName, TreeSet<Product> sortedProducts) throws IOException {
        List<String> result = new ArrayList<>();
        result.add(cheapestCityName + " " + String.format( "%.2f", cheapestCity.getValue().total/100.0));


        int count =0;
        for (Product product : sortedProducts) {
            if(count<5){
                result.add(product.name + " " + String.format( "%.2f", product.minPrice/100.0));
                count++;
            }else {
                break;
            }

        }

        Files.write(Paths.get(OUTPUT_FILE), result,StandardCharsets.US_ASCII);
    }

    private static TreeSet<Product> sortProductsByPriceAscAndNameAsc(Map.Entry<CharArrayKey, City> cheapestCity) {
        TreeSet<Product> sortedProducts = new TreeSet<>();
        for (Map.Entry<CharArrayKey, ShortWrapper> product :
                cheapestCity.getValue().products.entrySet()) {
            sortedProducts.add(new Product(new String(product.getKey().data,StandardCharsets.US_ASCII),product.getValue().value));
        }
        return sortedProducts;
    }

    private static Map.Entry<CharArrayKey, City> findCheapestCity(Map<CharArrayKey, City> mergeCitiesAndProducts) {
        Map.Entry<CharArrayKey, City> cheapestCity= null;
        long minTotal =Long.MAX_VALUE;
        for (Map.Entry<CharArrayKey, City> city:
                mergeCitiesAndProducts.entrySet()) {
            if(city.getValue().total < minTotal){
                cheapestCity = city;
                minTotal = city.getValue().total;
            }
        }
        return cheapestCity;
    }

    private static Map<CharArrayKey, City> mergeCitiesAndProducts() {
        Map<CharArrayKey, City> mergedMap = new HashMap<>();
        for ( Map.Entry<Long, Map<CharArrayKey, City>> entry:
             globalMapByThreadId.entrySet()) {
            mergeOneCity(mergedMap, entry);
        }
        return mergedMap;
    }

    private static void mergeOneCity(Map<CharArrayKey, City> mergedMap, Map.Entry<Long, Map<CharArrayKey, City>> entry) {
        for (Map.Entry<CharArrayKey, City> city :
                entry.getValue().entrySet()) {
            City mergedCity = mergedMap.get(city.getKey());
            if (mergedCity == null) {
                mergedCity = new City();
                mergedMap.put(city.getKey(), mergedCity);
            }
            mergedCity.total += city.getValue().total;

            mergeProductsOfOneCity(city, mergedCity);
        }
    }

    private static void mergeProductsOfOneCity(Map.Entry<CharArrayKey, City> city, City mergedCity) {
        Map<CharArrayKey, ShortWrapper> mergedMapOfProductsByCity = mergedCity.products;


        for (Map.Entry<CharArrayKey, ShortWrapper> product :
                city.getValue().products.entrySet()) {
            ShortWrapper minPriceOfProduct = mergedMapOfProductsByCity.get(product.getKey());
            if (minPriceOfProduct == null) {
                minPriceOfProduct = new ShortWrapper(product.getValue().value);
                mergedMapOfProductsByCity.put(product.getKey(), minPriceOfProduct);
            }
            minPriceOfProduct.value = (short) Math.min(minPriceOfProduct.value,product.getValue().value);
        }
    }

    private static void partitioning(ExecutorService executor) throws IOException {
        long fileSize =0;
        try(RandomAccessFile randomAccessFile = new RandomAccessFile(INPUT_FILE, "r");) {
            fileSize = randomAccessFile.length();
        }
        long segmentSize = fileSize / THREAD_COUNT;

        for (int i = 0b00000000; i < THREAD_COUNT; i++) {
            long startOffset = i * segmentSize;
            long endOffset = (i + 1) * segmentSize;
            if (i == THREAD_COUNT - 1) {
                endOffset = fileSize; // Ensure the last thread covers the rest of the file
            }
            executor.execute(new FileProcessorThread(startOffset, endOffset, i == 0b00000000));
        }
    }

    private static class FileProcessorThread implements Runnable {

        private final long startOffset;
        private final long endOffset;
        private final boolean isFirstSegment;

        public FileProcessorThread(long startOffset, long endOffset, boolean isFirstSegment) {
            this.startOffset = startOffset;
            this.endOffset = endOffset;
            this.isFirstSegment = isFirstSegment;
        }

        @Override
        public void run() {

            process();
        }

        private void process() {


            try (RandomAccessFile fis = new RandomAccessFile(INPUT_FILE, "r");
                 FileChannel fileChannel = fis.getChannel()) {

                fileChannel.position(startOffset);
                skipToNextLineToAvoidParallelRead(fileChannel);

                processOneSegmentOfFile( fileChannel);

            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        private void processOneSegmentOfFile( FileChannel fileChannel) throws IOException {
            long threadId = Thread.currentThread().getId();
            Map<CharArrayKey, City> cities = getCityMap(threadId);
            ByteBuffer buffer = ByteBuffer.allocateDirect(BUFFER_CAPACITY);
            long currentPosition = fileChannel.position();
            long currentLineNumber = 0l;
            byte [] charArray = new byte[MAX_CHAR_PER_LINE];
            CharArrayKey cityArray = null;
            CharArrayKey productArray = null;
            byte [] priceArray = null;
            byte i = ZERO;
            byte indexOfFirstComma= ZERO;
            byte indexOfSecondComma= ZERO;

            byte c;
            while (isOffsetNotReached(currentPosition) && buffer.clear()!= null && fileChannel.read(buffer) > ZERO && buffer.flip()!= null) {

                while (isOffsetNotReached(currentPosition)) {

                    try{
                        c = buffer.get();
                    }catch (BufferUnderflowException e){
                        //end of buffer reached
                        break;
                    }


                    currentPosition++;


                    if (isReturnChar(c)) {

                        if(mustCalculate(currentLineNumber)) {
                            calculateBiggestOfAllMins(cities);
                        }
                        assignCharToArray(charArray, i, NUL_CHAR);
                        priceArray = Arrays.copyOfRange(charArray, incrementByOne(indexOfSecondComma), incrementByOne(i));


                        processLine(cities, cityArray,productArray,priceArray);
                        i= ZERO;

                        indexOfFirstComma = ZERO;
                        indexOfSecondComma= ZERO;
                        currentLineNumber++;
                    }else {
                        if (isCommaChar(c)) {
                            if (indexOfFirstComma == ZERO) {
                                indexOfFirstComma = i;
                                cityArray = new CharArrayKey(Arrays.copyOf(charArray, indexOfFirstComma));

                            } else if (indexOfSecondComma == ZERO) {
                                indexOfSecondComma = i;
                                productArray = new CharArrayKey(Arrays.copyOfRange(charArray, incrementByOne(indexOfFirstComma), indexOfSecondComma));

                            }
                        }
                        assignCharToArray(charArray, i, c);
                        i = incrementByOne(i);
                    }
                }

            }
        }

        private void skipToNextLineToAvoidParallelRead(FileChannel fileChannel) throws IOException {
            if (!isFirstSegment) {
                // Skip to the next line to avoid partial reads
                skipToNextLine(fileChannel);
            }
        }

        private static Map<CharArrayKey, City> getCityMap(long threadId) {
            Map<CharArrayKey, City> cities = globalMapByThreadId.get(threadId);
            if(cities == null){
                cities = new HashMap<>();
                globalMapByThreadId.put(threadId, cities);
            }
            return cities;
        }

        private static boolean mustCalculate(long currentLineNumber) {
            return currentLineNumber % CALCULATION_FREQUENCY == ZERO;
        }

        private static byte incrementByOne(byte indexOfFirstComma) {
            return (byte) (indexOfFirstComma + 1);
        }


        private static void assignCharToArray(byte[] charArray, byte i, byte c) {
            charArray[i] = c;
        }

        private static boolean isCommaChar(byte c) {
            return (byte)(c ^ COMMA_CHAR) == ZERO;
        }

        private static boolean isNulChar(byte c) {
            return (byte)(c ^ NUL_CHAR) == ZERO;
        }

        private static boolean isDotChar(byte c) {
            return (byte)(c ^ DOT_CHAR) == ZERO;
        }

        private static boolean isReturnChar(byte c) {
            return (byte)(c ^ RETURN_CHAR) == ZERO;
        }

        private boolean isOffsetNotReached(long currentPosition) {
            return currentPosition < endOffset;
        }

        private void skipToNextLine(FileChannel fileChannel) throws IOException {
            ByteBuffer buffer = ByteBuffer.allocateDirect(1);
            while (fileChannel.read(buffer) > ZERO) {
                buffer.flip();
                if (isReturnChar(buffer.get())) {
                    break;
                }
                buffer.clear();
            }
        }

        private static void processLine(Map<CharArrayKey, City> cities,CharArrayKey cityArray, CharArrayKey productArray, byte [] priceArray) {


            byte [] priceA = priceArray;
            for (byte i = ZERO; i < priceA.length; i++) {
                if (isDotChar(priceA[i])) {
                    if (isNulChar(priceA[i+2])) {
                        priceA[i+2] = ZERO_CHAR ;

                    }
                    priceA[i] = priceA[incrementByOne(i)];
                    priceA[incrementByOne(i)] = priceA[i+2];

                    priceA = Arrays.copyOf(priceA, i+2);
                    break;
                }
            }



            short price = Short.parseShort(new String(priceA, StandardCharsets.US_ASCII));
            City city1 = cities.computeIfAbsent(cityArray,charArrayKey -> new City());

            city1.total+= price;


            Map<CharArrayKey, ShortWrapper> products = city1.products;



            if(city1.biggestOfAllMins > price) {


                ShortWrapper oldProductMinPrice = products.get(productArray);
                if (oldProductMinPrice == null) {
                    products.put(productArray, new ShortWrapper(price));
                } else {
                    oldProductMinPrice.value = (short)Math.min(oldProductMinPrice.value, price);

                }
            }
        }

        private static void calculateBiggestOfAllMins(Map<CharArrayKey, City> cities) {

            for (Map.Entry<CharArrayKey, City> city : cities.entrySet()) {
                short maxValue = ZERO;
                for (Map.Entry<CharArrayKey, ShortWrapper> entry : city.getValue().products.entrySet()) {
                    if (entry.getValue().value > maxValue) {
                        maxValue = entry.getValue().value;
                    }
                }
                city.getValue().biggestOfAllMins = maxValue;
            }

        }


    }

    private static class CharArrayKey{
        private byte[] data;
        private int hashCode = -1;

        public CharArrayKey(byte[] data) {
            this.data = data;
        }


        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            CharArrayKey that = (CharArrayKey) o;
            return Arrays.equals(data, that.data);
        }

        @Override
        public int hashCode() {
            if(hashCode == -1) {
                hashCode = Arrays.hashCode(data);
            }

            return hashCode;

        }

    }

    private static class City {
        private long total = 0;

        private short biggestOfAllMins = 10_000;

        private final Map<CharArrayKey, ShortWrapper> products = new HashMap<>();


    }

    private static class Product implements Comparable<Product>{
        private String name;
        private short minPrice;

        public Product(String name, short minPrice) {
            this.name = name;
            this.minPrice = minPrice;
        }

        @Override
        public int compareTo(Product o) {
            // First compare by minPrice
            int priceCompare = Short.compare(this.minPrice, o.minPrice);
            if (priceCompare != 0) {
                return priceCompare;
            }

            // If minPrice is the same, compare by name
            return this.name.compareTo(o.name);
        }
    }

    private static class ShortWrapper {
        private short value;

        public ShortWrapper(short value) {
            this.value = value;
        }

    }
}
