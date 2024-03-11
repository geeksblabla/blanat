import java.io.*;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {
    static SortedCities sortedCities = new SortedCities(Stream.of(
            "Agadir", "Ahfir", "Ait_Melloul", "Akhfenir", "Al_Hoceima", "Aourir", "Arfoud",
            "Asilah", "Assa", "Azilal", "Azrou", "Bab_Berred", "Bab_Taza", "Ben_guerir", "Beni_Mellal",
            "Berkane", "Berrechid", "Bir_Anzerane", "Bir_Lehlou", "Bni_Hadifa", "Bouarfa", "Boujdour",
            "Boulemane", "Béni_Mellal", "Casablanca", "Chefchaouen", "Chichaoua", "Dakhla", "Demnate",
            "Drarga", "El_Jadida", "Errachidia", "Essaouira", "Fes", "Figuig", "Fquih_Ben_Salah",
            "Goulmima", "Guelmim", "Guelta_Zemmur", "Guercif", "Guerguerat", "Ifrane", "Imzouren",
            "Inezgane", "Jerada", "Jorf_El_Melha", "Kalaat_MGouna", "Kenitra", "Khemisset", "Khenifra",
            "Khouribga", "Ksar_El_Kebir", "Ksar_es_Seghir", "Laayoune", "Larache", "Layoune", "Laâyoune",
            "Marrakech", "Meknes", "Midar", "Midelt", "Mohammedia", "Moulay_Bousselham", "Nador",
            "Ouarzazate", "Ouazzane", "Oujda", "Oujda_Angad", "Oulad_Teima", "Rabat", "Safi", "Saidia",
            "Sale", "Sefrou", "Settat", "Sidi_Bennour", "Sidi_Bouzid", "Sidi_Ifni", "Sidi_Kacem",
            "Sidi_Slimane", "Skhirate", "Smara", "Souk_Larbaa", "Tafraout", "Tan-Tan", "Tangier",
            "Taourirt", "Tarfaya", "Taroudant", "Taza", "Temara", "Tetouan", "Tichka", "Tichla", "Tiflet",
            "Tinghir", "Tiznit", "Youssoufia", "Zagora", "Zemamra", "had_soualem").map(City::new).toArray(City[]::new));

    public static void main(String[] args) throws IOException, InterruptedException {

        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(new FileInputStream("/Users/mohammedlamzira/geeksblabla/input.1b.txt")));

        int capacity = 1024;
        int elementsCapacity = 2048;
        List<Thread> jobs = new ArrayList<>();
        ChunkProvider chunkProvider = new ChunkProvider(capacity, elementsCapacity);

        Chunk chunk = chunkProvider.rent();
        String line;
        while ((line = bufferedReader.readLine()) != null) {
            if (chunk.hasCapacity()) {
                chunk.add(line);
            } else {
                jobs.add(chunk.proccesInVThread(sortedCities, chunkProvider));
                chunk = chunkProvider.rent();
                chunk.add(line);
            }
        }
        if (chunk.size() > 0) {
            Thread lastChunk = chunk.proccesInVThread(sortedCities, chunkProvider);
            lastChunk.join();
        }

        for (Thread vThread : jobs) {
            if (vThread.isAlive()) {
                vThread.join();
            }
        }

        City cheapestCity = sortedCities.getCheapestCity();
        FileOutputStream out = new FileOutputStream("./output.txt");
        PrintStream printStream = new PrintStream(out);
        printStream.print(cheapestCity.toString());
        printStream.close();
        out.flush();
        out.close();
    }

    public static int fastCompare(String a, String b) {
        int len1 = a.length();
        int len2 = b.length();
        int len = Math.min(Math.min(len1, len2), 7);

        int i = 0;
        while (i < len - 3) {
            if (a.charAt(i) != b.charAt(i)) {
                return a.charAt(i) - b.charAt(i);
            }
            if (a.charAt(i + 1) != b.charAt(i + 1)) {
                return a.charAt(i + 1) - b.charAt(i + 1);
            }
            if (a.charAt(i + 2) != b.charAt(i + 2)) {
                return a.charAt(i + 2) - b.charAt(i + 2);
            }
            i += 3;
        }
        for (; i < len; i++) {
            if (a.charAt(i) != b.charAt(i)) {
                return a.charAt(i) - b.charAt(i);
            }
        }

        return Integer.compare(len1, len2);
    }

    public static class Chunk implements Comparable<Chunk> {
        private final List<ChunkElement> elements;
        private final int initialCapacity;

        public Chunk(int initialCapacity) {
            elements = new ArrayList<>(initialCapacity);
            this.initialCapacity = initialCapacity;
        }

        public void add(String line) {
            elements.add(new ChunkElement(line));
        }

        public void add(ChunkElement chunkElement) {
            elements.add(chunkElement);
        }

        public boolean hasCapacity() {
            return elements.size() < initialCapacity;
        }

        public int size() {
            return elements.size();
        }

        public Thread proccesInVThread(SortedCities sortedCities, ChunkProvider chunkProvider) {
            return Thread.ofVirtual().start(() -> {
                elements.forEach(chunkElement -> chunkElement.processChunkElement(sortedCities));
                elements.clear();
                chunkProvider.back(this);
            });
        }

        @Override
        public int compareTo(Chunk o) {
            return elements.size() - o.elements.size();
        }
    }

    public record ChunkElement(String line) {
            public static final double[] COEFFICIENT = new double[]{100, 10, 1, 0.1f, 0.01f};

        public void processChunkElement(SortedCities sortedCities) {
                String[] parts = splitLine(line);
                double price = parse(parts[2]);
                City city = sortedCities.search(parts[0]);
                synchronized (city) {
                    city.addProduct(new Product(parts[1], price));
                }
            }

            private static double parse(String s) {
                double result = 0;
                int doubleingNumber = s.indexOf('.');
                int highestNumber = 0;
                if (doubleingNumber == 1) {
                    highestNumber = 2;
                } else if (doubleingNumber == 2) {
                    highestNumber = 1;
                } else if (doubleingNumber == 3) {
                    return 100;
                }

                int j = highestNumber;
                for (int i = 0; i < s.length(); i++) {
                    char c = s.charAt(i);
                    if (c != '.') {
                        result += Character.getNumericValue(c) * COEFFICIENT[j];
                        j++;
                    }
                }
                return result;
            }

            private static String[] splitLine(String line) {
                int firstComma = line.indexOf(',');
                int secondComma = line.indexOf(',', firstComma + 1);

                return new String[]{
                        line.substring(0, firstComma),
                        line.substring(firstComma + 1, secondComma),
                        line.substring(secondComma + 1)
                };
            }
        }

    public static class ChunkProvider {
        private final LinkedList<Chunk> chunks = new LinkedList<>();
        private final int chunksCapacity;
        private final int individualCapacity;

        public ChunkProvider(int chunksCapacity, int individualCapacity) {
            this.chunksCapacity = chunksCapacity;
            this.individualCapacity = individualCapacity;
            for (int i = 0; i < chunksCapacity; i++) {
                chunks.add(new Chunk(individualCapacity));
            }
        }

        public Chunk rent() {
            try {
                return chunks.removeLast();
            } catch (Exception e) {
                for (int i = 0; i < chunksCapacity; i++) {
                    chunks.addFirst(new Chunk(individualCapacity));
                }
                return chunks.removeLast();
            }
        }

        public void back(Chunk chunk) {
            chunks.addFirst(chunk);
        }
    }

    public static class City {
        public final String name;
        public double sumPrice = 0;
        private final TreeSet<Product> cheapestProducts = new TreeSet<>();

        public City(String name) {
            this.name = name;
        }

        public void addProduct(Product product) {
            sumPrice += product.price;
            cheapestProducts.add(product);
            if (cheapestProducts.size() == 6) {
                cheapestProducts.removeLast();
            }
        }

        @Override
        public String toString() {
            List<Product> array = cheapestProducts.stream().toList();
            Product product1 = array.get(0);
            Product product2 = array.get(1);
            Product product3 = array.get(2);
            Product product4 = array.get(3);
            Product product5 = array.get(4);
            return String.format("""
                            %s %.2f
                            %s %.2f
                            %s %.2f
                            %s %.2f
                            %s %.2f
                            %s %.2f""", name, sumPrice,
                    product1.name, product1.price,
                    product2.name, product2.price,
                    product3.name, product3.price,
                    product4.name, product4.price,
                    product5.name, product5.price
            );
        }
    }

    public record Product(String name, double price) implements Comparable<Product> {

        @Override
            public int compareTo(Product o) {
                if (price == o.price) {
                    return fastCompare(name, o.name);
                }
                return (int) (price * 1000 - o.price * 1000);
            }
        }

    public static class SortedCities {
        public final City[] cities;
        public final List<City> list;
        private final Map<String, City> map;

        public SortedCities(City[] cities) {
            this.cities = cities;
            list = Stream.of(cities).toList();
            map = Stream.of(cities).collect(Collectors.toMap(it -> it.name, it -> it));
        }

        public City search(String cityName) {
            return map.get(cityName);
        }

        City getCheapestCity() {
            City cheapestCity = null;
            for (City city : cities) {
                if (cheapestCity == null || cheapestCity.sumPrice > city.sumPrice) {
                    cheapestCity = city;
                }
            }
            return cheapestCity;
        }
    }
}
