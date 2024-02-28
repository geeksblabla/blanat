import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

public class GeeksChallenge {

    private static boolean isLessThan(Map<String, Double> map, double target) {
        for (Map.Entry<String, Double> entry : map.entrySet()) {
            if (target < entry.getValue())
                return true;
        }
        return false;
    }

    private static String maxValue(Map<String, Double> map) {
        String expensiveCity = "";
        double maxVal = Double.MIN_VALUE;
        for (Map.Entry<String, Double> entry : map.entrySet()) {
            if (entry.getValue() > maxVal) {
                maxVal = entry.getValue();
                expensiveCity = entry.getKey();
            }
        }
        return expensiveCity;
    }

    private static void saveDataOutput(String inputFile, String outputFile) throws IOException {
        Map<String, Double> cheapestCities = new HashMap<>();
        Map<String, Double> cheapestLegumes = new HashMap<>();

        try (BufferedReader br = new BufferedReader(new FileReader(inputFile));
                BufferedWriter bw = new BufferedWriter(new FileWriter(outputFile))) {

            String line;
            while ((line = br.readLine()) != null) {
                String[] data = line.replaceAll("\\s", "").split(",");
                String city = data[0];
                String product = data[1];
                double price = Double.parseDouble(data[2]);

                cheapestCities.put(city, cheapestCities.getOrDefault(city, 0.0) + price);

                if (cheapestLegumes.size() < 5 || isLessThan(cheapestLegumes, price)) {
                    if (!cheapestLegumes.containsKey(product) || cheapestLegumes.get(product) > price) {
                        cheapestLegumes.put(product, price);

                        if (cheapestLegumes.size() > 5) {
                            String expensiveProduct = maxValue(cheapestLegumes);
                            cheapestLegumes.remove(expensiveProduct);
                        }
                    }
                }
            }

            // Find cheapest city
            String cheapestCity = "";
            double minPrice = Double.MAX_VALUE;
            for (Map.Entry<String, Double> entry : cheapestCities.entrySet()) {
                if (entry.getValue() < minPrice) {
                    minPrice = entry.getValue();
                    cheapestCity = entry.getKey();
                }
            }

            bw.write(cheapestCity + " " + String.format("%.2f", minPrice));
            bw.newLine();

            // Sort and write cheapest legumes
            cheapestLegumes.entrySet().stream()
                    .sorted(Map.Entry.comparingByValue())
                    .forEachOrdered(entry -> {
                        try {
                            bw.write(entry.getKey() + " " + String.format("%.2f", entry.getValue()));
                            bw.newLine();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    });
        }
    }

    public static void main(String[] args) throws IOException {
        String inputFile = "input.txt";
        String outputFile = "./output.txt";

        saveDataOutput(inputFile, outputFile);
    }
}
