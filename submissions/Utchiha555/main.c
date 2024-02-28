#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000
#define MAX_CITIES 101
#define MAX_NAME_LENGTH 50

typedef struct {
    char name[MAX_NAME_LENGTH];
    double price;
} Product;

typedef struct {
    char name[MAX_NAME_LENGTH];
    double total_price;
    Product cheapest_products[5]; // Store the first 5 cheapest products
} City;

int main() {
    FILE *input_file = fopen("input.txt", "r");
    if (input_file == NULL)
    {
        printf("cannot open file: input.txt\n");
        return 1;
    }
    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL)
    {
        printf("cannot open file: output.txt\n");
        return 1;
    }
    City cities[MAX_CITIES] = {0};
    char line[MAX_LINE_LENGTH];

    // Process each line in the input file
    while (fgets(line, sizeof(line), input_file)) {
        char city_name[MAX_NAME_LENGTH];
        char product_name[MAX_NAME_LENGTH];
        double price;

        // Parse the line to extract city name, product name, and price
        sscanf(line, "%[^,],%[^,],%lf", city_name, product_name, &price);

        // Find or create the city entry
        int city_index = -1;
        for (int i = 0; i < MAX_CITIES; i++) {
            if (strcmp(cities[i].name, city_name) == 0) {
                city_index = i;
                break;
            } else if (cities[i].name[0] == '\0') {
                city_index = i;
                strcpy(cities[i].name, city_name);
                break;
            }
        }

        // Update total price for the city
        cities[city_index].total_price += price;

        // Update cheapest products for the city
        for (int i = 0; i < 5; i++) {
            if (cities[city_index].cheapest_products[i].name[0] == '\0' || price < cities[city_index].cheapest_products[i].price) {
                for (int j = 4; j > i; j--) {
                    strcpy(cities[city_index].cheapest_products[j].name, cities[city_index].cheapest_products[j - 1].name);
                    cities[city_index].cheapest_products[j].price = cities[city_index].cheapest_products[j - 1].price;
                }
                strcpy(cities[city_index].cheapest_products[i].name, product_name);
                cities[city_index].cheapest_products[i].price = price;
                break;
            } else if (price == cities[city_index].cheapest_products[i].price && strcmp(product_name, cities[city_index].cheapest_products[i].name) < 0) {
                for (int j = 4; j > i; j--) {
                    strcpy(cities[city_index].cheapest_products[j].name, cities[city_index].cheapest_products[j - 1].name);
                    cities[city_index].cheapest_products[j].price = cities[city_index].cheapest_products[j - 1].price;
                }
                strcpy(cities[city_index].cheapest_products[i].name, product_name);
                cities[city_index].cheapest_products[i].price = price;
                break;
            }
        }
    }

    // Find the cheapest city
    int cheapest_city_index = -1;
    double cheapest_total_price = -1;
    for (int i = 0; i < MAX_CITIES; i++) {
        if (cities[i].name[0] != '\0' && (cheapest_total_price == -1 || cities[i].total_price < cheapest_total_price)) {
            cheapest_total_price = cities[i].total_price;
            cheapest_city_index = i;
        }
    }

    // Write the results to the output file
    fprintf(output_file, "%s %.2lf\n", cities[cheapest_city_index].name, cities[cheapest_city_index].total_price);
    for (int i = 0; i < 5; i++) {
        if (cities[cheapest_city_index].cheapest_products[i].name[0] != '\0') {
            fprintf(output_file, "%s %.2lf\n", cities[cheapest_city_index].cheapest_products[i].name, cities[cheapest_city_index].cheapest_products[i].price);
        }
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}
