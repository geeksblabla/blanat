import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.net.URISyntaxException;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class Main {
    public static final String INPUT_TXT = "input.txt";
    public static final String OUTPUT_TXT = "output.txt";

    public static void main(String[] args) throws IOException, URISyntaxException {
        try (BufferedReader reader = new BufferedReader(new FileReader(INPUT_TXT))) {
            Map<String, Double> cityPrices = new ConcurrentHashMap<>();
            Map<String, Map<String, Double>> cityProducts = new ConcurrentHashMap<>();

            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(",");
                String city = parts[0];
                String product = parts[1];
                double price = Double.parseDouble(parts[2]);

                cityProducts.putIfAbsent(city, new ConcurrentHashMap<>());
                Map<String, Double> currentCity = cityProducts.get(city);
                currentCity.putIfAbsent(product, Double.MAX_VALUE); 
                currentCity.computeIfPresent(product, (p, currentPrice) -> Math.min(currentPrice, price));
                cityPrices.put(city, cityPrices.getOrDefault(city, 0.0) + price); 
            }
            writeResultToFile(cityPrices, cityProducts);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void writeResultToFile(Map<String, Double> cityPrices, Map<String,
            Map<String, Double>> cityProducts) {
        try (FileWriter writer = new FileWriter(OUTPUT_TXT)) {
            StringBuilder result = new StringBuilder();
            cityPrices.entrySet().stream()
                    .sorted(Map.Entry.comparingByValue())
                    .limit(1)
                    .forEach(city -> {
                                result.append(city.getKey())
                                        .append(" ")
                                        .append(Math.floor(city.getValue()))
                                        .append("\n");
                                cityProducts.get(city.getKey()).entrySet()
                                        .stream()
                                        .sorted(Map.Entry.comparingByValue())
                                        .forEach(entry -> {
                                            result.append(entry.getKey())
                                                    .append(" ")
                                                    .append(Math.floor(entry.getValue()))
                                                    .append("\n");
                                        });
                            }
                    );
            writer.write(result.toString());
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}
