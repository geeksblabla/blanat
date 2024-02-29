import jdk.incubator.foreign.MemorySegment;
import jdk.incubator.foreign.ResourceScope;
import sun.misc.Unsafe;

import java.io.*;
import java.lang.reflect.Field;
import java.nio.file.Path;
import java.util.*;
import java.util.stream.Collectors;

import static java.nio.channels.FileChannel.MapMode.READ_ONLY;


/**
 ************************************************************************************************
 *
 *  Note: To execute this program, thanks to use the following command
 *
 *          java   --enable-preview --source 17 --add-modules=jdk.incubator.foreign Main.java
 *
 ************************************************************************************************
 *
 */
public class Main {

    public static final String[] FRUITS_AND_VEGETABLES = {
            "Apple", "Banana", "Orange", "Strawberry", "Grapes",
            "Watermelon", "Pineapple", "Mango", "Kiwi", "Peach",
            "Plum", "Cherry", "Pear", "Blueberry", "Raspberry",
            "Blackberry", "Cantaloupe", "Honeydew", "Coconut", "Pomegranate",
            "Lemon", "Lime", "Grapefruit", "Avocado", "Papaya",
            "Guava", "Fig", "Passion_Fruit", "Apricot", "Nectarine",
            "Cucumber", "Carrot", "Broccoli", "Spinach", "Kale",
            "Lettuce", "Tomato", "Bell_Pepper", "Zucchini", "Eggplant",
            "Cabbage", "Cauliflower", "Brussels_Sprouts", "Radish", "Beet",
            "Asparagus", "Artichoke", "Green_Beans", "Peas", "Celery",
            "Onion", "Garlic", "Potato", "Sweet_Potato", "Yam",
            "Butternut_Squash", "Acorn_Squash", "Pumpkin", "Cranberry", "Goji_Berry",
            "Currant", "Date", "Clementine", "Cranberry", "Rhubarb",
            "Chard", "Collard_Greens", "Parsley", "Cilantro", "Mint",
            "Basil", "Thyme", "Rosemary", "Sage", "Dill",
            "Oregano", "Cantaloupe", "Honeydew", "Coconut", "Pomegranate",
            "Jackfruit", "Starfruit", "Persimmon", "Ginger", "Turnip",
            "Jicama", "Kohlrabi", "Watercress", "Okra", "Artichoke",
            "Plantain", "Cactus_Pear", "Kiwano", "Squash_Blossom", "Dragon_Fruit",
            "Parsnip", "Rutabaga", "Salsify", "Bok_Choy", "Endive"
    };

    public static final String[] MOROCCAN_CITIES = {
            "Casablanca", "Rabat", "Marrakech", "Fes", "Tangier",
            "Agadir", "Meknes", "Oujda", "Kenitra", "Tetouan",
            "Safi", "El_Jadida", "Beni_Mellal", "Errachidia",
            "Taza", "Essaouira", "Khouribga", "Guelmim",
            "Jorf_El_Melha", "Laayoune", "Ksar_El_Kebir", "Sale", "Bir_Lehlou",
            "Arfoud", "Temara", "Mohammedia", "Settat",
            "Béni_Mellal", "Nador", "Kalaat_MGouna",
            "Chichaoua", "Chefchaouen", "Al_Hoceima", "Taourirt",
            "Taroudant", "Guelta_Zemmur", "Dakhla", "Laâyoune",
            "Tiznit", "Tinghir", "Ifrane", "Azrou", "Bab_Taza",
            "Berrechid", "Sidi_Slimane", "Souk_Larbaa", "Tiflet", "Sidi_Bennour",
            "Larache", "Tan-Tan", "Sidi_Ifni", "Goulmima",
            "Midelt", "Figuig", "Azilal", "Jerada", "Youssoufia",
            "Ksar_es_Seghir", "Tichka", "Ait_Melloul",
            "Layoune", "Ben_guerir", "Ouarzazate", "Inezgane",
            "Oujda_Angad", "Sefrou", "Aourir",
            "Oulad_Teima", "Tichla", "Bni_Hadifa",
            "Fquih_Ben_Salah", "Guercif", "Bouarfa", "Demnate",
            "Ahfir", "Berkane", "Akhfenir", "Boulemane",
            "Khenifra", "Bir_Anzerane", "Assa", "Smara", "Boujdour",
            "Tarfaya", "Ouazzane", "Zagora", "had_soualem",
            "Saidia", "Bab_Berred", "Midar", "Moulay_Bousselham",
            "Khemisset", "Guerguerat", "Asilah", "Sidi_Bouzid", "Tafraout",
            "Imzouren", "Zemamra", "Sidi_Kacem", "Drarga", "Skhirate"
    };


    private static final String FILE = "input.txt";

    private static final int LF = 0x0A;
    public static final Unsafe UNSAFE;

