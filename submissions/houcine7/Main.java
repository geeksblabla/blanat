import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.concurrent.ConcurrentHashMap;

import java.util.List;
import java.util.ArrayList;


public class Main {
    public class Price {
        double total;
        double min ;

         Price(double total) {
            this.total= total;
            this.min =total;
        }

        synchronized double increaseTotal(double price) {
            total+= price;
            this.min = Math.min(price, this.min);
            return this.total;
        }

        public double getTotal() {
            return total;
        }
        public double getMin() {
            return min;
        }
    }

    class ProductEntry implements Comparable<ProductEntry> {
        String name;
        double price;

        public ProductEntry(String name, double price) {
            this.name = name;
            this.price = price;
        }

        @Override
        public int compareTo(ProductEntry other) {
            int priceComparison = Double.compare(other.price, price);
            if (priceComparison != 0) {
                return priceComparison;
            }
            return other.name.compareToIgnoreCase(name);
        }
    }


    public static void main(String[] args) { 

        Map<String,Map<String,Price>> mp =new ConcurrentHashMap<>();
        Map<String,Double> result = new ConcurrentHashMap<>();

        try {

            BufferedReader bf = new BufferedReader(new FileReader("./input.txt"));

            bf.lines().parallel().forEach(line -> {
                String[] st =line.split(",");
                double valueToAdd = Double.parseDouble(st[2]);
                result.computeIfPresent(st[0], (k, v) -> v + valueToAdd);
                result.putIfAbsent(st[0], valueToAdd);
                mp.getOrDefault(st[0], new ConcurrentHashMap<>())
                    .computeIfPresent(st[1], (k, price) -> {
                    price.increaseTotal(valueToAdd);
                    return price;
                });
                mp.computeIfAbsent(st[0], k -> new ConcurrentHashMap<>())
                .putIfAbsent(st[1], new Main().new Price(valueToAdd));
            });            

            Double min = Double.MAX_VALUE;
            String city = "";
            for(Map.Entry<String,Double> item : result.entrySet()) {
                if(min > item.getValue() ) {
                    min = item.getValue();
                    city = item.getKey();
                }
            }

            BufferedWriter bw = new BufferedWriter(new FileWriter("./output.txt"));

            bw.write(city+" "+String.format("%.2f",min));

            Map<String,Price> cityProducts = mp.get(city);
            
            PriorityQueue<ProductEntry> pq = new PriorityQueue<>(5);
            for (Map.Entry<String,Price> item :  cityProducts.entrySet()) {
                ProductEntry pe = new Main().new ProductEntry(item.getKey(), item.getValue().getMin());
                pq.add(pe);
                if(pq.size()>5) pq.poll();
            }
            List<ProductEntry> entries = new ArrayList<>();
            while (!pq.isEmpty()) {
                entries.add(pq.poll()); 
            }
            for (int i=4;i>=0;i--) {
                ProductEntry entry = entries.get(i);
                bw.append("\n").append(entry.name).append(" ").append(String.format("%.2f", entry.price));
            }

            bf.close();
            bw.close();

        } catch (Exception e) {
            System.out.println(e.getMessage());
            e.printStackTrace();
        }    }
}
