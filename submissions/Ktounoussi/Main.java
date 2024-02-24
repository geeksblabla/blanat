import java.nio.file.*;
import java.io.*;
import java.util.*;
import java.util.stream.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.function.Function;

public class Main {

    static class Product implements Comparable<Product> {
        String name;
        double price;

        Product(String name, double price) {
            this.name = name;
            this.price = price;
        }

        @Override
        public int compareTo(Product other) {
            int priceComp = Double.compare(this.price, other.price);
            if (priceComp != 0) return priceComp;
            return this.name.compareTo(other.name);
        }
    }

    static class CityInfo {
        double totalPrice = 0.0;
        ConcurrentHashMap<String, Product> minPriceProducts = new ConcurrentHashMap<>();

         void updateProduct(String productName, double price) {
            this.totalPrice += price;
            this.minPriceProducts.compute(productName, (k, v) -> v == null || price < v.price ? new Product(productName, price) : v);
        }
    }

    public static String[] splitStringByDelimiter(String input, char delimiter) {
        List<String> result = new ArrayList<>();
        int start = 0;
        for (int i = 0; i < input.length(); i++) {
            if (input.charAt(i) == delimiter) {
                result.add(input.substring(start, i));
                start = i + 1;
            }
        }
        // Add the last segment
        result.add(input.substring(start));

        return result.toArray(new String[0]);
    }

    public static void main(String[] args) {
        String inputFilename = "input.txt"; // Input file path
        String outputFilename = "output.txt"; // Output file path correction

        try {
            Map<String, Double> cityPrices = new ConcurrentHashMap<>();
            Map<String, CityInfo> cityInfos = new ConcurrentHashMap<>();

            Files.lines(Paths.get(inputFilename)).parallel().forEach(line -> {
                String[] parts = splitStringByDelimiter(line, ',');
                String city = parts[0];
                String product = parts[1];
                double price = Double.parseDouble(parts[2]);

                cityInfos.computeIfAbsent(city, k -> new CityInfo()).updateProduct(product, price);
            });

            String cheapestCity = cityInfos.entrySet().stream()
                .min(Map.Entry.comparingByValue(Comparator.comparingDouble(ci -> ci.totalPrice))) // Corrected lambda expression
                .map(Map.Entry::getKey)
                .orElse(null);

            CityInfo cheapestCityInfo = cityInfos.get(cheapestCity);

            List<Product> cheapestProducts = new ArrayList<>(cheapestCityInfo.minPriceProducts.values());
            Collections.sort(cheapestProducts);

            try (BufferedWriter out = Files.newBufferedWriter(Paths.get(outputFilename))) {
                out.write(String.format("%s %.2f%n", cheapestCity, cheapestCityInfo.totalPrice));
                for (int i = 0; i < Math.min(cheapestProducts.size(), 5); i++) {
                    Product p = cheapestProducts.get(i);
                            if (i == 4) {
                                out.write(String.format("%s %.2f", p.name, p.price));
                            } else {
                                out.write(String.format("%s %.2f%n", p.name, p.price));
                            }                
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
