import java.io.IOException;
import java.io.File;
import java.io.FileWriter;
import java.io.BufferedWriter;

import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import java.util.HashSet;
import java.util.stream.Collectors;


public class Main {
    public static void main(String[] args) throws IOException {

        File inputFile = new File("input.txt");
        FileWriter outputFile = new FileWriter("output.txt");
        BufferedWriter writer = new BufferedWriter(outputFile);
        Scanner reader = new Scanner(inputFile);

        Map<String, Double> cities = new HashMap<>();
        List<Product> cheapestCityProducts = new ArrayList<>();
        Set<String> top5Products = new HashSet<>(5);


        // construct the map : 
        while(reader.hasNextLine()) {
            String[] details = reader.nextLine().split(",");

            String city = details[0];
            double price = Double.valueOf(details[2]);

            cities.put(city, cities.getOrDefault(city, 0.0) + price);            
        }
        reader.close();

        // Get the cheapest city (°_~): 
        Double minPrice = Double.MAX_VALUE;
        String cheapestCity = null;

        for(Map.Entry<String, Double> entry : cities.entrySet()) {
            String c = entry.getKey();
            double p = entry.getValue();
            if(p < minPrice) {
                minPrice = p;
                cheapestCity = c;
            }
        }

        // Construct the output : 
        reader = new Scanner(inputFile);

        while(reader.hasNextLine()) {
            String line = reader.nextLine();
            String[] details = line.split(",");
            if(details[0].equals(cheapestCity)) {
                String productName = details[1];
                double productPrice = Double.valueOf(String.format("%.2f", Double.valueOf(details[2])));
                Product prod = new Product(productName, productPrice);
                cheapestCityProducts.add(prod);
            }
        }

        // sort the list according to price or by name if same price : 
        List<Product> sortedProducts = cheapestCityProducts.parallelStream().sorted(
            (p1, p2) -> {
                if(p1.getPrice() == p2.getPrice()) {
                    // sort by name : 
                    return p1.getName().compareTo(p2.getName());
                } else {
                    return Double.compare(p1.getPrice(), p2.getPrice());
                }
            }
        ).collect(Collectors.toList());

        // writing : 
        writer.write(cheapestCity + " " + String.format("%.2f", minPrice));
        writer.newLine();

        // handle repetitive names of top 5 products : 
        int count = 5;        
        for(Product p : sortedProducts) {
            if(top5Products.contains(p.getName())) {
                continue;
            } else {
                top5Products.add(p.getName());
                writer.write(p.getName() + " " + p.getPrice());
                if(count > 1) writer.newLine();
                count--;
            }
            
            if(count == 0) break;
        }


        writer.flush();
        writer.close();
    }
}

class Product {
    private String name;
    private double price;

    public Product() {}

    public Product(String name, double price) {
        this.name = name;
        this.price = price;
    }

    public String getName() {
        return this.name;
    }

    public double getPrice() {
        return this.price;
    }
}

