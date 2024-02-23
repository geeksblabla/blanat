//use java 17

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.DecimalFormat;
import java.util.*;
import java.util.function.Function;
import java.util.stream.Collector;
import java.util.stream.Collectors;
import java.util.stream.Stream;
import java.nio.charset.StandardCharsets;

 public class Main {

  public static String INPUT_FILE_PATH = "input.txt";
  public static String OUTPUT_FILE_PATH ="output.txt"; 
    
     public static void main(String[] args) throws IOException  {

      
        processing();


     }
    
    
   public static void processing() throws IOException {
             List<String> finalResult = Files.lines(Paths.get(INPUT_FILE_PATH), StandardCharsets.ISO_8859_1).parallel().map(l -> l.split(",")).collect(Collectors.teeing(
                     Collectors.groupingByConcurrent(city -> city[0],Collectors.summingDouble(score ->Double.parseDouble(score[2]))),
                     Collectors.groupingByConcurrent(product -> product[1],Collectors.minBy(Comparator.comparing(price -> Double.parseDouble(price[2])))),
                     (cityByScore , productByMinPrice ) ->{
                         List<String> finalList = new ArrayList<>();
                         Map.Entry<String, Double> resultCity = cityByScore.entrySet().stream().parallel().min(Map.Entry.comparingByValue()).get();
                         String formating = resultCity.getKey()+" "+ new DecimalFormat("#.00").format(resultCity.getValue())+"\n";
                          List<String> resultProduct = productByMinPrice.entrySet().stream().map(tmp ->new Product(tmp.getKey(),Double.parseDouble(tmp.getValue().get()[2])))
                                 .sorted(Comparator.comparing(Product::getValue).thenComparing(Product::getKey)).limit(5).map(productResult ->
                                 productResult.getKey() + " " + new DecimalFormat("#.00").format(productResult.getValue())+ "\n").toList();
                        finalList.add(formating);
                        finalList.addAll(resultProduct);
                        return finalList;
                     }
             ));
             writeOutput("output.txt",finalResult);
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
