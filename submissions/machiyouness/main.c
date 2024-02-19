// X account: @machiyouness
// Note: If I win, please send the prize to @laytoun, wah alyam wah

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

struct Product {
    char *name;
    float price;
};

struct City {
    char *name;
    float totalPrice;
    struct Product *products;
    int productCount;
    int productCapacity;
};

int compare_product_prices(const void *a, const void *b) {
    const struct Product *productA = (const struct Product *)a;
    const struct Product *productB = (const struct Product *)b;
    
    if (productA->price < productB->price) return -1;
    if (productA->price > productB->price) return 1;
    return 0;
}

void cheapest_city(const char* file_path) {
    struct City *cities = NULL;
    int cityCount = 0;

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("file not found.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        char *city = strtok(line, ",");
        char *product = strtok(NULL, ",");
        float price = atof(strtok(NULL, ","));

        int city_index = -1;
        for (int i = 0; i < cityCount; i++) {
            if (strcmp(cities[i].name, city) == 0) {
                city_index = i;
                break;
            }
        }

        if (city_index == -1) {
            cities = realloc(cities, (cityCount + 1) * sizeof(struct City));
            cities[cityCount].name = strdup(city);
            cities[cityCount].totalPrice = 0;
            cities[cityCount].productCount = 0;
            cities[cityCount].productCapacity = 10;
            cities[cityCount].products = malloc(cities[cityCount].productCapacity * sizeof(struct Product));
            city_index = cityCount;
            cityCount++;
        }

        int productIndex = -1;
        for (int i = 0; i < cities[city_index].productCount; i++) {
            if (strcmp(cities[city_index].products[i].name, product) == 0) {
                productIndex = i;
                break;
            }
        }

        if (productIndex == -1) {
            if (cities[city_index].productCount >= cities[city_index].productCapacity) {
                cities[city_index].productCapacity *= 2;
                cities[city_index].products = realloc(cities[city_index].products, cities[city_index].productCapacity * sizeof(struct Product));
            }
            cities[city_index].products[cities[city_index].productCount].name = strdup(product);
            cities[city_index].products[cities[city_index].productCount].price = price;
            cities[city_index].productCount++;
        } else {
            if (price < cities[city_index].products[productIndex].price) {
                cities[city_index].products[productIndex].price = price;
            }
        }

        cities[city_index].totalPrice += price;
    }

    fclose(file);

    int cheapestcity_index = 0;
    for (int i = 1; i < cityCount; i++) {
        if (cities[i].totalPrice < cities[cheapestcity_index].totalPrice) {
            cheapestcity_index = i;
        }
    }

    qsort(cities[cheapestcity_index].products, cities[cheapestcity_index].productCount, sizeof(struct Product), compare_product_prices);

    FILE *outputFile = fopen("output.txt", "w");

    fprintf(outputFile, "%s %.2f\n", cities[cheapestcity_index].name, cities[cheapestcity_index].totalPrice);
    for (int i = 0; i < cities[cheapestcity_index].productCount && i < 5; i++) {
        fprintf(outputFile, "%s %.2f\n", cities[cheapestcity_index].products[i].name, cities[cheapestcity_index].products[i].price);
    }

    fclose(outputFile);

    for (int i = 0; i < cityCount; i++) {
        free(cities[i].name);
        for (int j = 0; j < cities[i].productCount; j++) {
            free(cities[i].products[j].name);
        }
        free(cities[i].products);
    }
    free(cities);
}

int main() {
    cheapest_city("input.txt");
    return 0;
}
