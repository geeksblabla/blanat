import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Collectors;
import java.util.stream.Stream;

record Product(String name, double price) {
}

record City(String name, Product[] cheapestProducts, double total) {
    public City addProduct(Product product) {
        var cheapestProducts = Stream
                .concat(Arrays.stream(this.cheapestProducts), Arrays.stream(new Product[] { product }))
                .sorted(Comparator.comparingDouble(Product::price).thenComparing(Product::name))
                .distinct()
                .limit(5)
                .toArray(Product[]::new);

        var total = this.total + product.price();
        return new City(this.name, cheapestProducts, total);
    }

    public City merge(City c) {
        return new City(this.name, Stream
                .concat(Arrays.stream(this.cheapestProducts), Arrays.stream(c.cheapestProducts))
                .sorted(Comparator.comparingDouble(Product::price).thenComparing(Product::name))
                .distinct()
                .limit(5)
                .toArray(Product[]::new), this.total + c.total);
    }
}

public class Main {

    public static final String INPUT_TXT = "input.txt";
    public static final String OUTPUT_TXT = "./output.txt";

    private static final int PARSER_QUEUE_SIZE = 4000;
    private static final int REDUCER_QUEUE_SIZE = 60;
    private static final int THREAD_POOL_SIZE = 20;
    private static final int LINES_BACH_SIZE = 5000;

    private static final BlockingQueue<String[]> rowLinesChunksQueue = new ArrayBlockingQueue<>(PARSER_QUEUE_SIZE);
    private static final BlockingQueue<Map<String, City>> citiesQueue = new ArrayBlockingQueue<>(REDUCER_QUEUE_SIZE);
    private static final ExecutorService executorService = Executors.newFixedThreadPool(THREAD_POOL_SIZE);
    private static final AtomicBoolean finish_Reading = new AtomicBoolean(false);
    static long startTime;
    private static final AtomicInteger inProgressParsers = new AtomicInteger(0);

    public static void main(String[] args) throws IOException {

        executorService.submit(Main::fileReader);

        for (int i = 0; i < THREAD_POOL_SIZE - 3; i++) {
            executorService.submit(Main::parserConsumer);
        }

        var output = reducerConsumer().values().stream().min(Comparator.comparingDouble(City::total)).map((city) -> {
            Product[] ps = city.cheapestProducts();
            StringBuilder res = new StringBuilder(city.name() + " " + String.format("%.2f", city.total()) + "\n");
            for (Product p : ps) {
                res.append(p.name()).append(" ").append(String.format("%.2f", p.price())).append("\n");
            }
            return res.toString();
        });

        Files.write(Paths.get(OUTPUT_TXT), output.get().getBytes());

        executorService.shutdownNow();

    }

    private static void fileReader() {
        try (Stream<String> lines = Files.lines(Paths.get(INPUT_TXT))) {
            List<String> linesBach = new ArrayList<>();
            lines.forEach((String line) -> {
                linesBach.add(line);
                if (linesBach.size() == LINES_BACH_SIZE) {
                    addToRowLinesChunksQueue(linesBach);
                    linesBach.clear();
                }
            });
            if (!linesBach.isEmpty())
                addToRowLinesChunksQueue(linesBach);
            finish_Reading.set(true);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private static Map<String, City> parseChunk(String[] linesChunk) {
        Map<String, City> cities = new HashMap<>();
        Arrays.stream(linesChunk).forEach((line) -> {
            String[] parsedLine = line.split(",");
            if (parsedLine.length == 3) {
                Product product = new Product(parsedLine[1], Double.parseDouble(parsedLine[2]));
                String cityName = parsedLine[0];
                cities.compute(cityName,
                        (key, value) -> {
                            if (value == null)
                                return new City(cityName, new Product[] { product }, product.price());
                            return value.addProduct(product);
                        });
            }
        });
        return cities;
    }

    private static Map<String, City> reduce(Map<String, City> cities1, Map<String, City> cities2) {
        cities1.keySet().forEach((name) -> {
            cities2.compute(name,
                    (key, value) -> (value == null) ? cities1.get(name) : value.merge(cities1.get(name)));
        });
        return cities2;
    }

    private static void parserConsumer() {
        try {
            while ((finish_Reading.get() && !rowLinesChunksQueue.isEmpty()) || (!finish_Reading.get())) {
                String[] linesChunk = rowLinesChunksQueue.take();
                inProgressParsers.incrementAndGet();
                var cities = parseChunk(linesChunk);
                citiesQueue.put(cities);
                inProgressParsers.decrementAndGet();

            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    private static Map<String, City> reducerConsumer() {
        try {

            Map<String, City> res = new HashMap<>();

            while (!rowLinesChunksQueue.isEmpty() || !citiesQueue.isEmpty() || !finish_Reading.get()
                    || inProgressParsers.get() != 0) {

                Map<String, City> cities1 = citiesQueue.take();

                res = reduce(cities1, res);

                if ((rowLinesChunksQueue.isEmpty() && citiesQueue.isEmpty()) && finish_Reading.get()
                        && inProgressParsers.get() == 0) {
                    break;
                }
            }
            return res;
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    static void addToRowLinesChunksQueue(List<String> linesBach) {
        try {
            rowLinesChunksQueue.put(linesBach.toArray(new String[0]));
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }
}
