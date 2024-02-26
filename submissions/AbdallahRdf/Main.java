import java.io.*;
import java.util.*;

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

class City implements Comparable<Object> {
    private String name;
    private double totalPrice = 0;
    private static final int MAX_SIZE = 5;
    private HashMap<String, Product> cheapestProducts = new HashMap<>(5);
    private PriorityQueue<Product> cheapestProductsQueue = new PriorityQueue<>();

    public City(String name) {
        this.name = name;
    }

    public void addProduct(Product product) {
        String productName = product.getName();
        double productPrice = product.getPrice();
        Product mostExpensiveProduct;
        if (this.cheapestProducts.containsKey(productName)) {
            Product productToRemoveFromQueue = this.cheapestProducts.get(productName);
            if (productPrice < productToRemoveFromQueue.getPrice()) {
                this.cheapestProducts.put(productName, product);
                this.cheapestProductsQueue.remove(productToRemoveFromQueue);
                this.cheapestProductsQueue.add(product);
            }
        } else if (this.cheapestProducts.size() < City.MAX_SIZE) {
            this.cheapestProducts.put(productName, product);
            this.cheapestProductsQueue.add(product);
        } else if (productPrice < (mostExpensiveProduct = this.cheapestProductsQueue.peek()).getPrice()) {
            this.cheapestProducts.remove(mostExpensiveProduct.getName());
            this.cheapestProducts.put(productName, product);
            this.cheapestProductsQueue.poll();
            this.cheapestProductsQueue.add(product);
        }
        this.totalPrice += productPrice;
    }

    public String getName() {
        return this.name;
    }

    public double getTotalPrice() {
        return this.totalPrice;
    }

    public String info() {
        ArrayList<Product> cheapestFiveProducts = new ArrayList<>();
        for (Map.Entry<String, Product> element : this.cheapestProducts.entrySet()) {
            cheapestFiveProducts.add(element.getValue());
        }
        Collections.sort(cheapestFiveProducts);
        StringBuilder str = new StringBuilder();
        str.append(this.name);
        str.append(" ");
        str.append(String.format("%.2f", this.totalPrice));
        str.append("\n");

        for (int i = cheapestFiveProducts.size() - 1; i >= 0; i--) {
            str.append(cheapestFiveProducts.get(i).getName());
            str.append(" ");
            str.append(String.format("%.2f", cheapestFiveProducts.get(i).getPrice()));
            str.append("\n");
        }
        return str.toString().trim();
    }

    @Override
    public int compareTo(Object other) {
        return Double.compare(((City) other).getTotalPrice(), this.totalPrice);
    }
}

class Product implements Comparable<Object> {
    private String name;
    private double price;

    public Product(String name, double price) {
        this.name = name;
        this.price = price;
    }

    public double getPrice() {
        return this.price;
    }

    public String getName() {
        return this.name;
    }

    @Override
    public int compareTo(Object other) {
        int priceComparaison = Double.compare(((Product) other).getPrice(), this.price);
        return (priceComparaison == 0) ? ((Product) other).getName().compareTo(this.name) : priceComparaison;
    }
}
