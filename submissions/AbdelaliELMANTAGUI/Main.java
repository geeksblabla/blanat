import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Stream;

public class Main {
    final static String INPUT_FILE = "input.txt";
    final static String OUTPUT_FILE = "output.txt";
    public static void main(String[] args) {
        try (Stream<String> lines = Files.lines(Paths.get(INPUT_FILE))) {
            solve(lines);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static void solve(Stream<String> lines) throws IOException {
        Map<String, List<Map.Entry<String, Double>>> acc = new HashMap<>();
        var result = lines
                .map(line -> line.split(","))
                .reduce(acc, (accum,current)-> {
                    String city = current[0];
                    String product = current[1];
                    double price = Double.parseDouble(current[2]);
                    var entry = Map.entry(product, price);
                    boolean exist = accum.putIfAbsent(city, new ArrayList<>(List.of(entry))) != null;
                    if(exist){
                        accum.get(city).add(entry);
                    }

                    return accum;
                },(a,b)->b);

        Map.Entry<String, List<Map.Entry<String, Double>>> cheapestCity = null;
        Double total = null;
        for(var productsEntries : result.entrySet()){
            if(cheapestCity == null){
                cheapestCity = productsEntries;
                total = productsEntries.getValue().stream().mapToDouble(Map.Entry::getValue).sum();
                continue;
            }
            double newTotal = productsEntries.getValue().stream().mapToDouble(Map.Entry::getValue).sum();
            if(newTotal < total){
                total = newTotal;
                cheapestCity = productsEntries;
            }
        }


        List<Map.Entry<String,Double>> orderedProducts = cheapestCity.getValue().stream().sorted((e1, e2) -> {
            if(e1.getValue().compareTo(e2.getValue()) == 0){
                return e1.getKey().compareTo(e2.getKey());
            }
            return e1.getValue().compareTo(e2.getValue());
        }).toList();

        List<Map.Entry<String,Double>> cheapestProducts = new ArrayList<>();
        DecimalFormat df = new DecimalFormat("0.00");
        StringBuilder fileContent = new StringBuilder(cheapestCity.getKey() + " " + df.format(total) + "\n");
        for (var product : orderedProducts){
            if(cheapestProducts.stream().noneMatch(p -> p.getKey().equals(product.getKey()))){
                cheapestProducts.add(product);
                fileContent.append(product.getKey()).append(" ").append(df.format(product.getValue())).append("\n");
            }
            if(cheapestProducts.size() == 5) break;
        }
        BufferedWriter bw = new BufferedWriter(new FileWriter(OUTPUT_FILE));
        bw.write(fileContent.toString());
        bw.close();
    }
}