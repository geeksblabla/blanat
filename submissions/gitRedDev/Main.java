import java.io.IOException;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {

    static List<String> fruits_and_vegetables = Stream.of(
            "Apple", "Banana", "Orange", "Strawberry", "Grapes",
            "Watermelon", "Pineapple", "Mango", "Kiwi", "Peach",
            "Plum", "Cherry", "Pear", "Blueberry", "Raspberry",
            "Blackberry", "Cantaloupe", "Honeydew", "Coconut", "Pomegranate",
            "Lemon", "Lime", "Grapefruit", "Avocado", "Papaya",
            "Guava", "Fig", "Passion_Fruit", "Apricot", "Nectarine",
            "Cucumber", "Carrot", "Broccoli", "Spinach", "Kale",
            "Lettuce", "Tomato", "Bell_Pepper", "Zucchini", "Eggplant",
            "Cabbage", "Cauliflower", "Brussels_Sprouts", "Radish", "Beet",
            "Asparagus", "Artichoke", "Green_Beans", "Peas", "Celery",
            "Onion", "Garlic", "Potato", "Sweet_Potato", "Yam",
            "Butternut_Squash", "Acorn_Squash", "Pumpkin", "Cranberry", "Goji_Berry",
            "Currant", "Date", "Clementine", "Cranberry", "Rhubarb",
            "Chard", "Collard_Greens", "Parsley", "Cilantro", "Mint",
            "Basil", "Thyme", "Rosemary", "Sage", "Dill",
            "Oregano", "Cantaloupe", "Honeydew", "Coconut", "Pomegranate",
            "Jackfruit", "Starfruit", "Persimmon", "Ginger", "Turnip",
            "Jicama", "Kohlrabi", "Watercress", "Okra", "Artichoke",
            "Plantain", "Cactus_Pear", "Kiwano", "Squash_Blossom", "Dragon_Fruit",
            "Parsnip", "Rutabaga", "Salsify", "Bok_Choy", "Endive"
    ).sorted().toList();






    static List<String> moroccan_cities = Stream.of(
            "Casablanca", "Rabat", "Marrakech", "Fes", "Tangier",
            "Agadir", "Meknes", "Oujda", "Kenitra", "Tetouan",
            "Safi", "El_Jadida", "Beni_Mellal", "Errachidia",
            "Taza", "Essaouira", "Khouribga", "Guelmim",
            "Jorf_El_Melha", "Laayoune", "Ksar_El_Kebir", "Sale", "Bir_Lehlou",
            "Arfoud", "Temara", "Mohammedia", "Settat",
            "Béni_Mellal", "Nador", "Kalaat_MGouna",
            "Chichaoua", "Chefchaouen", "Al_Hoceima", "Taourirt",
            "Taroudant", "Guelta_Zemmur", "Dakhla", "Laâyoune",
            "Tiznit","Tinghir", "Ifrane", "Azrou", "Bab_Taza",
            "Berrechid", "Sidi_Slimane", "Souk_Larbaa", "Tiflet", "Sidi_Bennour",
            "Larache", "Tan-Tan", "Sidi_Ifni", "Goulmima",
            "Midelt", "Figuig", "Azilal", "Jerada", "Youssoufia",
            "Ksar_es_Seghir", "Tichka", "Ait_Melloul",
            "Layoune", "Ben_guerir", "Ouarzazate", "Inezgane",
            "Oujda_Angad", "Sefrou", "Aourir",
            "Oulad_Teima", "Tichla", "Bni_Hadifa",
            "Fquih_Ben_Salah", "Guercif", "Bouarfa", "Demnate",
            "Ahfir", "Berkane", "Akhfenir", "Boulemane",
            "Khenifra", "Bir_Anzerane", "Assa", "Smara", "Boujdour",
            "Tarfaya", "Ouazzane", "Zagora", "had_soualem",
            "Saidia", "Bab_Berred", "Midar", "Moulay_Bousselham",
            "Khemisset", "Guerguerat", "Asilah", "Sidi_Bouzid", "Tafraout",
            "Imzouren", "Zemamra", "Sidi_Kacem", "Drarga", "Skhirate"
    ).sorted().toList();

    static class CityProduct {
        int city;
        int product;
        long price;

        public CityProduct(String city, String product, String price) {
            this.city = city.hashCode();
            this.product = product.hashCode();
            this.price =  new java.math.BigDecimal(price).setScale(2, RoundingMode.HALF_UP).longValue();
        }

        public CityProduct(int city, int product, long price) {
            this.city = city;
            this.product = product;
            this.price =  price;
        }



        @Override
        public int hashCode() {
            return Objects.hash(city, product);
        }

        public int getCity() {
            return city;
        }

        public int getProduct() {
            return product;
        }

        public long getPrice() {
            return price;
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
            List<CityProduct> cityProducts = lines.map(line-> {
                String[] l = line.split(",");
                return new CityProduct(l[0], l[1], l[2]);
            }).toList();

            List<Map.Entry<Integer, LongSummaryStatistics>> cities = cityProducts.stream().collect(Collectors.groupingBy(CityProduct::getCity, Collectors.summarizingLong(CityProduct::getPrice)))
                    .entrySet().stream()
                    .toList();

            Map.Entry<Integer, LongSummaryStatistics> cheapestCity = cities.stream()
                    .min(Comparator.comparing(e->e.getValue().getSum()))
                    .orElse(null);
            Stream<CityProduct> cheapestProducts = cityProducts.stream().sorted(Comparator.comparing(CityProduct::getPrice).thenComparing(CityProduct::getProduct))
                    .distinct()
                    .limit(5);




        }
        long end = System.nanoTime();
        System.out.println(end-start);
    }

}


