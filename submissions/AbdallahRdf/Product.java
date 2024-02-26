public class Product implements Comparable<Object> {
    private String name;
    private double price;

    public Product(String name, double price){
        this.name = name;
        this.price = price;
    }

    public double getPrice(){
        return this.price;
    }

    public String getName() {
        return this.name;
    }

    @Override
    public int compareTo(Object other) {
        int priceComparaison = Double.compare(((Product) other).getPrice(), this.price);
        return (priceComparaison == 0) ? ((Product)other).getName().compareTo(this.name) : priceComparaison;
    }
}
