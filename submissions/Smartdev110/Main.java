import java.io.*;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.time.LocalTime;
import java.time.temporal.ChronoUnit;
import java.util.*;
import java.util.stream.Collectors;

public class Main {

    private static final DecimalFormat df=new DecimalFormat("0.00", new DecimalFormatSymbols(Locale.ENGLISH));

    public static void main(String[] args) throws IOException {
        LocalTime l1=LocalTime.now();
        File inputFile = new File("input.txt");
        File outputFile = new File("output.txt");

        BufferedReader reader = new BufferedReader(new FileReader(inputFile));
        BufferedWriter writer = new BufferedWriter(new FileWriter(outputFile));

        String currentLine;
        List<Element> list=new ArrayList<>();
        String[] lineSplited;
        Element e;
        while((currentLine = reader.readLine()) != null) {
            lineSplited = currentLine.split(",");
            e=new Element(lineSplited[0],lineSplited[1],Double.parseDouble(lineSplited[2]));
            list.add(e);
        }

        reader.close();

        Map<String, List<Element>> groupByVille= list.stream()
                .collect(Collectors.groupingBy(Element::getVille))
        ;

        List<ElementDto> dtos=new ArrayList<>();
        groupByVille.forEach(
                (s, elements) ->
                dtos.add(new ElementDto(s,elements.stream().
                        collect(Collectors.summingDouble(Element::getPrix)),elements)
                )
        );

        Optional<ElementDto> el= dtos.stream().min((o1, o2) -> (int) (o1.getSumPrix() - o2.getSumPrix()))
        ;
        StringBuilder result=new StringBuilder();

        el.ifPresent(
                elementDto ->
                {
                    result.append(elementDto.getVille()).append(" ").append(df.format(elementDto.getSumPrix())).append(System.getProperty("line.separator"));
                    elementDto.getElementList().stream().sorted(Element::compareTo)
                            .distinct().limit(5)
                            .forEach(element -> {
                                result.append(element.getVariete()).append(" ").append(df.format(element.getPrix())).append(System.getProperty("line.separator"));
                                }
                            );
                }
        );

        writer.write(String.valueOf(result));
        writer.close();
        LocalTime l2=LocalTime.now();
        System.out.println(l1.until(l2, ChronoUnit.SECONDS));
        System.out.println(l1.until(l2, ChronoUnit.MILLIS));
    }


}


class Element implements Comparable {

    private String ville ;
    private String variete;
    private double prix;

    public Element(String ville, String variete, double prix){
        this.ville = ville;
        this.variete = variete;
        this.prix = prix;
    }

    @Override
    public String toString() {
        return "Element{" +
                "ville='" + ville + '\'' +
                ", variete='" + variete + '\'' +
                ", prix='" + prix + '\'' +
                '}';
    }



    public Element() {
    }

    public String getVille() {
        return ville;
    }

    public void setVille(String ville) {
        this.ville = ville;
    }

    public String getVariete() {
        return variete;
    }

    public void setVariete(String variete) {
        this.variete = variete;
    }

    public double getPrix() {
        return prix;
    }

    public void setPrix(double prix) {
        this.prix = prix;
    }


    @Override
    public int compareTo(Object o) {
        Element element = (Element) o;
        if (Double.compare(prix, element.prix) == 0) return variete.compareTo(element.variete);
        else return Double.compare(prix, element.prix);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Element element = (Element) o;
        return Objects.equals(variete, element.variete);
    }

    @Override
    public int hashCode() {
        return Objects.hash(variete);
    }
}


class ElementDto {

    private String ville ;
    private double sumPrix;

    private List<Element> elementList;

    public ElementDto(String ville, double sumPrix, List<Element> elementList){
        this.ville = ville;
        this.sumPrix = sumPrix;
        this.elementList = elementList;
    }

    @Override
    public String toString() {
        return "ElementDto{" +
                "ville='" + ville + '\'' +
                ", sumPrix=" + sumPrix +
                ", elementList=" + elementList +
                '}';
    }

    public ElementDto() {
    }

    public String getVille() {
        return ville;
    }

    public void setVille(String ville) {
        this.ville = ville;
    }

    public double getSumPrix() {
        return sumPrix;
    }

    public void setSumPrix(double sumPrix) {
        this.sumPrix = sumPrix;
    }

    public List<Element> getElementList() {
        return elementList;
    }

    public void setElementList(List<Element> elementList) {
        this.elementList = elementList;
    }
}