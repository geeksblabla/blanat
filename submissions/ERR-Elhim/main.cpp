#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <unordered_map>
#include <algorithm>
#include <atomic>
#include <limits>
#include <mutex> // Include mutex header

std::mutex mtx; // Declare mutex variable

// Define struct for city data
struct CityData {
    double total_price;
    std::unordered_map<std::string, double> cheapest_products;
};

// Custom comparison function for sorting products by price and then by name
struct ProductCompare {
    bool operator()(const std::pair<std::string, double>& p1, const std::pair<std::string, double>& p2) const {
        if (p1.second == p2.second) {
            return p1.first < p2.first; // Sort alphabetically if prices are equal
        }
        return p1.second < p2.second; // Otherwise, sort by price
    }
};

// Function to process a batch of lines
void process_batch(const std::vector<std::string>& lines, std::unordered_map<std::string, CityData>& totals) {
    for (const auto& line : lines) {
        // Parse line and update city data
        std::istringstream iss(line);
        std::string city, product_name, price_str;
        if (std::getline(iss, city, ',') && std::getline(iss, product_name, ',') && std::getline(iss, price_str)) {
            try {
                double price = std::stod(price_str);
                // Lock mutex before accessing shared data
                std::lock_guard<std::mutex> lock(mtx);
                CityData& city_data = totals[city];
                city_data.total_price += price; // Update total price for the city
                // Update cheapest product if it's not present or the new price is lower
                if (!city_data.cheapest_products.count(product_name) || price < city_data.cheapest_products[product_name]) {
                    city_data.cheapest_products[product_name] = price;
                }
            } catch (...) {
                // Handle conversion error
                std::cerr << "Error converting price to double for line: " << line << std::endl;
            }
        } else {
            // Handle invalid line format
            std::cerr << "Invalid line format: " << line << std::endl;
        }
    }
}

int main() {
    const std::string filename = "input.txt";
    const int num_threads = std::thread::hardware_concurrency();
    const int batch_size = 1000000; // Adjust batch size as needed

    // Open file and check if it's opened successfully
    std::ifstream input(filename);
    if (!input.is_open()) {
        std::cerr << "Could not open input file.\n";
        return -1;
    }

    std::ofstream output_file("output.txt");
    if (!output_file.is_open()) {
        std::cerr << "Could not open output file.\n";
        return -1;
    }

    // Create atomic flag to track completion of file reading
    std::atomic<bool> file_read_complete(false);

    // Create a vector to store city data
    std::unordered_map<std::string, CityData> totals;

    // Create threads to process the file in parallel
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&] {
            std::vector<std::string> batch;
            std::string line;
            while (true) {
                {
                    std::lock_guard<std::mutex> lock(mtx); // Use lock_guard to lock mutex
                    if (!std::getline(input, line)) {
                        // End of file reached or error reading line
                        break;
                    }
                }
                batch.push_back(line);
                if (batch.size() >= batch_size) {
                    process_batch(batch, totals);
                    batch.clear();
                }
            }
            if (!batch.empty()) {
                process_batch(batch, totals);
            }
            file_read_complete = true;
        });
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }


    // Process any remaining lines in the file
    if (!file_read_complete) {
        std::vector<std::string> remaining_lines;
        std::string line;
        while (std::getline(input, line)) {
            remaining_lines.push_back(line);
        }
        process_batch(remaining_lines, totals);
    }

    // Find cheapest city and products
    std::string cheapest_city;
    double min_price = std::numeric_limits<double>::max();
    for (const auto& city_data : totals) {
        if (city_data.second.total_price < min_price) {
            min_price = city_data.second.total_price;
            cheapest_city = city_data.first;
        }
    }

    // Sort cheapest products by price and then by name if prices are equal
    std::vector<std::pair<std::string, double>> cheapest_products;
    for (const auto& product : totals[cheapest_city].cheapest_products) {
        cheapest_products.emplace_back(product);
    }
    std::sort(cheapest_products.begin(), cheapest_products.end(), ProductCompare());
  
    output_file << cheapest_city << " " << min_price << '\n';
    size_t  count = 0;
    for (const auto& product_price : cheapest_products) {
        if (count >= 5) break;
        output_file << product_price.first << ": " << product_price.second << '\n';
        ++count;
    }

    return 0;
}
