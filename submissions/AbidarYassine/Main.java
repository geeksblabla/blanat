package org.example.test.test2;

/**
 * @author yassineabidar on 27/2/2024
 * use JDK 21
 */
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.text.DecimalFormat;
import java.util.*;
import java.util.concurrent.ForkJoinPool;
import java.util.stream.Collectors;

public class Main {
	
	private static final String INPUT_TXT = "input.txt";
	private static final String OUTPUT_TXT = "output.txt";
	private static final DecimalFormat DECIMAL_FORMAT = new DecimalFormat("#.00");
	
	public static void process() throws IOException {
		ForkJoinPool customThreadPool = new ForkJoinPool(Runtime.getRuntime().availableProcessors());
		try {
			List<String> result = customThreadPool.submit(Main::processData).join();
			writeFile(result);
		} finally {
			customThreadPool.shutdown();
		}
	}
	
	private static List<String> processData() throws IOException {
		return Files.lines(Paths.get(INPUT_TXT), StandardCharsets.ISO_8859_1).parallel()
				.map(line -> line.split(","))
				.collect(Collectors.teeing(
						Collectors.groupingByConcurrent(city -> city[0], Collectors.summingDouble(score -> Double.parseDouble(score[2]))),
						Collectors.groupingByConcurrent(product -> product[1], Collectors.minBy(Comparator.comparing(price -> Double.parseDouble(price[2])))),
						Main::compileResults
				));
	}
	
	private static List<String> compileResults(Map<String, Double> cityByScore, Map<String, Optional<String[]>> productByMinPrice) {
		List<String> finalList = new ArrayList<>();
		cityByScore.entrySet().stream().min(Map.Entry.comparingByValue())
				.ifPresent(resultCity -> finalList.add(resultCity.getKey() + " " + DECIMAL_FORMAT.format(resultCity.getValue()) + "\n"));
		
		productByMinPrice.entrySet().stream()
				.flatMap(entry -> entry.getValue().stream())
				.map(entry -> new Product(entry[1], Double.parseDouble(entry[2])))
				.sorted(Comparator.comparing(Product::price).thenComparing(Product::name))
				.limit(5)
				.forEach(product -> finalList.add(product.name() + " " + DECIMAL_FORMAT.format(product.price()) + "\n"));
		
		return finalList;
	}
	
	private static void writeFile(List<String> lines) throws IOException {
		try (FileChannel channel = FileChannel.open(Paths.get(OUTPUT_TXT), StandardOpenOption.CREATE, StandardOpenOption.WRITE)) {
			for (String line : lines) {
				ByteBuffer buffer = ByteBuffer.wrap(line.getBytes(StandardCharsets.UTF_8));
				channel.write(buffer);
			}
		}
	}
	
	public record Product(String name, Double price) {}
	
	public static void main(String[] args) {
			process();
		}
	}
}
