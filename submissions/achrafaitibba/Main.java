import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteOrder;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.concurrent.atomic.AtomicReference;


public class Main {


    /**
     * >>>>>>>> Constraints:
     * 1.00 <= price <= 100.00
     * 100 products
     * A city may have same product duplicated several times with different prices or same price
     * Result should be in "./output.txt"
     * 1B rows should be processed by this program
     * <p>
     * <p>
     * <p>
     * >>>>>>>> Output format:
     * Should be 6 lines
     * First line : "cityName sum(5 cheapest products of that city)
     * The other 5 lines: first 5 cheapest products sorted by (price, alphabetically)
     * Don't print same product twice if it's cheap with 2 different prices, only print the cheapest
     * All prices decimal with scale of 2
     * No space at the beginning or ending
     * /* Like this
     * Tetouan 48.50
     * gaz 1.30
     * potato 3.50
     * tomato 3.50
     * sugar 4.50
     * flour 5.20
     */

    private static final String FILE = "input.txt";
    private static final String OUTPUT = "output.txt";

    public  static void main(String[] args) throws IOException {
        RandomAccessFile file = new RandomAccessFile(FILE, "r");
        var buffers = buildPieces(file);
        Map<String, List<Product>> productsByCity = new HashMap<>();
        buffers.forEach(piece -> {
            String data = StandardCharsets.UTF_8.decode(piece).toString();
            ArrayList<String> recordsPerBuffer = new ArrayList<>(Arrays.asList(data.split("\n")));
            recordsPerBuffer.stream()
                    .map(pieceOfData -> pieceOfData.split(","))
                    .forEach(record -> {
                        productsByCity.computeIfAbsent(record[0], key -> new ArrayList<>())
                                .add(new Product(record[1], Double.parseDouble(record[2])));
                    });
        });
        /*buffers.forEach(piece -> {
            String data = StandardCharsets.UTF_8.decode(piece).toString();
            ArrayList<String> recordsPerBuffer = new ArrayList<>(Arrays.asList(data.split("\n")));
            recordsPerBuffer.stream()
                    .map(pieceOfData -> pieceOfData.split(","))
                    .forEach(
                            record -> {
                                List<Product> products;
                                if (!productsByCity.isEmpty() && productsByCity.get(record[0]) != null) {
                                    products = productsByCity.get(record[0]);
                                } else {
                                    products = new ArrayList<>();
                                }
                                products.add(new Product(record[1], Double.parseDouble(record[2])));
                                productsByCity.put(record[0], products);
                            }
                    );
        });
*/
        Map<String, Double> totals = totalCalculator(productsByCity);
        String cheapestCityKey = totals.entrySet().stream()
                .min(Comparator.comparing(Map.Entry::getValue)).get().getKey();
        List<Product> winner = productsByCity.get(cheapestCityKey).stream()
                .sorted(
                        Comparator
                                .comparing(Product::price)
                                .thenComparing(Product::name))
                .limit(5)
                .toList();


        try (BufferedWriter writer = new BufferedWriter(new FileWriter(OUTPUT))) {
            writer.write(cheapestCityKey + " " + String.format("%.2f",totals.get(cheapestCityKey)) + "\n");
            winner.forEach(product -> {
                try {
                    writer.write(product.name() + " " + String.format("%.2f",product.price()) + "\n");
                } catch (IOException e) {
                    e.printStackTrace();
                }
            });
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    public  static Map<String, Double> totalCalculator(Map<String, List<Product>> data) {
        Map<String, Double> totalsByCity = new HashMap<>();
        AtomicReference<Double> total = new AtomicReference<>((double) 0);
        data.forEach((key, productList) -> {
            productList.forEach(p -> {
                if(p != null){
                    total.getAndSet((total.get() + p.price()));
                }

            });
            totalsByCity.put(key, total.get());
            total.set(0d);
        });

        return totalsByCity;

    }

    /**
     * todo > explain the following
     * Why using randomAccessFile and not (BufferReader/FileReader...), how it works, what it does
     */
    private static List<MappedByteBuffer> buildPieces(RandomAccessFile file) throws IOException {
        /**
         * The idea here is to divide the whole file into small pieces of data based on the available memory and processor cores,
         * As follows:  fileSize / (Runtime.getRuntime().availableProcessors())
         * Then chooses the smaller value between that and Integer.MAX_VALUE - 512.
         * This ensures that each piece is a manageable size and doesn't exceed the maximum limit.
         * If the calculated piece size is less than or equal to 0, it sets the piece size to the entire file size.
         */
        var fileSize = file.length(); // return the actual file size in bytes, in this case "23623882bytes" ~ "23.6MB"

        /**
         * GPT says hh :
         * -512: Subtracting 512 from Integer.MAX_VALUE
         * provides a buffer to ensure that the resulting value doesn't overflow the maximum integer value.
         * This is a common practice to prevent arithmetic overflow.
         */
        var pieceSize = Math.min(Integer.MAX_VALUE - 512, fileSize / Runtime.getRuntime().availableProcessors());
        if (pieceSize <= 0) {
            pieceSize = fileSize;
        }
        /** The following comments, just for me to understand buffers hh, so I can read it whenever I need it hh
         * Buffer: "explained by chatGPT hh"
         * In computing, a buffer is a temporary storage area typically used to hold data while it's being transferred from one place to another.
         * Think of it like a temporary holding area in memory where data can be stored and manipulated before being used or transferred elsewhere.
         * MappedByteBuffer:
         * A MappedByteBuffer in Java is a special type of buffer that is mapped to a region of a file in the file system.
         * It allows you to work with large files efficiently by mapping sections (the same we did with our file) of the file directly into memory,
         * thereby avoiding the need to load the entire file into memory at once.
         * How it works:
         * When you create a MappedByteBuffer,
         * you specify the file to be mapped and the range of bytes within the file that you want to map into memory.  >> (pieceSize)
         * The operating system then handles the actual mapping process,
         * associating the buffer with the specified region of the file.
         * Once mapped, you can read from or write to the buffer as if it were a regular in-memory buffer,
         * and the changes are automatically reflected in the underlying file.
         */

        var allPieces = new ArrayList<MappedByteBuffer>((int) (fileSize / pieceSize) + 1);
        var start = 0L;
        while (start < fileSize) { // todo, u & explain this while loop
            var position = start + pieceSize;
            if (position < fileSize) {
                file.seek(position);
                while (file.read() != '\n') {
                    position += 1;
                }
                position += 1;
            } else {
                position = fileSize;
            }
            var buf = file.getChannel().map(FileChannel.MapMode.READ_ONLY, start, position - start);
            buf.order(ByteOrder.nativeOrder());
            allPieces.add(buf);
            start = position;
        }
        return allPieces;
    }

    public record Product(String name, double price) {
    }
}