import std.stdio;
import std.array : array;
import std.range : enumerate;
import std.file : readText;
import std.format : formattedRead;
import std.algorithm : min, multiSort, reduce;
import std.parallelism : parallel, taskPool;
import std.mmfile : MmFile;
import std.string : lineSplitter, strip;

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

    foreach(i, line; chunk.lineSplitter().enumerate)
    {
        string city;
        string product;
        double price;

        line.formattedRead!`%s,%s,%f`(city, product, price);

        //writeln(i, ", line = ", city, ",", product, ",", price);
        cityCosts[city] += price;
        if(null == (city in products) || null == (product in products[city]))
        {
            products[city][product] = price;
        }
        products[city][product] = min(price, products[city][product]);
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
        chunks ~= input[start .. end].strip();
        start = end + 1;
        end += chunkSize;
    }
    chunks ~= input[start .. $].strip();
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
    //auto fh = new MmFile("input.txt");
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
    
    string[] cheapestProducts = dataset.products[cheapestCity].keys().array();
    //writeln("cheapestProducts = ", cheapestProducts);
    writefln!`%s %.2f`(cheapestCity, dataset.cityCosts[cheapestCity]);
    cheapestProducts.multiSort!(
        (a, b) => dataset.products[cheapestCity][a] < dataset.products[cheapestCity][b],
        (a, b) => a < b
    );
    foreach(i, product; cheapestProducts[0 .. 5])
    {
        writefln!`%s %.2f`(product, dataset.products[cheapestCity][product]);
    }
}


