import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Collectors;

public class Main {

    public static void main(String[] args) {

        Map<String, Double> map = new HashMap<>();

        try (BufferedReader reader = new BufferedReader(new FileReader("input.txt"))) {
            Map.Entry<String, Double> minEntry = reader.lines()
                    .parallel()
                    .map(line -> line.split(","))
                    .collect(Collectors.groupingBy(parts -> parts[0],
                            Collectors.summingDouble(parts -> Double.parseDouble(parts[2]))))
                    .entrySet().stream()
                    .min(Map.Entry.comparingByValue())
                    .orElse(null);

            if (minEntry != null) {
                try {
                    Files.writeString(Paths.get("output.txt"), String.format("%s %.2f%n", minEntry.getKey(), minEntry.getValue()));

                    Map<String, Double> cheapestCityProducts = Files.lines(Paths.get("input.txt"))
                            .parallel()
                            .filter(line -> line.startsWith(minEntry.getKey() + ","))
                            .map(line -> line.split(","))
                            .collect(Collectors.toMap(
                                    parts -> parts[1],
                                    parts -> Double.parseDouble(parts[2]),
                                    Double::min
                            ));

                    String output = cheapestCityProducts.entrySet().parallelStream()
                            .sorted((e1, e2) -> {
                                int priceComparison = Double.compare(e1.getValue(), e2.getValue());
                                if (priceComparison == 0) {
                                    return e1.getKey().compareTo(e2.getKey());
                                }
                                return priceComparison;
                            })
                            .limit(5)
                            .map(entry -> String.format("%s %.2f%n", entry.getKey(), entry.getValue()))
                            .collect(Collectors.joining());

                    Files.writeString(Paths.get("output.txt"), output, java.nio.file.StandardOpenOption.APPEND);

                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}


