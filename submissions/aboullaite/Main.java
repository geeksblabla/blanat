package com.company;

import java.io.IOException;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.stream.Stream;


// Naive implementation
public class Main {

    private record CityData(String name, String product, double price) {}

    public static void main(String[] args) throws IOException {
        Map<String, Double> cheapestCity = new HashMap<>();
        Map<String, CityData[]> productsByCity = new HashMap<>();
        // Maybe split this one into chucnks
        try (Stream<String> lines = java.nio.file.Files.lines(Paths.get("input.txt"))) {
            lines.forEach(line -> {
                String[] content = line.split(",");
                CityData data = new CityData(content[0], content[1], Double.parseDouble(content[2]));
                // Lot of unboxing here! Optimize ?
                cheapestCity.computeIfPresent(data.name(), (k,v) -> v + data.price());
                cheapestCity.putIfAbsent(data.name(), data.price());
                keepTrackof5CheapestProducts(data, productsByCity);
            });
        }
        String city = Collections.min(cheapestCity.entrySet(),
            Entry.comparingByValue()).getKey();

        // TODO save in a file
        System.out.println(city + " " + String.format("%.2f",cheapestCity.get(city)));
        for (CityData d: productsByCity.get(city))
        {
            System.out.println(d.product +" "+ String.format("%.2f", d.price));
        };
    }

    // Keeping only track of 5 cheapest product by cities. 
    // Using array as a stack here. Sorting every time might be expensive
    private static void keepTrackof5CheapestProducts(CityData cityData,
        Map<String, CityData[]> cache){
        String key = cityData.name();
        if(cache.containsKey(key)){
            CityData data[] = cache.get(key);
            if(data.length < 5){
                data = Arrays.copyOf(data, data.length + 1);
                data[cache.get(key).length] = cityData;
            }else {
                boolean exist = Arrays.stream(data).anyMatch(d -> d.name().equals(cityData.name()));
                if(!exist){
                    if(data[5].price() > cityData.price() ){
                        data[5] = cityData;
                    }
                }else{
                    for (int i=0; i< data.length; i++){
                        if(data[i].name().equals(cityData.name()) && data[i].price() > cityData.price()){
                            data[i] = cityData;
                            break;
                        }
                    }
                }
            }
            Arrays.sort(data, Comparator.comparingDouble(CityData::price));
            cache.put(key, data);
        }else{
            CityData data[] = {cityData};
            cache.put(key,  data);
        }
    }
}
