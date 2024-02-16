import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import java.io.FileWriter;

import static java.util.stream.Collectors.toList;

public class Main {

    private record CityData(String name, String product, BigDecimal price) {}
    
    public static void main(String[] args) {
        processData();
    }
    private static void processData(){
        Map<String, BigDecimal> map = new HashMap<>();
        List<CityData> data = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader("/home/adnan/IdeaProjects/blabla/src/file.txt"))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] lineArr = line.split(",");
                String city = lineArr[0];
                String name = lineArr[1];
                BigDecimal price = new BigDecimal(lineArr[2]);
                if(map.keySet().contains(city)){
                    BigDecimal totalPrice = map.get(city);
                    map.put(city, totalPrice.add(price));
                }else{
                    map.put(city, price);
                }
                data.add(new CityData(city, name, price));
                data = filterCheapestProdList(data, city);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
         writer(map, data);
    }

    private static List<CityData> filterCheapestProdList(List<CityData> cityData, String name){
        Stream<CityData> collect1 = cityData.stream()
                .filter(cd -> cd.name.equals(name))
                .sorted((p1, p2) -> p1.price.compareTo(p2.price))
                .collect(Collectors.toMap(CityData::product, obj -> obj, (first, second) -> first))
                .values()
                .stream()
                .limit(5);

        Stream<CityData> collect2 = cityData.stream()
                .filter(cd -> !cd.name.equals(name));

        return Stream.concat(collect1, collect2).collect(toList());

    }

    private static String formatResult(Map<String, BigDecimal> map, List<CityData> data){
        Map.Entry<String, BigDecimal> min = Collections.min(map.entrySet(),
                Map.Entry.comparingByValue());
        Stream<CityData> fiveCheapestProd = data.stream()
                .filter(cd -> cd.name.equals(min.getKey()))
                .sorted((cd1, cd2) -> cd1.price.compareTo(cd2.price));

        StringBuilder sb = new StringBuilder();
        sb.append(min.getKey() + " " + min.getValue().setScale(2, RoundingMode.FLOOR));
        fiveCheapestProd.forEach(cd -> sb.append("\n" + cd.product + " " + cd.price.setScale(2, RoundingMode.FLOOR)) );
        return sb.toString();
    }


    private static void writer(Map<String, BigDecimal> map, List<CityData> data){
        try {
            FileWriter wr = new FileWriter("output.txt");
            wr.write(formatResult(map, data));
            wr.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}