public class Program
{
    public static void Main(string[] args)
    {
        var lines = File.ReadLines("input.txt").Select(line => line.Trim().Split(',')).ToArray();

        var cityPrices = new Dictionary<string, decimal>();
        var productPrices = new Dictionary<string, decimal>();

        foreach (var line in lines)
        {
            string city = line[0];
            string product = line[1];
            decimal price = decimal.Parse(line[2]);

            if (price < 1.00m || price > 100.00m)
            {
                continue;
            }

            if (!cityPrices.ContainsKey(city))
            {
                cityPrices[city] = 0m;
            }
            cityPrices[city] += price;

            if (!productPrices.ContainsKey(product) || productPrices[product] > price)
            {
                productPrices[product] = price;
            }
        }

        string cheapestCity = cityPrices.OrderBy(kv => kv.Value).First().Key;
        decimal cheapestPrice = cityPrices[cheapestCity];

        var topProducts = productPrices.OrderBy(kv => kv.Value).Take(5).ToArray();

        using (var writer = new StreamWriter("output.txt"))
        {
            writer.WriteLine($"{cheapestCity} {cheapestPrice:0.0}");
            foreach (var product in topProducts)
            {
                writer.WriteLine($"{product.Key} {product.Value:0.0}");
            }
        }
    }
}
