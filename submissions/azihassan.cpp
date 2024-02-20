#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>

using namespace std;

int main(void)
{
    ifstream input("../input.txt");
    string line;
    unordered_map<string, double> cityCosts;
    unordered_map<string, unordered_set<string>> products;
    unordered_map<string, double> productCosts;


    getline(input, line); //skip header
    while(getline(input, line))
    {
        string city;
        string product;
        double price;

        stringstream splitter(line);
        getline(splitter, city, ',');
        getline(splitter, product, ',');
        string tmp;
        getline(splitter, tmp, ',');
        price = stod(tmp);

        cityCosts[city] += price;
        string productKey = city + "_" + product;
        products[city].insert(product);
        auto existingProduct = productCosts.find(productKey);
        if(existingProduct == productCosts.end())
        {
            productCosts[productKey] = price;
        }
        else
        {
            productCosts[productKey] = min(price, productCosts[productKey]);
        }
    }

    string cheapestCity = cityCosts.begin()->first;
    for(const auto kv: cityCosts)
    {
        string city = kv.first;
        double cost = kv.second;
        if(cost < cityCosts[cheapestCity])
        {
            cheapestCity = city;
        }
    }
    
    auto comparator = [&cheapestCity, &productCosts](string a, string b) {
        if(productCosts.at(cheapestCity + "_" + a) == productCosts.at(cheapestCity + "_" + b))
        {
            return a.compare(b) < 0;
        }
        return productCosts.at(cheapestCity + "_" + a) <= productCosts.at(cheapestCity + "_" + b);
    };
    set<string, decltype(comparator)> cheapestProducts(products[cheapestCity].begin(), products[cheapestCity].end(), comparator);
    cout << cheapestCity << ' ' << fixed << setprecision(2) << cityCosts[cheapestCity] << endl;
    int i = 0;
    for(string product: cheapestProducts)
    {
        cout << product << ' ' << fixed << setprecision(2) << productCosts[cheapestCity + "_" + product] << endl;
        if(++i == 5)
        {
            break;
        }
    }
}
