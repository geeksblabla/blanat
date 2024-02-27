import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.concurrent.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {

    public static final String INPUT_TXT = "input.txt";
    public static final String OUTPUT_TXT = "./output.txt";

    private static class Product {
        String product;
        double price;

        public Product(String product, double price) {
            this.product = product;
            this.price = price;
        }

        public String getProduct() {
            return product;
        }

        public double getPrice() {
            return price;
        }
    }

    private static class City {
        double total;
        Product[] products;

        public City(double total, Product[] products) {
            this.total = total;
            this.products = products;
        }

        public static City merge(City c1, City c2) {
            Set<Product> set = new TreeSet<>(Comparator.comparingDouble(Product::getPrice).thenComparing(Product::getProduct));
            set.addAll(Arrays.asList(c1.products));
            set.addAll(Arrays.asList(c2.products));

            return new City(
                    c1.total + c2.total,
                    set.stream().limit(5).toArray(Product[]::new)
            );
        }
    }

    public static void main(String[] args) throws IOException {
        ExecutorService executorService = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());
        try (Stream<String> lines = Files.lines(Paths.get(INPUT_TXT), StandardCharsets.ISO_8859_1)) {
            List<Callable<Map.Entry<String, City>>> tasks = lines.map(line -> (Callable<Map.Entry<String, City>>) () -> {
                String[] content = line.split(",");
                Product product = new Product(content[1], Double.parseDouble(content[2]));
                return new AbstractMap.SimpleEntry<>(content[0], new City(product.getPrice(), new Product[]{product}));
            }).collect(Collectors.toList());

            List<Future<Map.Entry<String, City>>> futures = executorService.invokeAll(tasks);

            Map<String, City> resultMap = new ConcurrentHashMap<>();
            for (Future<Map.Entry<String, City>> future : futures) {
                Map.Entry<String, City> entry = future.get();
                resultMap.merge(entry.getKey(), entry.getValue(), City::merge);
            }

            Optional<String> result = resultMap.entrySet().stream()
                    .sorted(Map.Entry.comparingByValue(Comparator.comparingDouble(cityRecord -> cityRecord.total)))
                    .findFirst()
                    .map(entry -> {
                        StringBuilder content = new StringBuilder(entry.getKey() + " " + String.format("%.2f", entry.getValue().total) + "\n");
                        for (Product product : entry.getValue().products) {
                            content.append(product.product).append(" ").append(String.format("%.2f", product.price)).append("\n");
                        }
                        return content.toString();
                    });

            // Flush to output file
            Files.write(Paths.get(OUTPUT_TXT), result.get().getBytes());
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        } finally {
            executorService.shutdown();
        }
    }
}