#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <map>
#include <vector>
#include <queue>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct Compare {
    bool operator()(pair<string, double> const& p1, pair<string, double> const& p2) {
        // return "true" if "p1" is ordered before "p2", for example:
        return p1.second > p2.second;
    }
};

// Custom comparison function
bool comparePairs(const pair<string, double> &a, const pair<string, double> &b) {
    // First, compare by price
    if (a.second != b.second) {
        return a.second < b.second; // Sort by price
    }
    // If prices are equal, sort alphabetically by product name
    return a.first < b.first;
}

int main() {

    // auto startTime = high_resolution_clock::now(); // Declare and initialize startTime
    
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cout << "Failed to open input.txt" << endl;
        return 1;
    }

    std::ofstream outputFile("output.txt");
    if (!outputFile) {
        cout << "Failed to open output.txt" << endl;
        return 1;
    }
    
    string line;
    map<string, double> city_costs;
    map<string, double> product_costs;

    while (getline(inputFile, line)) {
        stringstream ss(line);
        string city, product, price;
        getline(ss, city, ',');
        getline(ss, product, ',');
        getline(ss, price, ',');
        double cost = stof(price);

        auto city_costs_insertion = city_costs.insert({city, cost});
        if (!city_costs_insertion.second) // if the insertion did not take place, the city is already in the map
            city_costs_insertion.first->second += cost; // add the cost to the existing cost
    }

    // Create a min heap
    priority_queue<pair<string, double>, vector<pair<string, double>>, Compare> sheapest_cities;

    // Insert all elements into the min heap
    for (auto& pair : city_costs) {
        sheapest_cities.push(pair);
    }

    outputFile << sheapest_cities.top().first << " " << fixed << setprecision(2) << sheapest_cities.top().second << endl; //print the sheapest city that is in the top

    // Seek back to the beginning of the file
    inputFile.clear(); // Clear any error flags
    inputFile.seekg(0, ios::beg); // Move the file pointer to the beginning

    while (getline(inputFile, line)) {
        stringstream ss(line);
        string city, product, price;
        getline(ss, city, ',');
        getline(ss, product, ',');
        getline(ss, price, ',');
        double cost = stof(price);
        if (city == sheapest_cities.top().first) {
            auto product_costs_insertion = product_costs.insert({product, cost});
            if (!product_costs_insertion.second) // if the insertion did not take place, the product is already in the map
               product_costs_insertion.first->second = min(product_costs_insertion.first->second, cost); // update with the sheapest cost
        }
    }

    priority_queue<pair<string, double>, vector<pair<string, double>>, Compare> sheapest_products;

     // Insert all elements into the min heap
    for (auto& pair : product_costs) {
        sheapest_products.push(pair);
    }

    vector<pair<string, double>> products;
    for (size_t i = 0; i < 5 && !sheapest_products.empty(); i++) {
        products.push_back({sheapest_products.top().first, sheapest_products.top().second});
        sheapest_products.pop();
    }

    sort(products.begin(), products.end(), comparePairs); //sort equal cost products by alpha
    // Print the elements in ascending order of cost

     for (auto& product : products) {
        outputFile << product.first << " " << fixed << setprecision(2) << product.second << endl;
    }

    // Calculate execution time
    // auto endTime = high_resolution_clock::now();
    // auto duration = duration_cast<milliseconds>(endTime - startTime);
    // cout << "Execution time: " << duration.count() << " milliseconds" << endl;

    return 0;
}