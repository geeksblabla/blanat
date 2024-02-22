import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.*;

public class Main {

    public record City(String name , List<Product> products) {}
    public record Product(String name , double price) {}

    public static void main(String[] args){

        long startTime;
        long endTime;
        startTime = System.nanoTime();

        List<City> cities = new ArrayList<>();
        String dataSet = "input.txt";
        String output = "output.txt";

        // read data
        try (BufferedReader reader = new BufferedReader(new FileReader(dataSet))){
            String line;
            while ((line = reader.readLine()) != null){

                String[] city_product_price = line.split(",");
                String getCity = city_product_price[0];
                String getProduct = city_product_price[1];
                double getPrice = Double.parseDouble(city_product_price[2]);

                Product product = new Product(getProduct , getPrice);
                Optional<City> isExist = cities.stream()
                        .filter(city -> city.name().equals(getCity))
                        .findFirst();

                if(isExist.isPresent()){
                    isExist.get().products().add(product);
                }
                else {
                    City city = new City(getCity , new ArrayList<>());
                    city.products().add(product);
                    cities.add(city);
                }

            }

        } catch (Exception e) {
            System.out.printf(e.getMessage());
        }



        // calculate the total price of each city :
        Map<String , Double> totalPrice = new HashMap<>();

        cities.forEach(city -> {
            totalPrice.put(city.name() ,
                    city.products().stream()
                            .map(Product::price)
                            .reduce(Double::sum).get());

        });



        // find the lowest city in the country :
        Map.Entry<String, Double> cityWithLowestPrice = findCityWithLowestPrice(totalPrice);

        // get the cheapest city from the list
        City getTheCheapestCity = cities.stream()
                .filter(city -> city.name().equals(cityWithLowestPrice.getKey()))
                .findFirst().get();
        // sort the products based on the price
        getTheCheapestCity.products().sort(Comparator.comparingDouble(Product::price));


        // write the result in the output file
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(output))) {
            writer.write(cityWithLowestPrice.getKey() + " " + cityWithLowestPrice.getValue());
            for (int i = 0; i < 5; i++) {
                writer.write("\n" + getTheCheapestCity.products().get(i).name() + " "
                        + String.format("%.2f" , getTheCheapestCity.products().get(i).price()));
            }
        } catch (Exception e) {
            System.err.println(e.getMessage());
        }

        // calculate the time of execution
        endTime = System.nanoTime();
        long duration = (endTime - startTime)/ 1_000_000 ;
        System.out.println(duration + "ms");


    }

    public static Map.Entry<String, Double> findCityWithLowestPrice(Map<String, Double> cityPrices) {
        Map.Entry<String, Double> lowestPriceEntry = null;
        double lowestPrice = Double.MAX_VALUE;

        for (Map.Entry<String, Double> entry : cityPrices.entrySet()) {
            if (entry.getValue() < lowestPrice) {
                lowestPrice = entry.getValue();
                lowestPriceEntry = entry;
            }
        }

        return lowestPriceEntry;
    }


}
