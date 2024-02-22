#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iomanip>
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
         if (!product_costs_insertion.second) // if the insertion did not take place, the product is already in the map
            product_costs_insertion.first->second = min(product_costs_insertion.first->second, cost); // update with the sheapest cost
    }

    // Create a min heap
    priority_queue<pair<string, double>, vector<pair<string, double>>, Compare> sheapest_cities;

    // Insert all elements into the min heap
    for (auto& pair : city_costs) {
        sheapest_cities.push(pair);
    }

    cout << sheapest_cities.top().first << " " << fixed << setprecision(2) << sheapest_cities.top().second << endl; //print the sheapest city that is in the top
    // sheapest_cities.erase(); // erase the min heap
    priority_queue<pair<string, double>, vector<pair<string, double>>, Compare> sheapest_products;

     // Insert all elements into the min heap
    for (auto& pair : product_costs) {
        sheapest_products.push(pair);
    }
    vector<pair<string, double>> res;
    for (size_t i = 0; i < 5 && !sheapest_products.empty(); i++) {
        res.push_back({sheapest_products.top().first, sheapest_products.top().second});
        // cout << sheapest_products.top().first << " " << fixed << sheapest_products.top().second << endl;
        sheapest_products.pop();
    }
    // Print the elements in ascending order of cost

    for(vector<pair<string, double>>::iterator it = res.begin(); it != res.end(); it++)
    {
        if (it->second == (it+1)->second)
            iter_swap(it, it+1);
        cout << it->first << " " << fixed << setprecision(2) << it->second << endl;
    }
    
    inputFile.close();
    return 0;
}