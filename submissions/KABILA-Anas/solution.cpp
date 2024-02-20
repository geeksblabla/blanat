#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <cstring>
#include <iomanip>
#include <unordered_map>
#include <queue>

using namespace std;

struct Product {
    string name;
    double price;

    bool operator<(const Product& p) const {
        return price == p.price ? name > p.name : price > p.price;
    }
};

unordered_map<string, double> cities_prices;
unordered_map<string, double> products_prices;

void process_chunk(const string &chunk)
{
    istringstream is(chunk);
    string line;
    string city, product;
    double price;

    while (getline(is, line))
    {
        istringstream iss(line);
        getline(iss, city, ',');
        getline(iss, product, ',');
        iss >> price;

        cities_prices[city] += price;

        if (products_prices.find(product) == products_prices.end() || products_prices[product] > price)
            products_prices[product] = price;
    }
}

void blanat()
{
    ifstream file("input.txt", ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file: " << strerror(errno) << endl;
        return;
    }

    constexpr size_t bufferSize = 1024 * 1024;
    unique_ptr<char[]> buffer(new char[bufferSize]);

    while (file)
    {
        file.read(buffer.get(), bufferSize);
        process_chunk(buffer.get());
    }

    file.close();

    auto cheapest_city = min_element(cities_prices.begin(), cities_prices.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    string cheapestCity = cheapest_city->first;
    double cheapestCityTotalPrice = cheapest_city->second;

    priority_queue<Product> cheapestProducts;

    for (const auto& [product, price] : products_prices) {
        cheapestProducts.push({product, price});
    }

    ofstream output("output.txt");
    if (!output.is_open()) {
        cerr << "Error opening file: " << strerror(errno) << endl;
        return;
    }

    output << cheapestCity << " " << fixed << setprecision(2) << cheapestCityTotalPrice << endl;

    int count = 0;
    while (count < 5 && !cheapestProducts.empty()) {
        output << cheapestProducts.top().name << " " << fixed << setprecision(2) << cheapestProducts.top().price << endl;
        cheapestProducts.pop();
        count++;
    }

    output.close();
}

int main()
{
    blanat();
    return 0;
}