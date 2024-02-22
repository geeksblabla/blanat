// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>
// #include <algorithm>
// #include <map>
// #include <vector>

// using namespace std;

// int main() {
//     ifstream inputFile("../../input.txt");
//     if (!inputFile) {
//         cout << "Failed to open input.txt" << endl;
//         return 1;
//     }

//     string line;
//     map<string, double> city_costs;
//     while (getline(inputFile, line)) {
//         stringstream ss(line);
//         string city, product, price;
//         getline(ss, city, ',');
//         getline(ss, product, ',');
//         getline(ss, price, ',');
//         double cost = stof(price);

//         auto result = city_costs.insert({city, cost});
//         if (!result.second) // if the insertion did not take place, the city is already in the map
//             result.first->second += cost; // add the cost to the existing cost
//     }
//     map<string, double> sorted_city_costs;
//     for (map<string, double>::iterator it = city_costs.begin(); it != city_costs.end(); it++)
//     {
//         sorted_city_costs.insert({it->first, it->second});
//     }
    
//     for (map<string, double>::iterator it = sorted_city_costs.begin(); it != sorted_city_costs.end(); it++)
//     {
//         cout << it->first << ": " << fixed << it->second << endl;
//     }

//     inputFile.close();
//     return 0;
// }


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <queue>

using namespace std;

struct Compare {
    bool operator()(pair<string, double> const& p1, pair<string, double> const& p2) {
        // return "true" if "p1" is ordered before "p2", for example:
        return p1.second > p2.second;
    }
};

int main() {
    ifstream inputFile("../../input.txt");
    if (!inputFile) {
        cout << "Failed to open input.txt" << endl;
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
        auto product_costs_insertion = product_costs.insert({product, cost});
         if (!product_costs_insertion.second) // if the insertion did not take place, the city is already in the map
            product_costs_insertion.first->second = min(product_costs_insertion.first->second, cost);
    }

    // Create a min heap
    priority_queue<pair<string, double>, vector<pair<string, double>>, Compare> sheapest_cities;

    // Insert all elements into the min heap
    for (auto& pair : city_costs) {
        sheapest_cities.push(pair);
    }

    // Print the elements in ascending order of cost
    while (!sheapest_cities.empty()) {
        cout << sheapest_cities.top().first << ": " << fixed << sheapest_cities.top().second << endl;
        sheapest_cities.pop();
    }

    cout << "*******************************************************" << endl;
    priority_queue<pair<string, double>, vector<pair<string, double>>, Compare> sheapest_products;

     // Insert all elements into the min heap
    for (auto& pair : product_costs) {
        sheapest_products.push(pair);
    }
    // Print the elements in ascending order of cost
    while (!sheapest_products.empty()) {
        cout << sheapest_products.top().first << ": " << fixed << sheapest_products.top().second << endl;
        sheapest_products.pop();
    }

    inputFile.close();
    return 0;
}