package org.example;

import java.io.IOException;
import java.nio.file.Paths;
import java.util.AbstractMap;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.stream.Collectors;
import java.util.stream.Stream;


public class Main {

  public static final String INPUT_TXT = "input.txt";

  private record Product(String product, double price) {}
  private record CityRecord(double total, Product[] products) {
    CityRecord(double price, Product product) {
      this(price, new Product[]{product});
    }
    public static CityRecord combineWith(CityRecord c1, CityRecord c2) {
      return new CityRecord(
          c1.total() + c2.total(),
          Stream.concat(Arrays.stream(c1.products()), Arrays.stream(c2.products))
              .sorted(Comparator.comparingDouble(Product::price).thenComparing(Product::product))
              .distinct()
              .limit(5)
              .toArray(Product[]::new)
      );
    }
  }

  public static void main(String[] args) throws IOException {

    try (Stream<String> lines = java.nio.file.Files.lines(Paths.get(INPUT_TXT))) {
      Map<String, CityRecord> resultMap = lines.parallel().map(line -> {
        String[] content = line.split(",");
        Product product = new Product(content[1], Double.parseDouble(content[2]));
        return new AbstractMap.SimpleEntry<>(content[0], product);
      }).collect(Collectors.toConcurrentMap(
          // Combine/reduce:
          AbstractMap.SimpleEntry::getKey,
          entry -> new CityRecord(entry.getValue().price(), entry.getValue()),
              CityRecord::combineWith));

      resultMap.entrySet().stream()
          .sorted(Entry.comparingByValue(Comparator.comparingDouble(CityRecord::total)))
          .limit(1)
          .forEach(entry -> {
            Product[] products = entry.getValue().products();
            System.out.println(entry.getKey() + " " + String.format( "%.2f", entry.getValue().total()));
            for (Product product : products) {
              System.out.println(product.product() + " " + String.format( "%.2f", product.price()));
            }
          });
    }
  }
}