    static {
        try {
            Field f = Unsafe.class.getDeclaredField("theUnsafe");
            f.setAccessible(true);
            UNSAFE = (Unsafe) f.get(null);
        } catch (IllegalAccessException e) {
            throw new RuntimeException(e);
        } catch (NoSuchFieldException e) {
            throw new RuntimeException(e);
        }


    }

    public static void main(String[] args) throws Exception {
        process();
    }

    public static void process() throws Exception {
        try(final var raf  = new RandomAccessFile(FILE, "r");
            final var writer = new PrintWriter(new FileWriter("output.txt", false));
        ) {

            // set up mapped memory segment with java foreign api, MappedByteBuffer does not support mapped memory with more than 2GB(array of Integer.MAX_VALUE bytes)
            var mainMappedmemorySegment = MemorySegment.mapFile(
                    Path.of(FILE),
                    0,
                    raf.length(),
                    READ_ONLY,
                    ResourceScope.globalScope()
            );


            // number of workers equal number of machine cores
            var numberOfWorkers = Runtime.getRuntime().availableProcessors();

            // we divide the file into chunks (number of machine cores)
            // each worker will work on one chunk of the file
            // for each worker we calculate the start offset of its memory chunk
            var workersStartOffset = new long[numberOfWorkers];
            for (int i = 1; i < numberOfWorkers; ++i) {
                var start = raf.length() / numberOfWorkers * i;
                raf.seek(start);
                while ((raf.read() ^ LF) != 0){}
                start = raf.getFilePointer();
                workersStartOffset[i] = start;
            }


            // for each worker we create a MemorySegment on which will work
            var cheapestCityWorkers = new CheapestCityWorker[numberOfWorkers];

            for (var i = 0; i < numberOfWorkers; ++i) {
                var startOffset = workersStartOffset[i];
                var endOffset   = (i < numberOfWorkers - 1) ? workersStartOffset[i + 1] : raf.length();
                var segmentSize = endOffset - startOffset;

                cheapestCityWorkers[i] = new CheapestCityWorker(mainMappedmemorySegment.asSlice(startOffset, segmentSize));
            }



            // create threads and run them
            var threads = new Thread[numberOfWorkers];
            for (var i = 0; i < numberOfWorkers; ++i) {
                threads[i] = new Thread(cheapestCityWorkers[i]);
                threads[i].setPriority(Thread.MAX_PRIORITY);
                threads[i].start();
            }
            for (var thread: threads) thread.join();

            // calculate total price for each
            var totalPrice = new long[CheapestCityWorker.CITES_SLOTS_SIZE + 1];
            for (int i = 0; i < totalPrice.length; ++i) {
                for (var worker: cheapestCityWorkers) {
                    totalPrice[i] += UNSAFE.getLong(worker.citiesTotalMemAddr[i]);
                }
            }


            // find the cheapest city by name
            var index = -1;
            var value = Long.MAX_VALUE;
            for (var i = 0; i < totalPrice.length; ++i) {
                if (totalPrice[i] > 0 && totalPrice[i] < value) {
                    value = totalPrice[i];
                    index = i;
                }
            }

            var cityName = "";
            for (var city : MOROCCAN_CITIES) {
                if ((Arrays.hashCode(city.getBytes()) & CheapestCityWorker.CITES_SLOTS_SIZE) == index) {
                    cityName = city;
                    break;
                }
            }

            writer.printf("%s %.2f", cityName, value / 100.00).println();

            //***********************************************************

            // product workers
            var cheapestProductsWorkers = new CheapestProductsWorker[numberOfWorkers];
            for (int i = 0; i < numberOfWorkers; ++i) {
                var startOffset = workersStartOffset[i];
                var endOffset   = (i < numberOfWorkers - 1) ? workersStartOffset[i + 1] : raf.length();
                var segmentSize = endOffset - startOffset;
                cheapestProductsWorkers[i] =  new CheapestProductsWorker(mainMappedmemorySegment.asSlice(startOffset, segmentSize), cityName, index);
            }

            threads = new Thread[numberOfWorkers];
            for (var i = 0; i < numberOfWorkers; ++i) {
                threads[i] = new Thread(cheapestProductsWorkers[i]);
                threads[i].setPriority(Thread.MAX_PRIORITY);
                threads[i].start();
            }
            for (var thread: threads) thread.join();

            Arrays.stream(cheapestProductsWorkers)
                    .flatMap(th -> th.products.entrySet().stream())
                    .filter(p -> p.getValue() > 0)
                    .collect(Collectors.toMap(
                            Map.Entry::getKey,
                            Map.Entry::getValue,
                            Long::min))
                    .entrySet()
                    .stream()
                    .sorted(Comparator.comparing(Map.Entry<String, Long>::getValue)
                            .thenComparing(Map.Entry::getKey))
                    .limit(5)
                    .forEach(entry -> writer.printf("%s %.2f", entry.getKey(), entry.getValue() / 100.).println());

        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}


class Worker {

    public static final int COMMA = 0X2C;
    public static final int LF = 0x0A;
    public static final int PERIOD = 0x2E;
    public static final int CITES_SLOTS_SIZE = 6655;
    protected MemorySegment segment;
    protected long segmentAddress;
    protected long segmentSize;

    protected int calculateCityIndex(byte[] bytes, int from , int to) {
        int result = 1;
        for (int i = from; i <= to; ++i)
            result = 31 * result + bytes[i];
        return result & CITES_SLOTS_SIZE;
    }

    protected long parsePrice(byte[] bytes, int from, int to) {

        long value = 0;
        int dotPos = -1;
        for (var pos = from; pos <= to; ++pos) {
            if ((PERIOD ^ bytes[pos]) != 0) {
                value = (value * 10) + (bytes[pos] - '0');
            }else {
                dotPos = pos;
            }
        }

        if (dotPos + 1 == to) {
            value *= 10;
        }
        return value;
    }
}




class CheapestCityWorker extends Worker implements Runnable {

    public final long[] citiesTotalMemAddr = new long[CITES_SLOTS_SIZE + 1];


    public CheapestCityWorker(MemorySegment memorySegment) {
        segment = memorySegment;
        segmentAddress = segment.address().toRawLongValue();
        segmentSize = segment.byteSize();

        for (var i = 0; i < citiesTotalMemAddr.length; ++i) {
            citiesTotalMemAddr[i] = Main.UNSAFE.allocateMemory(8);
            Main.UNSAFE.putLong(citiesTotalMemAddr[i], 0);
        }
    }

    @Override
    public void run() {

        long limit = segmentAddress + segmentSize;

        int city = -1;
        int priceFrom = -1;
        int priceTo = -1;
        int pointer = 0;
        byte[] arr = new byte[100];

        for (var cursor = segmentAddress; cursor <= limit; ++cursor ) {
            byte aByte = Main.UNSAFE.getByte(cursor);
            arr[pointer] = aByte;

            switch (aByte) {
                case COMMA:
                    if (city == -1) {
                        city = pointer - 1;
                    }else {
                        priceFrom = pointer + 1;
                    }
                    break;
                case LF:{
                    priceTo = pointer - 1;
                    int index = calculateCityIndex(arr, 0, city);
                    long price = parsePrice(arr, priceFrom, priceTo);

                    long total = Main.UNSAFE.getLong(citiesTotalMemAddr[index]);
                    Main.UNSAFE.putLong(citiesTotalMemAddr[index], total + price);

                    pointer = 0;
                    city = -1;
                    priceFrom = -1;
                    continue;
                }
            }

            ++pointer;
        }
    }
}

class CheapestProductsWorker extends Worker implements Runnable {


    Map<String, Long> products = new HashMap<>(Main.FRUITS_AND_VEGETABLES.length);


    private final int cityBytesSize;

    private final int citySlot;

    public CheapestProductsWorker(MemorySegment memorySegment, String cheapestCity, int citySlot) {
        segment = memorySegment;
        segmentAddress = segment.address().toRawLongValue();
        segmentSize = segment.byteSize();
        this.citySlot = citySlot;
        cityBytesSize = cheapestCity.getBytes().length;

        for(var product : Main.FRUITS_AND_VEGETABLES) products.put(product, Long.MAX_VALUE);
    }

    @Override
    public void run() {

        long limit = segmentAddress + segmentSize;

        int city = -1;
        int priceFrom = -1;
        int priceTo = -1;
        int p = 0;
        byte[] arr = new byte[100];

        for (var cursor = segmentAddress; cursor <= limit; ++cursor ) {
            byte aByte = Main.UNSAFE.getByte(cursor);
            arr[p] = aByte;

            switch (aByte) {
                case COMMA:
                    if (city == -1) {
                        city = p - 1;
                    }else {
                        priceFrom = p + 1;
                    }
                    break;
                case LF:{
                    priceTo = p - 1;
                    long price = parsePrice(arr, priceFrom, priceTo);

                    if (cityBytesSize != city + 1) {
                        p = 0;
                        city = -1;
                        priceTo = -1;
                        priceFrom = -1;
                        continue;
                    }

                    int index = calculateCityIndex(arr, 0, city);
                    if (citySlot != index) {
                        p = 0;
                        city = -1;
                        priceTo = -1;
                        priceFrom = -1;
                        continue;
                    }

                    var product = new String(Arrays.copyOfRange(arr, city + 2, priceFrom - 1));

                    products.put(product, Math.min(products.get(product), price));


                    p = 0;
                    city = -1;
                    priceTo = -1;
                    priceFrom = -1;
                    continue;
                }
            }
            ++p;
        }
    }
}
