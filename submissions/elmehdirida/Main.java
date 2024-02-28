
import java.io.*;
import java.util.*;


public class Main {
    public static void main(String[] args) throws IOException {        
        BufferedReader reader = new BufferedReader(new FileReader("input.txt"));
        Map<String, Map<String, Double>> cities = new HashMap<>();
        String line;
        while ((line = reader.readLine()) != null) {
            String[] parts = line.split(",");
            String city = parts[0];
            String product = parts[1];
            double price = Double.parseDouble(parts[2]);
            cities.putIfAbsent(city, new HashMap<>());
            cities.get(city).put(product, Math.min(cities.get(city).getOrDefault(product, Double.MAX_VALUE), price));
        }
        reader.close();
        String cheapestCity = null;
        double cheapestPrice = Double.MAX_VALUE;
        for (Map.Entry<String, Map<String, Double>> entry : cities.entrySet()) {
            double price = entry.getValue().values().stream().mapToDouble(Double::doubleValue).sum();
            if (price < cheapestPrice) {
                cheapestPrice = price;
                cheapestCity = entry.getKey();
            }
        }
        List<String> cheapestProducts = new ArrayList<>(cities.get(cheapestCity).keySet());
        cheapestProducts.sort(Comparator.comparingDouble(cities.get(cheapestCity)::get)
        .thenComparing(Comparator.comparing(String::valueOf)));
                BufferedWriter writer = new BufferedWriter(new FileWriter("output.txt"));
        writer.write(cheapestCity + " " + String.format("%.2f", cheapestPrice) + "\n");
        for (int i = 0; i < Math.min(5, cheapestProducts.size()); i++) {
            writer.write(cheapestProducts.get(i) + " " + String.format("%.2f", cities.get(cheapestCity).get(cheapestProducts.get(i))) + "\n");
        }
        writer.close();
    }
}
