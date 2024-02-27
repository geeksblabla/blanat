import java.io.*;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.DoubleAdder;
import java.util.List;
import java.util.Map;

class C {
    static List<String> f = Arrays.asList("Apple", "Banana", "Orange", "Strawberry", "Grapes", "Watermelon", "Pineapple", "Mango", "Kiwi", "Peach", "Plum", "Cherry", "Pear", "Blueberry", "Raspberry", "Blackberry", "Cantaloupe", "Honeydew", "Coconut", "Lemon", "Lime", "Grapefruit", "Avocado", "Papaya", "Guava", "Fig", "Passion_Fruit", "Apricot", "Nectarine", "Cucumber", "Carrot", "Broccoli", "Spinach", "Kale", "Lettuce", "Tomato", "Bell_Pepper", "Zucchini", "Eggplant", "Cabbage", "Cauliflower", "Brussels_Sprouts", "Radish", "Beet", "Asparagus", "Artichoke", "Green_Beans", "Peas", "Celery", "Onion", "Garlic", "Potato", "Sweet_Potato", "Yam", "Butternut_Squash", "Acorn_Squash", "Pumpkin", "Goji_Berry", "Currant", "Date", "Clementine", "Cranberry", "Rhubarb", "Chard", "Collard_Greens", "Parsley", "Cilantro", "Mint", "Basil", "Thyme", "Rosemary", "Sage", "Dill", "Oregano", "Pomegranate", "Jackfruit", "Starfruit", "Persimmon", "Ginger", "Turnip", "Jicama", "Kohlrabi", "Watercress", "Okra", "Plantain", "Cactus_Pear", "Kiwano", "Squash_Blossom", "Dragon_Fruit", "Parsnip", "Rutabaga", "Salsify", "Bok_Choy", "Endive");
    String n;
    ConcurrentHashMap<String, Double> m;
    DoubleAdder s = new DoubleAdder();

    public void a(String g, double va) {
        m.compute(g, (k, v) -> va < v ? va : v);
        s.add(va);
    }

    C(String r) {
        this.n = r;
        s.add(-1.0);
        m = new ConcurrentHashMap<>();
        f.parallelStream().forEach(z -> m.put(z, Double.POSITIVE_INFINITY));
    }
}

public class Main {
    static ConcurrentHashMap<String, C> i = new ConcurrentHashMap<>();
    static List<String> mc = Arrays.asList("Casablanca", "Rabat", "Marrakech", "Fes", "Tangier", "Agadir", "Meknes", "Oujda", "Kenitra", "Tetouan", "Safi", "El_Jadida", "Beni_Mellal", "Errachidia", "Taza", "Essaouira", "Khouribga", "Guelmim", "Jorf_El_Melha", "Laayoune", "Ksar_El_Kebir", "Sale", "Bir_Lehlou", "Arfoud", "Temara", "Mohammedia", "Settat", "Béni_Mellal", "Nador", "Kalaat_MGouna", "Chichaoua", "Chefchaouen", "Al_Hoceima", "Taourirt", "Taroudant", "Guelta_Zemmur", "Dakhla", "La\u00E2youne", "Tiznit", "Tinghir", "Ifrane", "Azrou", "Bab_Taza", "Berrechid", "Sidi_Slimane", "Souk_Larbaa", "Tiflet", "Sidi_Bennour", "Larache", "Tan-Tan", "Sidi_Ifni", "Goulmima", "Midelt", "Figuig", "Azilal", "Jerada", "Youssoufia", "Ksar_es_Seghir", "Tichka", "Ait_Melloul", "Layoune", "Ben_guerir", "Ouarzazate", "Inezgane", "Oujda_Angad", "Sefrou", "Aourir", "Oulad_Teima", "Tichla", "Bni_Hadifa", "Fquih_Ben_Salah", "Guercif", "Bouarfa", "Demnate", "Ahfir", "Berkane", "Akhfenir", "Boulemane", "Khenifra", "Bir_Anzerane", "Assa", "Smara", "Boujdour", "Tarfaya", "Ouazzane", "Zagora", "had_soualem", "Saidia", "Bab_Berred", "Midar", "Moulay_Bousselham", "Khemisset", "Guerguerat", "Asilah", "Sidi_Bouzid", "Tafraout", "Imzouren", "Zemamra", "Sidi_Kacem", "Drarga", "Skhirate");

    public static void main(String[] args) {
        System.setProperty("java.util.concurrent.ForkJoinPool.common.parallelism", String.valueOf(Runtime.getRuntime().availableProcessors()));
        mc.parallelStream().forEach(c -> i.put(c, new C(c)));
        try (BufferedReader b = new BufferedReader(new FileReader("input.txt"))) {
            b.lines().parallel().forEach(lines -> {
                String[] p = lines.split(",");
                //Laâyoune isn't being read properly from the file
                i.computeIfAbsent(p[0].trim(), C::new).a(p[1].trim(), Double.parseDouble(p[2].trim()));
            });
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        try (PrintWriter w = new PrintWriter("./output.txt")) {
            i.values().parallelStream()
                    .filter(x -> x.s.doubleValue() > 0.0)
                    .min(Comparator.comparingDouble(c -> c.s.doubleValue()))
                    .ifPresent(a -> {
                        w.printf("%s %.2f%n", a.n, a.s.doubleValue() + 1.0);
                        a.m.entrySet().parallelStream()
                                .sorted(Map.Entry.comparingByValue())
                                .limit(5)
                                .forEach(entry -> w.printf("%s %.2f%n", entry.getKey(), entry.getValue()));
                    });
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
