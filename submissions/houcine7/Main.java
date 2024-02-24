import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.HashMap;
import java.util.Map;
import java.util.PriorityQueue;


public class Main {
    public class Price {
        double total;
        Double min ;

        Price(double total, int length) {
            this.total= total;
            this.min = Double.MAX_VALUE;
        }

         void increaseTotal(double price) {
            total+= price;
            this.min = Math.min(min, price);
        }

        public double getTotal() {
            return total;
        }
        public Double getMin() {
            return min;
        }
    }


    public static void main(String[] args) { 
        Map<String,Map<String,Price>> mp =new HashMap<>();
        try {
            BufferedReader bf = new BufferedReader(new FileReader("./input.txt"));
            bf.lines().forEach(line -> {
                String[] st =line.split(",");
                if(mp.containsKey(st[0])) {
                    Map<String,Price> cityProducts =  mp.get(st[0]);
                    if(cityProducts.containsKey(st[1])) {
                        cityProducts.get(st[1]).increaseTotal(Double.parseDouble(st[2]));
                    }else{
                        Price p = new Main().new Price(Double.parseDouble(st[2]), 1);
                        cityProducts.put(st[1], p);
                    }
                }else {
                    Map<String,Price> tmp = new HashMap<>();
                    Price p = new Main().new Price(Double.parseDouble(st[2]), 1);

                    tmp.put(st[1], p);
                    mp.put(st[0],  tmp );
                }
            });

            Map<String,Double> result = new HashMap<>();


            for (Map.Entry<String,Map<String,Price>> item:  mp.entrySet()) {
               for (Map.Entry<String,Price> pr : item.getValue().entrySet()) {
                    result.put(item.getKey(), 
                    (pr.getValue().getTotal()) +
                     result.getOrDefault(item.getKey(), 0.0)
                    );
               }
            }

            Double min = Double.MAX_VALUE;
            String city= "NONE";
            for(Map.Entry<String,Double> item : result.entrySet()) {
                if(min > item.getValue() ) {
                    min = item.getValue();
                    city = item.getKey();
                }
            }

            BufferedWriter bw = new BufferedWriter(new FileWriter("./output.txt"));

            bw.write(city+" "+min);

            Map<String,Price> cityProduct = mp.get(city);
            
            PriorityQueue<String> pq = new PriorityQueue<>((b,a)->{
                String[] splitedB = b.split(":");
                String[] splittedA = a.split(":");
                if(Double.parseDouble(splitedB[1])-Double.parseDouble(splittedA[1])>0.0) {
                    return 1;
                }else if(Double.parseDouble(splitedB[1])-Double.parseDouble(splittedA[1])<0.0){
                    return -1;
                }else{
                    return splitedB[0].compareToIgnoreCase(splittedA[0]);
                }
            });

            for (Map.Entry<String,Price> item :  cityProduct.entrySet()) {
                pq.add(item.getKey()+":"+item.getValue().getMin());
            }

            for(int i=0;i<5;i++) {
                String row = pq.poll();
                String temp= row.split(":")[0]+" "+row.split(":")[1];
                bw.append("\n" + temp);
            }

            bf.close();
            bw.close();
        } catch (Exception e) {
            System.out.println(e.getMessage());
            e.printStackTrace();
        }    }
}
