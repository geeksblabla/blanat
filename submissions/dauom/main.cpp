#include <iostream>
#include <fstream>
#include <string>
#include <cstring> // For strtok & memcpy
#include <cstdio>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <iomanip>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> // For close()
#include <cstring>  // For  
#include <thread>
#include <future>

using namespace std;

const char* INPUT_FILENAME = "input.txt";
const char* OUTPUT_FILENAME = "output.txt";
const int BLOCK_SIZE = 10 * 4096;
const int NUM_THREADS = thread::hardware_concurrency();
int total = 0;

inline void handle_error(const char* msg) {
    perror(msg); 
    exit(255);
}

struct MappedFile {
    const int fd;
    const size_t file_size;
    const char* file_data;
};

struct Result {
    unordered_map<string, unordered_map<string, double>> product_cost;
    unordered_map<string, double> city_cost;
};

inline const MappedFile map_input() {
    int fd = open(INPUT_FILENAME, O_RDONLY);
    if (fd == -1) handle_error("map_input: open failed");

    struct stat sb;
    if (fstat(fd, &sb) == -1) handle_error("map_input: fstat failed");

    const char* addr = static_cast<const char*>(mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0u));
    if (addr == MAP_FAILED) handle_error("map_input: mmap failed");
    madvise((void *)addr, 0, MADV_SEQUENTIAL);

    return {fd, sb.st_size, addr};
}

const string consume_str(char* &start) {
    stringstream ss;
    char c;
    while ((c = *start) != 0 && c != ',' && c != '\n') {
        ss << c;
        start++;
    }
    start++;
    return ss.str();
}

Result process_chunk(char * start, char * end) {
    if (*start != '\n') {
        start = (char *) rawmemchr(start, '\n');
    }
    start++;

    unordered_map<string, unordered_map<string, double>> product_cost;
    unordered_map<string, double> city_cost;
    char *cur = start;
    while (cur < end) {
        string city = consume_str(cur);
        string product = consume_str(cur);
        string price = consume_str(cur);
        double dprice = stod(price);
        if (product_cost[city].find(product) == product_cost[city].end()) {
            product_cost[city][product] = dprice;
        } else {
            product_cost[city][product] = min(product_cost[city][product], dprice);
        }
        city_cost[city] += dprice;
    }

    return {product_cost, city_cost};
}

vector<Result> process_concurrently(const MappedFile &mp) {
    char* start = (char*) mp.file_data;
    char* end = start + mp.file_size;
    const int block_size = mp.file_size / NUM_THREADS;

    vector<future<Result>> future_results;
    for (int i = 0; i < NUM_THREADS; i++) {
        future_results.emplace_back(async(process_chunk, start + i * block_size, min(start + (i + 1) * block_size, end)));
    }

    vector<Result> results;
    for (auto& fr : future_results) {
        results.emplace_back(fr.get());
    }
    return results;
}

Result merge(vector<Result> &results) {
    unordered_map<string, unordered_map<string, double>> product_cost;
    unordered_map<string, double> city_cost;
    for (auto &r : results) {
        for (auto &cc : r.city_cost) {
            city_cost[cc.first] += cc.second;
        }
        for (auto &pc : r.product_cost) {
            string city = pc.first;
            for (auto &ppc : pc.second) {
                string product = ppc.first;
                if (product_cost[city].find(product) == product_cost[city].end()) {
                    product_cost[city][product] = ppc.second;
                } else {
                    product_cost[city][product] = min(product_cost[city][product], ppc.second);
                }
            }
        }
    }
    return {product_cost, city_cost};
}

void ans(Result &result) {
    FILE *f = fopen(OUTPUT_FILENAME, "w");
    
    double min_city_cost = 4e18;
    string city = "";
    for (auto &cc : result.city_cost) {
        if (cc.second < min_city_cost) {
            min_city_cost = cc.second;
            city = cc.first;
        } else if (cc.second == min_city_cost && cc.first < city) {
            city = cc.first;
        }
    }
    fprintf(f, "%s %.2f\n", city.c_str(), min_city_cost);

    vector<pair<double, string>> products;
    for (auto &pc : result.product_cost[city]) {
        products.push_back({pc.second, pc.first});
    }
    sort(products.begin(), products.end());
    products.resize(5);

    for (auto &p : products) {
        fprintf(f, "%s %.2f\n", p.second.c_str(), p.first);
    }
    fclose(f);
}

int main() {
    const MappedFile mp = map_input();
    
    vector<Result> results = process_concurrently(mp);
    Result result = merge(results);
    ans(result);

    // Unmap the memory and close the file
    munmap((void *) mp.file_data, mp.file_size);  
    close(mp.fd);
    return 0;
}
