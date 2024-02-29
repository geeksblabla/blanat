import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.stream.*;

 // by @mohsine1999  
 // I have used a Map to store the products of each city, and for each product, I have stored the cheapest price.
 // Then, I have used the stream API to find the city with the cheapest products and the total cost of the 5 cheapest products.
 // Finally, I have written the result to the output file.
 // The time complexity is O(n * m * log(m)), where n is the number of cities and m is the number of products in each city.

class Product implements Comparable<Product> {
    private final String name;
    private final double price;

    public Product(String name, double price) {
        this.name = name;
        this.price = price;
    }

    public String getName() {
        return name;
    }

    public double getPrice() {
        return price;
    }

    @Override
    public int compareTo(Product other) {
        int priceComparison = Double.compare(this.price, other.price);
        return (priceComparison != 0) ? priceComparison : this.name.compareTo(other.name);
    }
}

public class Main {
    private static final String INPUT_FILE_PATH = "input.txt";
    private static final String OUTPUT_FILE_PATH = "output.txt";

    public static void main(String[] args) throws IOException {
        long startTime = System.currentTimeMillis();

        Map<String, Map<String, Product>> cityProductPrices = new HashMap<>();
        try (BufferedReader reader = Files.newBufferedReader(Paths.get(INPUT_FILE_PATH))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(",");
                String city = parts[0].trim();
                String productName = parts[1].trim();
                double price = Double.parseDouble(parts[2].trim());

                cityProductPrices.computeIfAbsent(city, k -> new HashMap<>())
                        .merge(productName, new Product(productName, price),
                                (existing, newProduct) -> existing.getPrice() <= newProduct.getPrice() ? existing : newProduct);
            }
        }

        Optional<Map.Entry<String, Map<String, Product>>> cheapestCityEntry = cityProductPrices.entrySet().stream()
                .min(Comparator.comparingDouble(e -> e.getValue().values().stream().mapToDouble(Product::getPrice).sum()));

        if (cheapestCityEntry.isPresent()) {
            Map.Entry<String, Map<String, Product>> entry = cheapestCityEntry.get();
            List<Product> cheapestProducts = new ArrayList<>(entry.getValue().values());
            Collections.sort(cheapestProducts);

            double totalCost = cheapestProducts.stream().mapToDouble(Product::getPrice).sum();
            List<String> outputLines = new ArrayList<>();
            outputLines.add(entry.getKey() + " " + String.format("%.2f", totalCost));
            cheapestProducts.stream().limit(5).forEach(product ->
                    outputLines.add(product.getName() + " " + String.format("%.2f", product.getPrice())));

            Files.write(Paths.get(OUTPUT_FILE_PATH), outputLines);
        }

        long endTime = System.currentTimeMillis();
        long executionTime = endTime - startTime;
        System.out.println("Execution Time: " + executionTime + "ms");
    }
}
