import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.Optional;
import java.util.stream.Stream;

public class Main {
    private static final DecimalFormat df=new DecimalFormat("0.00", new DecimalFormatSymbols(Locale.ENGLISH));

    public static void main(String[] args) throws IOException {

        File outputFile = new File("output.txt");
        BufferedWriter writer = new BufferedWriter(new FileWriter(outputFile));

        Map<String, Double> villes = new HashMap<>();
        Map<String, Map<String, Double>> varietes = new HashMap<>();

        try(Stream<String > lines = Files.lines(Paths.get("input.txt"))) {
            lines.forEach(line -> {
                String[] splitLine = line.split(",");
                String ville = splitLine[0];
                String variete = splitLine[1];
                double prix = Double.parseDouble(splitLine[2]);

                villes.put(ville, villes.getOrDefault(ville, 0.0) + prix);

                varietes.putIfAbsent(ville, new HashMap<String, Double>());
                Map<String, Double> listOfVarietes = varietes.get(ville);
                listOfVarietes.putIfAbsent(variete, prix);
                listOfVarietes.computeIfPresent(variete, (s, aDouble) -> Math.min(aDouble, prix));

            });

            Optional<Map.Entry<String, Double>> minVille = villes.entrySet().stream().min((o1, o2) -> (int) (o1.getValue() - o2.getValue()));

            StringBuilder result=new StringBuilder();
            minVille.ifPresent(
                    a -> result.append(a.getKey()).append(" ").append(df.format(a.getValue()))
                            .append("\n"));

            minVille.ifPresent(ville -> varietes.get(ville.getKey()).
                    entrySet()
                    .stream()
                    .sorted((o1, o2) -> o1.getValue().compareTo(o2.getValue()) == 0 ? o1.getKey().compareTo(o2.getKey()) : o1.getValue().compareTo(o2.getValue()))
                    .distinct()
                    .limit(5)
                    .forEach(
                            a -> result.append(a.getKey()).append(" ").append(df.format(a.getValue())).append("\n")
                    ));

            writer.write(String.valueOf(result));
            writer.close();


        } catch (IOException e) {
            e.printStackTrace();
        }

    }




}
