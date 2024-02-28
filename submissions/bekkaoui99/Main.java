import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Optional;


class City {
    private final String name;
    private final List<Main.Product> products;
    private double totalPrice;

    public City(String name, List<Main.Product> products, double totalPrice) {
        this.name = name;
        this.products = products;
        this.totalPrice = totalPrice;
    }

    public String name() {
        return name;
    }

    public List<Main.Product> products() {
        return products;
    }

    public double totalPrice() {
        return totalPrice;
    }

    public void setTotalPrice(double totalPrice) {
        this.totalPrice = totalPrice;
    }

}

public class Main {


    public record Product(String name , double price) {}


    public static void main(String[] args){

        List<City> cities = new ArrayList<>();
        final int MAX_PRODUCTS = 5;
        String dataSet = "input.txt";
        String output = "output.txt";


        // reading the dataset and collecting information
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
                    isExist.get().setTotalPrice( isExist.get().totalPrice() + getPrice);
                }
                else {
                    City city = new City(getCity , new ArrayList<>() , getPrice );
                    city.products().add(product);
                    cities.add(city);
                }

            }

        } catch (Exception e) {
            System.out.printf(e.getMessage());
        }


        // searching for the cheapest city in the country and sort the list of products
        City cheapestCity = cities.stream().min(Comparator.comparing(City::totalPrice)).get();
        cheapestCity.products().sort(Comparator.comparing(Product::price).thenComparing(Product::name));

        // write the result in the output file
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(output))) {
            writer.write(cheapestCity.name() + " " + String.format("%.2f" ,cheapestCity.totalPrice()) );
            for (int i = 0; i < MAX_PRODUCTS; i++) {
                writer.write("\n" + cheapestCity.products().get(i).name() + " "
                        + String.format("%.2f" , cheapestCity.products().get(i).price()));
            }
        } catch (Exception e) {
            System.err.println(e.getMessage());
        }


    }

}
