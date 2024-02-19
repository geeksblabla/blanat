
/*
Mohamed Lahyane (Anir_Ln)
C++17
g++ -std=c++17 solution.cpp -o a.out
./a.out
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

const char *INPUT = "input.txt";
const char *OUTPUT = "output.txt";

string line;
string city, product;
double price;

unordered_map<string, double> cities;
unordered_map<string, double> products;

void processChunk(const string &chunk)
{
	istringstream is(chunk);
	while (getline(is, line))
	{
		istringstream iss(line);
		getline(iss, city, ',');
		getline(iss, product, ',');
		iss >> price;
		cities[city] += price;
		auto it = products.find(product);
		if (it != products.end())
		{
			it->second = min(it->second, price);
		}
		else
		{
			products[product] = price;
		}
	}
}

void solve()
{
	cout << fixed << setprecision(2);
	ifstream file(INPUT, ios::binary);
	if (!file)
	{
		cerr << "Cannot open file!\n";
		return;
	}

	const size_t bufferSize =  1024 * 1024; // 1 MB buffer size
	vector<char> buffer(bufferSize + 1);
	string leftover;

	while (file.read(buffer.data(), bufferSize) || file.gcount() > 0)
	{
		buffer[file.gcount()] = '\0';
		string chunk = leftover + string(buffer.data());

		size_t lastNewline = chunk.rfind('\n');
		if (lastNewline == string::npos)
		{
			leftover = chunk;
			continue;
		}

		processChunk(chunk.substr(0, lastNewline));

		leftover = chunk.substr(lastNewline + 1);
	}

	// process remaining data after the last chunk
	if (!leftover.empty())
	{
		processChunk(leftover);
	}

	// get results
	auto cheapest_city = min_element(all(cities), [](const auto &a, const auto &b)
																	 { return a.second < b.second; });
	vector<pair<string, double>> products_vec(all(products));
	sort(all(products_vec), [](const pair<string, double> &a, const pair<string, double> &b)
			 { return a.second == b.second ? a.first < b.first : a.second < b.second; });

	// print results
	freopen(OUTPUT, "w", stdout);
	cout.tie(0);
	cout << cheapest_city->first << sp << cheapest_city->second << ln;
	for (auto prod = products_vec.begin(); prod != products_vec.begin() + 5; prod++)
	{
		cout << prod->first << sp << prod->second << ln;
	}
}

int main()
{
	// auto start = chrono::high_resolution_clock::now();
	solve();
	// auto end = chrono::high_resolution_clock::now();
	// chrono::duration<double> duration = end - start;
	// cout << "duration: " << duration.count() << endl;
	return 0;
}
