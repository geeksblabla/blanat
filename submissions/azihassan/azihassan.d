import std.stdio;
import std.array : array;
import std.conv : to;
import std.range : enumerate, front;
import std.file : readText;
import std.format : formattedRead;
import std.algorithm : min, multiSort, reduce, splitter;
import std.parallelism : parallel, taskPool;
import std.mmfile : MmFile;
import std.string : lineSplitter, strip, split;

struct Dataset
{
    double[string] cityCosts;
    double[string][string] products;
}

Dataset parseChunk(string chunk)
{
    double[string] cityCosts;
    double[string][string] products;

    //writeln(chunk);

    foreach(line; chunk.lineSplitter())
    {
        auto parts = line.splitter(",");
        string city = parts.front();
        parts.popFront();
        string product = parts.front();
        parts.popFront();
        double price = parts.front().to!double;

        //writeln("city = ", city);
        //writeln("product = ", product);
        //writeln("price = ", price);
        //writeln(i, ", line = ", city, ",", product, ",", price);
        cityCosts[city] += price;
        //writeln(i, ", line = ", city, ",", product, ",", price);
        //if(null == (city in products) || null == (product in products[city]))
        //{
        //    products[city][product] = price;
        //}
        products[city][product] = min(price, products.get(city, (double[string]).init).get(product, int.max));
        //writeln(i, ", line = ", city, ",", product, ",", price);
    }

    //writeln("chunk parsed");
    return Dataset(cityCosts, products);
}

Dataset combine(Dataset a, Dataset b)
{
    foreach(city, cost; b.cityCosts)
    {
        a.cityCosts[city] += cost;
    }
    foreach(city, products; b.products)
    {
        foreach(product, cost; products)
        {
            a.products[city][product] = min(
                a.products
                    .get(city, (double[string]).init)
                    .get(product, int.max),
                cost
            );
        }
    }
    return a;
}

unittest
{
    auto dataset = Dataset(
        [
            "casablanca": 12,
            "rabat": 11
        ],
        [
            "casablanca": [
                "food": 1,
                "rent": 4000
            ],
            "rabat": [
                "rent": 3000,
                "misc": 10
            ]
        ]
    );

    auto dataset2 = Dataset(
        [
            "casablanca": 12,
            "marrakech": 11
        ],
        [
            "casablanca": [
                "food": 1,
                "rent": 3000
            ],
            "marrakech": [
                "rent": 2000,
                "misc": 12
            ]
        ]
    );

    auto actual = combine(dataset, dataset2);
    auto expected = Dataset(
        [
            "casablanca": 24,
            "marrakech": 11,
            "rabat": 11
        ],
        [
            "casablanca": [
                "food": 1,
                "rent": 3000
            ],
            "rabat": [
                "rent": 3000,
                "misc": 10
            ],
            "marrakech": [
                "rent": 2000,
                "misc": 12
            ]
        ]
    );
    writeln(expected == actual);
    writeln(expected, " == ", actual);
    assert(expected == actual);
}

string[] splitChunks(string input, int n)
{
    string[] chunks;
    chunks.reserve(n);
    ulong chunkSize = input.length / n;
    ulong start = 0, end = chunkSize;
    while(end < input.length)
    {
        while(input[end] != '\n')
        {
            end++;
        }
        chunks ~= input[start .. end];
        start = end + 1;
        end += chunkSize;
    }
    chunks ~= input[start .. $];
    return chunks;
}

unittest
{
    string input = "casa,tomato,6.23
casa,tomato,7.23
casa,tomato,8.23
casa,tomato,9.23
casa,potato,4.21
casa,flour,6.24
casa,oil,9.24";

    string[] expected = [
        "casa,tomato,6.23\ncasa,tomato,7.23",
        "casa,tomato,8.23\ncasa,tomato,9.23",
        "casa,potato,4.21\ncasa,flour,6.24",
        "casa,oil,9.24"
    ];
    auto actual = input.splitChunks(4);
    writeln("chunks = ", actual);
    assert(expected == actual);
}

void main()
{
    //auto fh = new MmFile("input.txt", MmFile.Mode.read, 0, null);
    //string input = cast(string) fh[];
    string input = "input.txt".readText();
    //writeln("mmfile loaded");
    Dataset[4] chunks;

    //foreach(i, chunk; input.splitChunks(4).parallel(4))
    foreach(i, chunk; input.splitChunks(4).parallel(4))
    {
        //writeln("chunk ", i, "..");
        //writeln("chunk ", i, "...");
        chunks[i] = parseChunk(chunk);
        //writeln("chunk ", i, " ok");
    }
    //writeln("chunks parsed");

    //writeln("cityCosts = ", cityCosts);
    //writeln("products = ", products);
    //writeln("productCosts = ", productCosts);

    Dataset dataset = taskPool.reduce!combine(chunks[]);
    //writeln("dataset combined");

    string cheapestCity = dataset.cityCosts.byKey().front();
    foreach(city, cost; dataset.cityCosts)
    {
        if(cost < dataset.cityCosts[cheapestCity])
        {
            cheapestCity = city;
        }
    }
    
    auto fout = File("output.txt", "w");
    string[] cheapestProducts = dataset.products[cheapestCity].keys().array();
    //writeln("cheapestProducts = ", cheapestProducts);
    fout.writefln!`%s %.2f`(cheapestCity, dataset.cityCosts[cheapestCity]);
    cheapestProducts.multiSort!(
        (a, b) => dataset.products[cheapestCity][a] < dataset.products[cheapestCity][b],
        (a, b) => a < b
    );
    foreach(i, product; cheapestProducts[0 .. 5])
    {
        fout.writefln!`%s %.2f`(product, dataset.products[cheapestCity][product]);
    }
}


