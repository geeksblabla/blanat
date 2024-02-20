#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Product
{
    string name;
    double price;
    Product(const string &n, double p) : name(n), price(p) {}
};

bool comp(const Product &a, const Product &b)
{
    return (a.price < b.price) || ((a.price == b.price) && (a.name < b.name));
}

int main()
{
    ifstream infile("input.txt");
    if (!infile)
    {
        cerr << "Failed to open input file!\n";
        return 1;
    }

    ofstream outfile("output.txt");
    if (!outfile)
    {
        cerr << "Failed to open output file!\n";
        return 1;
    }

    unordered_map<string, pair<double, vector<Product>>> cityData;

    const int batchSize = 1024 * 1024;
    string line;
    while (true)
    {
        vector<string> batch;
        batch.reserve(batchSize);
        for (int i = 0; i < batchSize && getline(infile, line); ++i)
        {
            batch.push_back(line);
        }
        if (batch.empty())
        {
            break;
        }
        for (const auto &line : batch)
        {
            stringstream ss(line);
            string city, product;
            double price;

            getline(ss, city, ',');
            getline(ss, product, ',');
            ss >> price;

            auto &data = cityData[city];
            data.first += price;
            data.second.emplace_back(product, price);
        }
    }

    string cheapestCity;
    double minTotalCost = numeric_limits<double>::max();
    for (const auto &city : cityData)
    {
        if (city.second.first < minTotalCost)
        {
            cheapestCity = city.first;
            minTotalCost = city.second.first;
        }
    }

    outfile << cheapestCity << " " << fixed << setprecision(2) << minTotalCost << "\n";

    sort(cityData[cheapestCity].second.begin(), cityData[cheapestCity].second.end(), comp);

    int count = 0;
    for (const auto &product : cityData[cheapestCity].second)
    {
        outfile << product.name << " " << fixed << setprecision(2) << product.price << "\n";
        if (++count >= 5)
            break;
    }

    return 0;
}
