import std.stdio;
import std.array : array;
import std.format : formattedRead;
import std.algorithm : min, multiSort;
import std.parallelism : parallel;

void main()
{
    auto fh = File("input.txt");
    double[string] cityCosts;
    double[string][string] products;


    string lastCity;
    foreach(char[] line; fh.byLine().parallel)
    {
        string city;
        string product;
        double price;

        line.formattedRead!`%s,%s,%f`(city, product, price);
        lastCity = city;

        cityCosts[city] += price;
        if(city !in products || product !in products[city])
        {
            products[city][product] = price;
        }
        products[city][product] = min(price, products[city][product]);
    }

    //writeln("cityCosts = ", cityCosts);
    //writeln("products = ", products);
    //writeln("productCosts = ", productCosts);

    string cheapestCity = lastCity;
    foreach(city, cost; cityCosts)
    {
        if(cost < cityCosts[cheapestCity])
        {
            cheapestCity = city;
        }
    }
    
    string[] cheapestProducts = products[cheapestCity].keys().array();
    //writeln("cheapestProducts = ", cheapestProducts);
    writefln!`%s %.2f`(cheapestCity, cityCosts[cheapestCity]);
    cheapestProducts.multiSort!(
        (a, b) => products[cheapestCity][a] < products[cheapestCity][b],
        (a, b) => a < b
    );
    foreach(i, product; cheapestProducts[0 .. 5])
    {
        writefln!`%s %.2f`(product, products[cheapestCity][product]);
    }
}
