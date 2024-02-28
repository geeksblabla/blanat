import java.io.*;
import static java.lang.Math.toIntExact;
import java.nio.*;
import java.nio.channels.*;
import java.nio.charset.Charset;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.DoubleAdder;

class C  {
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
public class Main implements Runnable {
    private FileChannel _c;
    private long _sl;
    private int _s;
    int _s_n;
    static ConcurrentHashMap<String, C> i = new ConcurrentHashMap<>();
    public static List<String> mc = Arrays.asList("Casablanca", "Rabat", "Marrakech", "Fes", "Tangier", "Agadir", "Meknes", "Oujda", "Kenitra", "Tetouan", "Safi", "El_Jadida", "Beni_Mellal", "Errachidia", "Taza", "Essaouira", "Khouribga", "Guelmim", "Jorf_El_Melha", "Laayoune", "Ksar_El_Kebir", "Sale", "Bir_Lehlou", "Arfoud", "Temara", "Mohammedia", "Settat", "Béni_Mellal", "Nador", "Kalaat_MGouna", "Chichaoua", "Chefchaouen", "Al_Hoceima", "Taourirt", "Taroudant", "Guelta_Zemmur", "Dakhla", "La\u00E2youne", "Tiznit", "Tinghir", "Ifrane", "Azrou", "Bab_Taza", "Berrechid", "Sidi_Slimane", "Souk_Larbaa", "Tiflet", "Sidi_Bennour", "Larache", "Tan-Tan", "Sidi_Ifni", "Goulmima", "Midelt", "Figuig", "Azilal", "Jerada", "Youssoufia", "Ksar_es_Seghir", "Tichka", "Ait_Melloul", "Layoune", "Ben_guerir", "Ouarzazate", "Inezgane", "Oujda_Angad", "Sefrou", "Aourir", "Oulad_Teima", "Tichla", "Bni_Hadifa", "Fquih_Ben_Salah", "Guercif", "Bouarfa", "Demnate", "Ahfir", "Berkane", "Akhfenir", "Boulemane", "Khenifra", "Bir_Anzerane", "Assa", "Smara", "Boujdour", "Tarfaya", "Ouazzane", "Zagora", "had_soualem", "Saidia", "Bab_Berred", "Midar", "Moulay_Bousselham", "Khemisset", "Guerguerat", "Asilah", "Sidi_Bouzid", "Tafraout", "Imzouren", "Zemamra", "Sidi_Kacem", "Drarga", "Skhirate");
    public Main(long loc, int size, FileChannel chnl, int sequence) {
        _sl = loc;
        _s = size;
        _c = chnl;
        _s_n = sequence;
    }
    @Override
    public void run() {
        try {
            ByteBuffer buff = ByteBuffer.allocate(_s);
            _c.read(buff, _sl);
            String string_chunk = new String(buff.array(), Charset.forName("UTF-8"));
            string_chunk.lines().parallel().forEach(lines -> {
                String[] p = lines.split(",");
                if (p.length == 3 ) {try {i.get(p[0].trim()).a(p[1].trim(), Double.parseDouble(p[2].trim()));}catch (Exception e){}}
            });
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
    public static void main(String[] args) throws Exception {
        int av = 1;
        FileInputStream fileInputStream = new FileInputStream("input.txt");
        FileChannel channel = fileInputStream.getChannel();
        long remaining_size = channel.size();
        long chunk_size = remaining_size / av;
        if (chunk_size > (Integer.MAX_VALUE - 5)) {
            chunk_size = (Integer.MAX_VALUE - 5);
        }
        mc.parallelStream().forEach(c -> i.put(c, new C(c)));
        ExecutorService executor = Executors.newFixedThreadPool(av);
        long start_loc = 0;
        int q = 0;
        while (remaining_size >= chunk_size) {
            executor.execute(new Main(start_loc, toIntExact(chunk_size), channel, q));
            remaining_size = remaining_size - chunk_size;
            start_loc = start_loc + chunk_size;
            q++;
        }
        executor.execute(new Main(start_loc, toIntExact(remaining_size), channel, q));
        executor.shutdown();
        while (!executor.isTerminated()) {
        }
        try (PrintWriter w = new PrintWriter("./output.txt")) {
            i.values().parallelStream()
                    .filter(x -> x.s.doubleValue() > 0.0)
                    .min(Comparator.comparingDouble(c -> c.s.doubleValue()))
                    .ifPresent(a -> {
                        w.printf("%s %.2f%n", a.n, a.s.doubleValue() + 1.0);
                        a.m.entrySet().stream()
                                .sorted(Comparator.comparing(Map.Entry<String, Double>::getValue)
                                        .thenComparing(Map.Entry::getKey))
                                .limit(5)
                                .forEach(entry -> w.printf("%s %.2f%n", entry.getKey(), entry.getValue()));
                    });
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
