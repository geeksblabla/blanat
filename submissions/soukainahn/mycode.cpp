#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Product {
    string name;
    double price;

    bool operator<(const Product& other) const {
        if (price == other.price) {
            return name < other.name;
        }
        return price < other.price;
    }
};

int main() {
    ifstream file("prices.csv");
    string line;
    unordered_map<string, vector<Product>> products;

    while (getline(file, line)) {
        size_t comma = line.find(',');
        string city = line.substr(0, comma);
        string product = line.substr(comma + 1, line.find(',', comma + 1) - comma - 1);
        double price = stod(line.substr(line.find(',', comma + 1) + 1));

        if (price >= 1.00 && price <= 100.00) {
            products[city].push_back({product, price});
        }
    }

    string cheapestCity;
    double cheapestTotalPrice = numeric_limits<double>::max();
    vector<Product> cheapestProducts;

    for (const auto& cityProducts : products) {
        double totalPrice = 0;
        vector<Product> sortedProducts;

        for (const auto& product : cityProducts.second) {
            totalPrice += product.price;
            sortedProducts.push_back(product);
        }

        if (totalPrice < cheapestTotalPrice) {
            cheapestCity = cityProducts.first;
            cheapestTotalPrice = totalPrice;
            cheapestProducts = sortedProducts;
        } else if (totalPrice == cheapestTotalPrice) {
            for (size_t i = 0; i < min(cheapestProducts.size(), size_t(5)); ++i) {
                if (i >= sortedProducts.size()) {
                    break;
                }
                if (sortedProducts[i].price < cheapestProducts[i].price) {
                    cheapestProducts = {sortedProducts.begin(), sortedProducts.begin() + 5};
                    break;
                } else if (sortedProducts[i].price > cheapestProducts[i].price) {
                    break;
                }
            }
        }
    }

    cout << cheapestCity << " " << fixed << setprecision(2) << cheapestTotalPrice << endl;

    for (size_t i = 0; i < min(cheapestProducts.size(), size_t(5)); ++i) {
        cout << cheapestProducts[i].name << " " << fixed << setprecision(2) << cheapestProducts[i].price << endl;
    }

    return 0;
}
