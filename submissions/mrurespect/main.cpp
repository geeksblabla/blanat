#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
//#include <time.h>

using namespace std;


struct CityInfo {
    double total_price;
    unordered_map<string, double> product_prices;
};

bool compareCities(const pair<string, CityInfo>& a, const pair<string, CityInfo>& b) {
    return a.second.total_price < b.second.total_price;
}

int main() {
   // clock_t start = clock();

    ifstream infile("input.txt");
    ofstream outfile("output.txt");

    unordered_map<string, CityInfo> city_prices;

    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string city, product;
        double price;
        getline(ss, city, ',');
        getline(ss, product, ',');
        ss >> price;

        if (city_prices.find(city) == city_prices.end()) {
            city_prices[city] = {0.0, {}};
        }
        if (city_prices[city].product_prices.find(product) == city_prices[city].product_prices.end() || price < city_prices[city].product_prices[product]) {
            city_prices[city].product_prices[product] = price;
        }
        city_prices[city].total_price += price;
    }

    vector<pair<string, CityInfo>> sorted_cities(city_prices.begin(), city_prices.end());
    sort(sorted_cities.begin(), sorted_cities.end(), compareCities);

    outfile << sorted_cities[0].first << " " << fixed << sorted_cities[0].second.total_price << endl;

    auto& cheapest_city_products = sorted_cities[0].second.product_prices;
    vector<pair<string, double>> cheapest_products(cheapest_city_products.begin(), cheapest_city_products.end());
    sort(cheapest_products.begin(), cheapest_products.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
        if (a.second != b.second) return a.second < b.second;
        return a.first < b.first;
    });

    for (int i = 0; i < min(5, (int)cheapest_products.size()); ++i) {
        outfile << cheapest_products[i].first << " " << fixed << cheapest_products[i].second << endl;
    }
    //clock_t stop = clock();
    //double elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
    //printf("\nTime elapsed: %.5f seconds\n", elapsed);
    return 0;
}
