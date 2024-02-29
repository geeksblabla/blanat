import java.io.BufferedWriter;
import java.io.FileWriter;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Stream;


public class Main {

    private static final String DATASET_URL = "input.txt";
    private static final String OUTPUT_URL = "output.txt";
    private static final int MAX_PRODUCTS = 5;

    public static void main(String[] args){

        Map<String , Data> allData = new HashMap<>();
        allData.put("Imzouren" , new Data());
        allData.put("Khouribga" , new Data());
        allData.put("Bni_Hadifa" , new Data());
        allData.put("Ouazzane" , new Data());
        allData.put("Fes" , new Data());
        allData.put("Tichka" , new Data());
        allData.put("Sidi_Kacem" , new Data());
        allData.put("Tiflet" , new Data());
        allData.put("Sale" , new Data());
        allData.put("El_Jadida" , new Data());
        allData.put("Chefchaouen" , new Data());
        allData.put("Oujda" , new Data());
        allData.put("Midar" , new Data());
        allData.put("Tiznit" , new Data());
        allData.put("Arfoud" , new Data());
        allData.put("Azilal" , new Data());
        allData.put("Azrou" , new Data());
        allData.put("Tichla" , new Data());
        allData.put("Sidi_Bennour" , new Data());
        allData.put("Nador" , new Data());
        allData.put("Chichaoua" , new Data());
        allData.put("Smara" , new Data());
        allData.put("Dakhla" , new Data());
        allData.put("Guelta_Zemmur" , new Data());
        allData.put("Rabat" , new Data());
        allData.put("Tetouan" , new Data());
        allData.put("Sidi_Ifni" , new Data());
        allData.put("Sidi_Slimane" , new Data());
        allData.put("Béni_Mellal" , new Data());
        allData.put("Ouarzazate" , new Data());
        allData.put("Ifrane" , new Data());
        allData.put("Mohammedia" , new Data());
        allData.put("Zemamra" , new Data());
        allData.put("Aourir" , new Data());
        allData.put("Ksar_es_Seghir" , new Data());
        allData.put("Laâyoune" , new Data());
        allData.put("Souk_Larbaa" , new Data());
        allData.put("Ahfir", new Data());
        allData.put("Moulay_Bousselham", new Data());
        allData.put("Casablanca", new Data());
        allData.put("Larache", new Data());
        allData.put("Temara", new Data());
        allData.put("Beni_Mellal", new Data());
        allData.put("Boujdour", new Data());
        allData.put("Bab_Taza", new Data());
        allData.put("Sefrou", new Data());
        allData.put("Tinghir", new Data());
        allData.put("Jerada", new Data());
        allData.put("Bab_Berred", new Data());
        allData.put("Taroudant", new Data());
        allData.put("Tan-Tan", new Data());
        allData.put("Berrechid", new Data());
        allData.put("Al_Hoceima", new Data());
        allData.put("Boulemane", new Data());
        allData.put("Kenitra", new Data());
        allData.put("Bir_Lehlou", new Data());
        allData.put("Bir_Anzerane", new Data());
        allData.put("Demnate", new Data());
        allData.put("Guercif", new Data());
        allData.put("Sidi_Bouzid", new Data());
        allData.put("Ben_guerir", new Data());
        allData.put("Jorf_El_Melha", new Data());
        allData.put("Akhfenir", new Data());
        allData.put("Tafraout", new Data());
        allData.put("Taza", new Data());
        allData.put("Kalaat_MGouna", new Data());
        allData.put("Asilah", new Data());
        allData.put("Goulmima", new Data());
        allData.put("Ksar_El_Kebir", new Data());
        allData.put("Laayoune", new Data());
        allData.put("Layoune", new Data());
        allData.put("Errachidia", new Data());
        allData.put("Saidia", new Data());
        allData.put("Youssoufia", new Data());
        allData.put("Oujda_Angad", new Data());
        allData.put("Assa", new Data());
        allData.put("Tangier", new Data());
        allData.put("Settat", new Data());
        allData.put("Essaouira", new Data());
        allData.put("had_soualem", new Data());
        allData.put("Tarfaya", new Data());
        allData.put("Marrakech", new Data());
        allData.put("Safi", new Data());
        allData.put("Ait_Melloul", new Data());
        allData.put("Figuig", new Data());
        allData.put("Zagora", new Data());
        allData.put("Guelmim", new Data());
        allData.put("Fquih_Ben_Salah", new Data());
        allData.put("Oulad_Teima", new Data());
        allData.put("Inezgane", new Data());
        allData.put("Skhirate", new Data());
        allData.put("Khenifra", new Data());
        allData.put("Khemisset", new Data());
        allData.put("Guerguerat", new Data());
        allData.put("Meknes", new Data());
        allData.put("Agadir", new Data());
        allData.put("Bouarfa", new Data());
        allData.put("Taourirt", new Data());
        allData.put("Midelt", new Data());
        allData.put("Berkane", new Data());
        allData.put("Drarga", new Data());


        // reading the dataset and collecting information
        try (Stream<String> lines = Files.lines(Paths.get(DATASET_URL))){

            lines.parallel().forEach(line -> {
                String[] city_product_price = line.split(",");
                String getCity = city_product_price[0];
                String getProduct = city_product_price[1];
                double getPrice = Double.parseDouble(city_product_price[2]);
                collectingDataV2(getCity , getProduct , getPrice , allData);

            });

        } catch (Exception e) {
            System.out.printf(e.getMessage());
        }


        // find the cheapest city in the list and sort its products based on the price
        Map.Entry<String, Data> cheapestCity = allData.entrySet().stream()
                .min(Comparator.comparingDouble(entry -> entry.getValue().getTotalPrice())).get();
        cheapestCity.getValue().getProducts().sort(Comparator.comparing(Product::productPrice).thenComparing(Product::productName));


        // write the result in the output file
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(OUTPUT_URL))) {
            writer.write(cheapestCity.getKey() + " " + String.format("%.2f" ,cheapestCity.getValue().getTotalPrice()) );
            for (int i = 0; i < MAX_PRODUCTS; i++) {
                writer.write("\n" + cheapestCity.getValue().getProducts().get(i).productName() + " "
                        + String.format("%.2f" , cheapestCity.getValue().getProducts().get(i).productPrice()));
            }
        } catch (Exception e) {
            System.err.println(e.getMessage());
        }

    }


    static synchronized void collectingDataV2(String cityName , String productName , double productPrice , Map<String , Data> allData){
        Product product = new Product(productName , productPrice);
        allData.get(cityName).setProducts(product);
        allData.get(cityName).setTotalPrice(productPrice);
    }

}

class Data {

    private List<Product> products;
    private double totalPrice;

    public Data() {
        products = new ArrayList<>();
        totalPrice = 0;
    }


    public List<Product> getProducts() {
        return products;
    }


    public double getTotalPrice() {
        return totalPrice;
    }


    public void setTotalPrice(double totalPrice){
        this.totalPrice += totalPrice;
    }

    public void setProducts(Product products){
        this.products.add(products);
    }

}

record Product(String productName , double productPrice) {
}