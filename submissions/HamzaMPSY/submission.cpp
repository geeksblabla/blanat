#include <bits/stdc++.h>
using namespace std;


class ProductsSet
{
    public:
        ProductsSet(){};
        ~ProductsSet(){};
        bool add(pair<string, double>);
        int size();
        set<pair<string, double>>::iterator begin();
        set<pair<string, double>>::iterator end();
        set<pair<string, double>> products;
};

bool ProductsSet::add(pair<string, double> product)
{
    auto it = products.lower_bound(std::make_pair(product.first, 0)); // Find element with same first value or the next greater one
    // printf("Trying (%s,%lf), found (%s,%lf)\n",product.first.c_str(),product.second,it->first.c_str(), it->second);
    if (it != products.end() && it->first == product.first) {
        if (product.second <= it->second) {
            products.erase(it); // Remove existing pair
            //printf("Inserted\n");
            return products.insert(product).second; // Insert new pair
        }
        return false;
    } else {
        // printf("Inserted\n");
        return products.insert(product).second; // Insert new pair
    }
}

int ProductsSet::size()
{
    return products.size();
}

set<pair<string, double>>::iterator ProductsSet::begin()
{
    return products.begin();
}

set<pair<string, double>>::iterator ProductsSet::end()
{
    return products.end();
}

char city[100],product[100];
double price;
map<string, ProductsSet> products;
map<string, double> sums;

int main(int argc, const char** argv) {
    freopen("input.txt","r", stdin);
    freopen("output.txt","w", stdout);
    while(scanf("%[^,],%[^,],%lf\n",city, product, &price) == 3){
        products[city].add(make_pair(product,price));
        if(sums.count(city) == 0)
            sums[city] = price;
        else
            sums[city] += price;
    }


    double minVal = numeric_limits<double>::max(); // Initialize minVal to maximum possible value
    string minKey;

    for (const auto& pair : sums) {
        if (pair.second < minVal) {
            minVal = pair.second;
            minKey = pair.first;
        }
    }

    // Multiset of pairs (double, string)
    multiset<pair<double, string>> ans;

    // Iterate through the original set and insert into the multiset with the appropriate ordering
    for (const auto& pair : products[minKey].products) {
        ans.insert(make_pair(pair.second, pair.first));
    }

    printf("%s %.2lf\n",minKey.c_str(),sums[minKey]);
    int i = 0;
    for(pair<double, string> p : ans){
        printf("%s %.2lf\n",p.second.c_str(), p.first);
        i++;
        if(i >= 5)
            break;
    }
    return 0;
}