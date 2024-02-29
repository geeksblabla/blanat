import java.io.IOException;
import java.math.RoundingMode;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Comparator;
import java.util.List;
import java.util.LongSummaryStatistics;
import java.util.Map;
import java.util.function.Function;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

public class Main {

    static Stream<String> fruits_and_vegetables = Stream.of(
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
    ).distinct().sorted();

    static String[] products = fruits_and_vegetables.toArray(String[]::new);

    static Map<String, Short> productsStrIntMap = IntStream.range(0, products.length)
            .boxed()
            .collect(Collectors.toMap(i->products[i], Integer::shortValue));


    static Stream<String> moroccan_cities = Stream.of(
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
                    "Imzouren", "Zemamra", "Sidi_Kacem", "Drarga", "Skhirate")
            .distinct().sorted();

    static String[] cities_array = moroccan_cities.toArray(String[]::new);

    static Map<String, Short> citiesStrIntMap = IntStream.range(0, cities_array.length)
            .boxed()
            .collect(Collectors.toMap(i->cities_array[i], Integer::shortValue));

    public static void main(String[] args) throws IOException {
        try (Stream<String> lines = Files.lines(Paths.get("input.txt"))) {

            List<short[]> cityProducts = lines.parallel().map(line-> {
                String[] l = line.split(",");
                return new short[]{citiesStrIntMap.get(l[0]), productsStrIntMap.get(l[1]), (short) (Double.parseDouble(l[2])*100)};
            }).toList();



            Map.Entry<Short, LongSummaryStatistics> cheapestCity = cityProducts.stream().parallel().collect(Collectors.groupingByConcurrent(e->e[0], Collectors.summarizingLong(e->e[2])))
                    .entrySet().stream()
                    .parallel()
                    .min(Comparator.comparing(e->e.getValue().getSum()))
                    .orElse(null);



            String cheapestProducts = cityProducts.stream().parallel().collect(Collectors.groupingByConcurrent(e->e[1], Collectors.summarizingLong(e->e[2])))
                    .entrySet().stream()
                    .sorted(Comparator.comparing(e->e.getValue().getMin()))
                    .limit(5).map(e->products[e.getKey()]+" "+String.format("%.2f", (long)e.getValue().getMin()/100.0)).collect(Collectors.joining("\n"));

            assert cheapestCity != null;
            Files.write(Paths.get("output.txt"), List.of(
                    cities_array[cheapestCity.getKey()] + " " + String.format("%.2f", (long)cheapestCity.getValue().getSum()/100.0),
                    cheapestProducts
            ));

        }
    }

}


