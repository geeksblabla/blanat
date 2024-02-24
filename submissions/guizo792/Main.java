import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {

    public static void main(String[] args) {

        Map<String, Double> map = new HashMap<>();

        try (Stream<String> lines = Files.lines(Paths.get("input.txt"))) {
            List<String> linesList = lines.collect(Collectors.toList());

            linesList.forEach((line) -> {
                String[] splitted = line.split(",");
                String city = splitted[0];
                double price = Double.parseDouble(splitted[2]);
                map.put(city, map.getOrDefault(city, 0.0) + price);
            });

            Map.Entry<String, Double> minEntry = map.entrySet().stream().min(Map.Entry.comparingByValue()).orElse(null);

            if (minEntry != null) {
                Map<String, Double> cheapestCityProducts = linesList.stream()
                        .filter(line -> line.startsWith(minEntry.getKey() + ","))
                        .map(line -> line.split(","))
                        .collect(Collectors.toMap(
                                parts -> parts[1],
                                parts -> Double.parseDouble(parts[2]),
                                Double::min
                        ));

                String output = String.format("%s %.2f%n", minEntry.getKey(), minEntry.getValue());
                output += cheapestCityProducts.entrySet().stream()
                        .sorted((e1, e2) -> {
                            int priceComparison = Double.compare(e1.getValue(), e2.getValue());
                            if (priceComparison == 0) {
                                // If prices are equal, compare by product name
                                return e1.getKey().compareTo(e2.getKey());
                            }
                            return priceComparison;
                        })
                        .limit(5) // Limit to top 5 products
                        .map(entry -> String.format("%s %.2f%n", entry.getKey(), entry.getValue()))
                        .collect(Collectors.joining());
                
			 Files.writeString(Paths.get("output.txt"), output);

            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

