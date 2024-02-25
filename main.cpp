#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

struct Product {
    string name;
    double price;
};

struct City {
    string name;
    double total_price;
    vector<Product> products;
};

// Function to read the input file and populate the data structures
void read_input(string filename, map<string, City>& cities) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string city_name, product_name, price_str;
        getline(ss, city_name, ',');
        getline(ss, product_name, ',');
        getline(ss, price_str, ',');
        double price = stod(price_str);

        if (cities.find(city_name) == cities.end()) {
            cities[city_name] = City{city_name, 0, {}};
        }
        cities[city_name].total_price += price;
        cities[city_name].products.push_back(Product{product_name, price});
    }
    file.close();
}

// Function to find the city with the lowest total price
string find_cheapest_city(map<string, City>& cities) {
    string cheapest_city;
    double min_total_price = numeric_limits<double>::max();
    for (auto& pair : cities) {
        if (pair.second.total_price < min_total_price) {
            min_total_price = pair.second.total_price;
            cheapest_city = pair.first;
        }
    }
    return cheapest_city;
}

// Function to find the five cheapest products
vector<Product> find_cheapest_products(map<string, City>& cities) {
    vector<Product> all_products;
    for (auto& pair : cities) {
        all_products.insert(all_products.end(), pair.second.products.begin(), pair.second.products.end());
    }

    // Add sort operation here
    sort(all_products.begin(), all_products.end(), [](const Product& a, const Product& b) {
        return a.price < b.price || (a.price == b.price && a.name < b.name);
    });

    // Add erase operation here
    all_products.erase(unique(all_products.begin(), all_products.end(), [](const Product& a, const Product& b) {
        return a.name == b.name;
    }), all_products.end());

    return vector<Product>(all_products.begin(), all_products.begin() + min(5, (int)all_products.size()));
}

// Function to write the output to a file
void write_output(string filename, string city, vector<Product>& products, map<string, City>& cities) {
    ofstream file(filename);
    file << city << " " << fixed << setprecision(2) << cities[city].total_price << "\n";
    for (auto& product : products) {
        file << product.name << " " << fixed << setprecision(2) << product.price << "\n";
    }
    file.close();
}

int main() {
    map<string, City> cities;

    read_input("input.txt", cities);

    string cheapest_city = find_cheapest_city(cities);
    vector<Product> cheapest_products = find_cheapest_products(cities);

    write_output("output.txt", cheapest_city, cheapest_products, cities);

    return 0;
}
