
/*
Mohamed Lahyane (Anir_Ln)
C++17
*/

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>

using namespace std;

#define ln "\n"
#define sp ' '
#define all(x) (x).begin(), (x).end()

void prep() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	ios::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
}



void solve() {
	string line;
	// skip first line: csv header
	getline(cin, line);

	string city, product;
	double price;

	unordered_map<string, double> cities;
	unordered_map<string, double> products;
	while(getline(cin, line)) {
		istringstream iss(line);			
		getline(iss, city, ',');
		getline(iss, product, ',');
		iss >> price;
		cities[city] += price;
		auto it = products.find(product);
		if (it != products.end()) {
			it->second = min(it->second, price);
		} else {
			products[product] = price;
		}
	}
	
	// get restuls
	auto cheapest_city = min_element(all(cities), [](const auto& a, const auto& b) { 
			return a.second < b.second;
	});
	vector<pair<string, double>> products_vec(all(products));
	sort(all(products_vec), [](const pair<string, double>& a, const pair<string, double>& b) {
		if (a.second == b.second) {
			return a.first < b.first;
		}
		return a.second < b.second;
	});

	// print results
	cout << cheapest_city->first << sp << cheapest_city->second << ln;
	for (auto prod = products_vec.begin(); prod != products_vec.begin() + 5; prod++) {
		cout << prod->first << sp << prod->second << ln;
	}
}


int main() {
	prep();
	cout << fixed << setprecision(2);
	solve();
	return 0;
}

