import java.io.*;
import java.util.*;

public class Main {
    public static void main(String[] args) {
        String inputFileName = "input.txt";
        String outputFileName = "output.txt";

        try {
            Map<String, Double> cityPrices = processInputFile(inputFileName);
            String cheapestCity = findCheapestCity(cityPrices);
            List<Map.Entry<String, Double>> cheapestProducts = findCheapestProducts(inputFileName, cheapestCity);
            writeOutputFile(cheapestCity, cityPrices.get(cheapestCity), cheapestProducts, outputFileName);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static Map<String, Double> processInputFile(String fileName) throws IOException {
        Map<String, Double> cityPrices = new HashMap<>();
        BufferedReader reader = new BufferedReader(new FileReader(fileName));
        String line;

        while ((line = reader.readLine()) != null) {
            String[] parts = line.split(",");
            String city = parts[0];
            double price = Double.parseDouble(parts[2]);

            cityPrices.put(city, cityPrices.getOrDefault(city, 0.0) + price);
        }

        reader.close();
        return cityPrices;
    }

    public static String findCheapestCity(Map<String, Double> cityPrices) {
        String cheapestCity = null;
        double minPrice = Double.MAX_VALUE;

        for (Map.Entry<String, Double> entry : cityPrices.entrySet()) {
            if (entry.getValue() < minPrice) {
                minPrice = entry.getValue();
                cheapestCity = entry.getKey();
            }
        }

        return cheapestCity;
    }

    public static List<Map.Entry<String, Double>> findCheapestProducts(String fileName, String city) throws IOException {
        Map<String, Double> productPrices = new HashMap<>();
        BufferedReader reader = new BufferedReader(new FileReader(fileName));
        String line;

        while ((line = reader.readLine()) != null) {
            String[] parts = line.split(",");
            String currentCity = parts[0];
            String product = parts[1];
            double price = Double.parseDouble(parts[2]);

            if (currentCity.equals(city)) {
                if (!productPrices.containsKey(product) || price < productPrices.get(product)) {
                    productPrices.put(product, price);
                }
            }
        }

        reader.close();

        List<Map.Entry<String, Double>> cheapestProducts = new ArrayList<>(productPrices.entrySet());
        cheapestProducts.sort(Map.Entry.comparingByValue());

        return cheapestProducts.subList(0, Math.min(5, cheapestProducts.size()));
    }

    public static void writeOutputFile(String cheapestCity, double totalPrice, List<Map.Entry<String, Double>> cheapestProducts, String fileName) throws IOException {
        BufferedWriter writer = new BufferedWriter(new FileWriter(fileName));
        writer.write(cheapestCity + " " + String.format("%.2f", totalPrice) + "\n");

        for (int i = 0; i < cheapestProducts.size(); i++) {
            Map.Entry<String, Double> entry = cheapestProducts.get(i);
            writer.write(entry.getKey() + " " + String.format("%.2f", entry.getValue()));
            if (i < cheapestProducts.size() - 1) {
                writer.write("\n");
            }
        }

        writer.close();
    }

}
