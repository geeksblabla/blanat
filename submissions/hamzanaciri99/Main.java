import java.util.*;
import java.util.concurrent.*;
import java.io.*;
import java.nio.channels.FileChannel;
import java.nio.ByteBuffer;
import java.nio.file.*;
import java.util.concurrent.atomic.DoubleAdder;


public class Main {

  static String input = "input.txt", output = "output.txt";

  static class Product implements Comparable<Product> {
    String name;
    double price;

    public Product(String name, double price) {
      this.name = name;
      this.price = price;
    }

    @Override
    public int compareTo(Product p) {
      if (p.price != this.price)
        return Double.compare(this.price, p.price);
      return this.name.compareTo(p.name);
    }

    @Override
    public boolean equals(Object p) {
      Product o = (Product) p;
      return this.name.equals(o.name);
    }

  }

  static class ConcurrentFileReader {
    private final String filePath;
    private final int numThreads;

    public ConcurrentFileReader(String filePath, int numThreads) {
      this.filePath = filePath;
      this.numThreads = numThreads;
    }

    public void processAll() throws IOException, InterruptedException, ExecutionException {
      try (FileInputStream fileInputStream = new FileInputStream(filePath);
             FileChannel fileChannel = fileInputStream.getChannel()) {
        long fileSize = Files.size(Paths.get(filePath));
        long segmentSize = fileSize / numThreads;
        ByteBuffer[] buffers = new ByteBuffer[1000];
        
        long start = 0;
        int idx = 0;
        while(start < fileSize) {
          buffers[idx++] = fileChannel.map(FileChannel.MapMode.READ_ONLY, start, Math.min(fileSize - start, Integer.MAX_VALUE));
          start += Math.min(fileSize, Integer.MAX_VALUE);
        }
        

        ExecutorService es = Executors.newFixedThreadPool(numThreads);
        List<Callable<Pair>> tasks = new ArrayList<>();
        for (int i = 0; i < numThreads; i++) {
          long startOffset = i * segmentSize;
          long endOffset = (i == numThreads - 1) ? fileSize : startOffset + segmentSize;
          tasks.add(new ReaderThread(buffers, startOffset, endOffset));
        }

        java.util.List<Future<Pair>> results = es.invokeAll(tasks);
        es.shutdown();

        List<Pair> res = new ArrayList<>();
        for(Future<Pair> r : results) {
          res.add(r.get());
        }
        Collections.sort(res);

        PrintWriter out = new PrintWriter(new File(output));
        out.printf("%s %.2f\n", res.get(0).city, res.get(0).score.doubleValue());
        int l = Math.min(5, res.get(0).products.size());
        for(int i = 0; i < l; i++) {
          Product p = res.get(0).products.get(i);
          out.printf("%s %.2f\n", p.name, p.price);
        }
        out.flush();
      }
    }

    class ReaderThread implements Callable<Pair> {
      private ByteBuffer[] buffers;
      private long startOffset;
      private long endOffset;

      public ReaderThread(ByteBuffer[] buffers, long startOffset, long endOffset) {
        this.buffers = buffers;
        this.startOffset = startOffset;
        this.endOffset =  endOffset;
      }

      public char get() throws Exception {
        int bufferIndex = (int) (startOffset / Integer.MAX_VALUE);
        int offsetInBuffer = (int) (startOffset % Integer.MAX_VALUE);
        if (offsetInBuffer < buffers[bufferIndex].capacity()) {
          return (char) buffers[bufferIndex].get(offsetInBuffer);
        }
        throw new Exception("end of file");
      }

      @Override
      public Pair call() throws Exception {
        try {
          char c = get();
          while(startOffset != 0 && c  != '\n') {
            startOffset++;
            c = get();
          }

          while (startOffset <= endOffset) {
            if (c == '\n') startOffset++;
            StringBuilder sb = new StringBuilder();
            while((c = get()) != '\n') {
              startOffset++;
              sb.append(c);
            }
            StringTokenizer st = new StringTokenizer(sb.toString(), ",");
            String city = st.nextToken();
            String product = st.nextToken();
            Float price = Float.parseFloat(st.nextToken());
            processLine(city, product, price);
          }
        } catch(Exception e) {}

        Pair res = new Pair();
        res.score = new DoubleAdder();
        res.score.add(Double.MAX_VALUE);
        for(Map.Entry<String, Pair> e: cities.entrySet()) {
          if(e.getValue().score.doubleValue() < res.score.doubleValue()) {
            res.score = e.getValue().score;
            res.city = e.getKey();
            res.products = e.getValue().products;
          }
        };
        
        return res;
      }
    }

    static class Pair implements Comparable<Pair> {
      DoubleAdder score = new DoubleAdder();
      String city;
      Vector<Product> products = new Vector<>();

      @Override
      public int compareTo(Main.ConcurrentFileReader.Pair o) {
        if(!this.score.equals(o.score)) return Double.compare(this.score.doubleValue(), o.score.doubleValue());
        return this.city.compareTo(o.city);
      }
    }

    static ConcurrentHashMap<String, Pair>  cities = new ConcurrentHashMap<>();

    public void processLine(String city, String product, double price) throws Exception {
      cities.computeIfAbsent(city, k -> new Pair());
      Pair p = cities.get(city);
      p.score.add(price);
      p.products.add(new Product(product, price));
      Collections.sort(p.products);
      Vector<Product> p2 = new Vector<>();
      p.products.forEach(pr -> {
        if(p2.size() >= 5 || p2.contains(pr)) return;
        p2.add(pr);
      });
      p.products = p2;
    }
  }

  public static void main(String[] args) throws Exception {
    ConcurrentFileReader in = new ConcurrentFileReader(input, Runtime.getRuntime().availableProcessors());
    in.processAll();
  }
}