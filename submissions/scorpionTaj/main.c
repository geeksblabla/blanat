#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define NUM_CITIES 101
#define NUM_PRODUCTS 94
#define MAX_LINE_LENGTH 100
#define MAX_PRODUCT_NAME_LENGTH 100
#define MAX_PRODUCTS 5

// Define a structure for product information
struct Product {
    const char *name;
    int price;
};

// Define a structure for city information
struct City {
    const char *name;
    int64_t total_price;
    struct Product cheapest_products[MAX_PRODUCTS];
};

// Hash function to generate indices
int hash(const char *str) {
    int hash = 0;
    while (*str) {
        hash += *str++;
    }
    return hash % NUM_CITIES;
}

int main() {
    FILE *input_file = fopen("input.txt", "r");
    FILE *output_file = fopen("output.txt", "w");

    // Ensure files are opened successfully
    if (!input_file || !output_file) {
        fprintf(stderr, "Error opening files.\n");
        return 1;
    }

    // Define hash tables for city and product lookup
    struct City cities[NUM_CITIES] = {{0}};
    struct Product products[NUM_PRODUCTS] = {{0}};

    char line[MAX_LINE_LENGTH];
    char city[MAX_PRODUCT_NAME_LENGTH], product[MAX_PRODUCT_NAME_LENGTH];
    float price;

    // Read prices from the input file
    while (fgets(line, sizeof(line), input_file)) {
        if (sscanf(line, "%[^,],%[^,],%f", city, product, &price) == 3) {
            // Find city index
            int city_idx = hash(city);
            if (cities[city_idx].name == NULL) {
                cities[city_idx].name = strdup(city);
            }

            // Find product index
            int product_idx = hash(product);
            if (products[product_idx].name == NULL) {
                products[product_idx].name = strdup(product);
            }

            // Update city data
            cities[city_idx].total_price += price * 100;

            // Update cheapest products for the city
for (int i = 0; i < MAX_PRODUCTS; ++i) {
    if (cities[city_idx].cheapest_products[i].name != NULL && strcmp(cities[city_idx].cheapest_products[i].name, products[product_idx].name) == 0) {
        // If the product already exists in the list, update its price if the new price is smaller
        if (price * 100 < cities[city_idx].cheapest_products[i].price) {
            cities[city_idx].cheapest_products[i].price = price * 100;
        }
        break; // Exit the loop since we've updated the price for the existing product
    }
    // If the product doesn't exist in the list and there's space, add it
    if (cities[city_idx].cheapest_products[i].name == NULL || price * 100 < cities[city_idx].cheapest_products[i].price) {
        // Shift cheaper products down
        for (int j = MAX_PRODUCTS - 1; j > i; --j) {
            cities[city_idx].cheapest_products[j] = cities[city_idx].cheapest_products[j - 1];
        }
        cities[city_idx].cheapest_products[i].name = products[product_idx].name;
        cities[city_idx].cheapest_products[i].price = price * 100;
        break;
    }
}

        } else {
            fprintf(stderr, "Error parsing line: %s\n", line);
        }
    }

    // Find the cheapest city
    int cheapest_city_idx = -1;
    int64_t cheapest_city_price = INT64_MAX;
    for (int i = 0; i < NUM_CITIES; ++i) {
        if (cities[i].total_price > 0 && cities[i].total_price < cheapest_city_price) {
            cheapest_city_price = cities[i].total_price;
            cheapest_city_idx = i;
        }
    }

    // Print the cheapest city and its 5 cheapest products
    fprintf(output_file, "%s %.2f\n", cities[cheapest_city_idx].name, (double)cheapest_city_price / 100.0);
    for (int i = 0; i < MAX_PRODUCTS && cities[cheapest_city_idx].cheapest_products[i].name != NULL; ++i) {
        fprintf(output_file, "%s %.2f\n", cities[cheapest_city_idx].cheapest_products[i].name, (double)cities[cheapest_city_idx].cheapest_products[i].price / 100.0);
    }

    // Close files and free allocated memory
    fclose(input_file);
    fclose(output_file);

    for (int i = 0; i < NUM_CITIES; ++i) {
        free((void *)cities[i].name);
    }

    for (int i = 0; i < NUM_PRODUCTS; ++i) {
        free((void *)products[i].name);
    }

    return 0;
}
