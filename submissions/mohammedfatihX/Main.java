//use java 17

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.DecimalFormat;
import java.util.*;
import java.util.concurrent.atomic.AtomicLong;
import java.util.function.Function;
import java.util.stream.Collector;
import java.util.stream.Collectors;
import java.util.stream.Stream;

 public class Main {

  public static String INPUT_FILE_PATH = "input.txt";
  public static String OUTPUT_FILE_PATH ="output.txt"; 
    
     public static void main(String[] args) throws IOException  {

      
        processing();


     }
    
    
     public static void processing() throws IOException {
             List <String> output = Files.lines(Paths.get(INPUT_FILE_PATH)).parallel().map(l -> l.split(","))
                     .collect(Collectors.teeing(
                             Collectors.groupingByConcurrent(p -> p[0].trim(), Collectors.summingDouble(p -> Double.parseDouble(p[2].trim())))
                             ,filteringDuplicatedByProductAndPrice (5),
                             (sumByCity, cleanProductByPrice) -> {
                                  Map.Entry<String, Double> rex = sumByCity.entrySet().stream().parallel().min(Map.Entry.comparingByValue()).get();
                                 List<String> result = new ArrayList<>();
                                 String formating = rex.getKey()+" "+ new DecimalFormat("#.00").format(rex.getValue())+"\n";
                                 result.add(formating);
                                 result.addAll(cleanProductByPrice.stream().parallel()
                                         .map(t -> new Product(t[1].trim(),Double.parseDouble(t[2].trim())))
                                        .sorted(Comparator.comparing(Product::getValue).thenComparing(Product::getKey)).map(productResult ->
                                                 productResult.getKey() + " " + String.format( "%.2f", productResult.getValue()) + "\n").toList());

                                 return result;
                             }
                     ));


                 writeOutput(OUTPUT_FILE_PATH,output);

         }


     public static Collector<String[], ?, List<String[]>> filteringDuplicatedByProductAndPrice(int n) {
             return Collector.of(
                     ArrayList::new,
                     (list, array) -> {
                         String key = array[1].trim();
                         double valueToCompare = Double.parseDouble(array[2].trim());

                         boolean keyExists = list.contains(key);

                         if (!keyExists) {
                             // If the list size is less than n, or the new value is less than the last element, add to the list
                             if (list.size() < n || valueToCompare < Double.parseDouble(list.get(n - 1)[2])) {
                                 list.add(array);
                                 list.sort(Comparator.comparing(temp ->Double.parseDouble(temp[2])));
                             }
                         } else {
                             int index = list.indexOf(array);
                             if (valueToCompare < Double.parseDouble(list.get(index)[2])) {
                                 list.remove(index);
                                 list.add(array);
                             }
                             list.sort(Comparator.comparing(temp ->Double.parseDouble(temp[2])));
                         }


                     },
                     (list1, list2) -> {
                         List<String[]> combinedList = new ArrayList<>(list1);
                         combinedList.addAll(list2);
                         combinedList = new ArrayList<>(combinedList.stream().parallel()
                                 .collect(Collectors.toConcurrentMap(temp -> temp[1], Function.identity(),
                                         (existing, replacement) ->
                                                 Double.parseDouble(existing[2]) <  Double.parseDouble(replacement[2]) ? existing : replacement))
                                 .values());

                         combinedList.sort(Comparator.comparingDouble(temp -> Double.parseDouble(temp[2])));
                         return combinedList.size() > n ? combinedList.subList(0, n) : combinedList;
                     },
                     Collector.Characteristics.UNORDERED
             );
         }


    public static void writeOutput(String filepath,List<String> result){
            try {
                 Files.write(Paths.get(filepath),
                             result.stream().collect(Collectors.joining()).getBytes());
           } catch (IOException e) {
              throw new RuntimeException(e);
          }
      }

  static class Product {
         public String key;
         public double value;

         public Product(String key, double value) {
             this.key = key;
             this.value = value;
         }


         public double getValue() {
             return value;
         }

         public String getKey(){
             return key;
         }

         @Override
         public String toString() {
             return key + " " + value;
         }

         @Override
         public boolean equals(Object obj) {
             // Check if the other object is an instance of KeyValueObject
             if (obj instanceof Product) {
                 // Compare objects based on their keys
                 Product other = (Product) obj;
                 return this.key.equals(other.key);
             }
             return false;
         }
    }
  }
