#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

struct Product {
  string name;
  double price;
};

struct CityData {
  string name;
  double totalPrice;
  vector<Product> products;
};

// Function to read the input file and populate the data structures
void processInputData(string filename, unordered_map<string, CityData>& cityMap) {
  ifstream file(filename);
  string line;
  while (getline(file, line)) {
    stringstream ss(line);
    string cityName, productName, priceStr;
    getline(ss, cityName, ',');
    getline(ss, productName, ',');
    getline(ss, priceStr, ',');
    double price = stod(priceStr);

    // Update city price directly within the map
    cityMap[cityName].totalPrice += price;

    // Efficiently manage unique products using a set
    auto& productList = cityMap[cityName].products;
    if (find_if(productList.begin(), productList.end(),
                [&productName, &price](const Product& p) { return p.name == productName && p.price == price; }) == productList.end()) {
      productList.push_back({productName, price});
    }
  }
  file.close();
}

// Function to find the city with the lowest total price
string findLeastExpensiveCity(unordered_map<string, CityData>& cityMap) {
  string leastExpensiveCity;
  double minTotalPrice = numeric_limits<double>::max();
  for (auto& pair : cityMap) {
    if (pair.second.totalPrice < minTotalPrice) {
      minTotalPrice = pair.second.totalPrice;
      leastExpensiveCity = pair.first;
    }
  }
  return leastExpensiveCity;
}

// Function to find the five cheapest products (optimized for efficiency)
vector<Product> findFiveCheapestProducts(unordered_map<string, CityData>& cityMap) {
  // Combine product finding and sorting in a single loop
  vector<Product> allProducts;
  for (auto& pair : cityMap) {
    for (auto& product : pair.second.products) {
      auto it = find_if(allProducts.begin(), allProducts.end(),
                         [&product](const Product& p) { return p.name == product.name; });
      if (it == allProducts.end()) {
        allProducts.push_back(product);
      } else {
        it->price = min(it->price, product.price);
      }
    }
  }

  // Sort based on price and name directly within the vector
  sort(allProducts.begin(), allProducts.end(), [](const Product& a, const Product& b) {
    return a.price < b.price || (a.price == b.price && a.name < b.name);
  });

  return vector<Product>(allProducts.begin(), allProducts.begin() + min(5, (int)allProducts.size()));
}

// Function to write the output to a file
void writeOutput(string filename, string city, vector<Product>& products, unordered_map<string, CityData>& cityMap) {
  ofstream file(filename);
  file << city << " " << fixed << setprecision(2) << cityMap[city].totalPrice << "\n";
  for (auto& product : products) {
    file << product.name << " " << fixed << setprecision(2) << product.price << "\n";
  }
  file.close();
}

int main() {
  unordered_map<string, CityData> cityMap;

  processInputData("input.txt", cityMap);

  string leastExpensiveCity = findLeastExpensiveCity(cityMap);
  vector<Product> fiveCheapestProducts = findFiveCheapestProducts(cityMap);

  writeOutput("output.txt", leastExpensiveCity, fiveCheapestProducts, cityMap);

  return 0;
}

