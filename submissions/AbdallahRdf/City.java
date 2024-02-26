import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.PriorityQueue;

class City implements Comparable<Object>{
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
    public int compareTo(Object other){
        return Double.compare(((City)other).getTotalPrice(), this.totalPrice);
    }
}