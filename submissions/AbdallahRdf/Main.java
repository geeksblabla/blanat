import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

public class Main {
    public static void main(String[] args) throws FileNotFoundException, IOException {
        try {
            File input = new File("input.txt");
            BufferedReader reader = new BufferedReader(new FileReader(input));

            HashMap<String, City> cities = new HashMap<>();

            String line;
            String cityName = null;
            while ((line = reader.readLine()) != null) {
                String[] array = line.split(",");
                cityName = array[0];
                String productName = array[1];
                double productPrice = Double.parseDouble(array[2]);

                cities.computeIfAbsent(cityName, key -> {
                    return new City(key);
                }).addProduct(new Product(productName, productPrice));
            }
            reader.close();

            City cheapesCity = cities.get(cityName);
            double cheapestPrice = cheapesCity.getTotalPrice();
            for (Map.Entry<String, City> c : cities.entrySet()) {
                City current = c.getValue();
                double currentPrice = current.getTotalPrice();
                if (cheapestPrice > currentPrice) {
                    cheapesCity = current;
                    cheapestPrice = currentPrice;
                }
            }

            File output = new File("output.txt");
            output.createNewFile();
            BufferedWriter writer = new BufferedWriter(new FileWriter(output));

            String info = cheapesCity.info();
            writer.write(info, 0, info.length());
            writer.close();
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}
