
import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.nio.ByteBuffer;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CodingErrorAction;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {

    static class CityProduct {
        String city;
        String product;

        String price;

        public CityProduct(String city, String product, String price) {
            this.city = city;
            this.product = product;
            this.price = price;
        }

        @Override
        public int hashCode() {
            return Objects.hash(city, product);
        }

        public String getCity() {
            return city;
        }

        public String getProduct() {
            return product;
        }

        public BigDecimal getPrice() {
            return new java.math.BigDecimal(price).setScale(2, RoundingMode.HALF_UP);
        }

        public String getProductWithPrice() {
            return product + " " + price;
        }

        public String getCityWithPrice() {
            return city + " " + price;
        }
    }

    public static void main(String[] args) throws IOException {
        long start = System.nanoTime();
        try (Stream<String> lines = Files.lines(Paths.get("input.txt"))) {
            List<CityProduct> cityProducts = lines.map(line->line.split(",")).map(l->new CityProduct(l[0], l[1], l[2])).toList();
            CityProduct cheapestCity = cityProducts.stream().collect(Collectors.groupingBy(CityProduct::getCity))
                    .entrySet().stream()
                    .map(e-> new CityProduct(e.getKey(), "", e.getValue().stream()
                            .map(CityProduct::getPrice)
                            .reduce(BigDecimal::add).orElse(BigDecimal.ZERO).toString()))
                    .min(Comparator.comparing(CityProduct::getPrice))
                    .orElse(new CityProduct("","", ""));
            Stream<CityProduct> cheapestProducts = cityProducts.stream().collect(Collectors.groupingBy(CityProduct::getProduct))
                    .entrySet().stream()
                    .map(e-> new CityProduct("", e.getKey(), e.getValue().stream().min(Comparator.comparing(CityProduct::getPrice)).orElse(new CityProduct("","", "0.0")).getPrice().toString()))
                    .sorted(Comparator.comparing(CityProduct::getPrice).thenComparing(CityProduct::getProduct))
                    .limit(5);


            //printing to file
            Files.write(Paths.get("output.txt"), List.of(
                    cheapestCity.getCityWithPrice(),
                    cheapestProducts.map(CityProduct::getProductWithPrice).collect(Collectors.joining("\n"))
            ));
        }
        long end = System.nanoTime();
        System.out.println(end-start);
    }

}


