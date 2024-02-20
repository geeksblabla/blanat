#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <cstring>
using namespace std;



void process_chunk(const string &chunk)
{
    istringstream is(chunk);
    string line;
    string city, product;
    double price;

    while (getline(is, line))
        {
            istringstream iss(line);
            getline(iss, city, ',');
            getline(iss, product, ',');
            iss >> price;
            cout << city << " " << product << " " << price << std::endl;
        }
    }

void blanat()
{
    ifstream file("input.txt", ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file: " << strerror(errno) << endl;
        return;
    }

    constexpr size_t bufferSize = 1024 * 1024;
    unique_ptr<char[]> buffer(new char[bufferSize]);

    while (file)
    {
        file.read(buffer.get(), bufferSize);
        process_chunk(buffer.get());
    }

    file.close();
}

int main()
{
    cout << "Hello, World!" << endl;
    blanat();
    return 0;
}