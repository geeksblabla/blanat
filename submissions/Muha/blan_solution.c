#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CITIES 101
#define MAX_PRODUCTS 10000
#define CITY_HASH_TABLE_SIZE 2039  // Prime number larger than MAX_CITIES for a hash table
#define PRODUCT_HASH_TABLE_SIZE 2503  // Prime number for a hash table

typedef struct {
    char name[50];
    double price;
} Product;

typedef struct {
    char name[50];
    double totalPrice;
    int count;
    Product products[MAX_PRODUCTS];
} City;

typedef struct {
    City* cities[CITY_HASH_TABLE_SIZE];
    int count;
} CityHashTable;

unsigned int hashFunction(const char* str) {
    unsigned int hash = 5381;
    int c;
    while (c = *str++) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % CITY_HASH_TABLE_SIZE;
}

City* findCity(CityHashTable* table, const char* cityName) {
    unsigned int index = hashFunction(cityName);
    City* city = table->cities[index];
    // Collision resolution by linear probing
    while(city) {
        if(strcmp(city->name, cityName) == 0) {
            return city;
        }
        index = (index + 1) % CITY_HASH_TABLE_SIZE;
        city = table->cities[index];
    }
    return NULL;
}

City* getOrAddCity(CityHashTable* table, const char* cityName) {
    unsigned int index = hashFunction(cityName);
    City* city = table->cities[index];
    while(city) {
        if(strcmp(city->name, cityName) == 0) {
            return city;
        }
        index = (index + 1) % CITY_HASH_TABLE_SIZE;
        city = table->cities[index];
    }
    // Add new city
    city = (City*)malloc(sizeof(City));
    strcpy(city->name, cityName);
    city->totalPrice = 0.0;
    city->count = 0;
    table->cities[index] = city;
    table->count++;
    return city;
}

void insertProduct(City* city, const char* productName, double price) {
    // Try to find and update product price if it's cheaper
    for (int i = 0; i < city->count; i++) {
        if (strcmp(city->products[i].name, productName) == 0) {
            if (city->products[i].price > price) {
                city->totalPrice = city->totalPrice - city->products[i].price + price;
                city->products[i].price = price;
            }
            return;
        }
    }
    // Insert new product
    strcpy(city->products[city->count].name, productName);
    city->products[city->count].price = price;
    city->totalPrice += price;
    city->count++;
}

int compareProducts(const void* a, const void* b) {
    const Product* prodA = (const Product*)a;
    const Product* prodB = (const Product*)b;

    if (prodA->price < prodB->price) return -1;
    if (prodA->price > prodB->price) return 1;
    return strcmp(prodA->name, prodB->name);
}

void findCheapestCity(CityHashTable* cityTable, FILE* outputFile) {
    double minTotalPrice = 1e9;
    City* cheapestCity = NULL;
    for (int i = 0; i < CITY_HASH_TABLE_SIZE; i++) {
        City* city = cityTable->cities[i];
        if (city && city->totalPrice < minTotalPrice) {
            minTotalPrice = city->totalPrice;
            cheapestCity = city;
        }
    }

    if (!cheapestCity)
        return;

    // Sort only the products of the cheapest city
    qsort(cheapestCity->products, cheapestCity->count, sizeof(Product), compareProducts);

    // Write the cheapest city and its products to the output file
    fprintf(outputFile, "%s %.2f\n", cheapestCity->name, cheapestCity->totalPrice);
    for (int i = 0; i < 5 && i < cheapestCity->count; i++) {
        fprintf(outputFile, "%s %.2f\n", cheapestCity->products[i].name, cheapestCity->products[i].price);
    }
}




int main() {
    CityHashTable cityTable = {0};


    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

     // Open the output file for writing
    FILE *out = fopen("output.txt", "w");
    if (!out) {
        perror("Error opening output file");
        fclose(fp);
        return EXIT_FAILURE;
    }

    char cityName[50];
    char productName[50];
    double price;

    while (fscanf(fp, "%49[^,],%49[^,],%lf\n", cityName, productName, &price) == 3) {
        City* city = getOrAddCity(&cityTable, cityName);
        insertProduct(city, productName, price);
    }

    fclose(fp);

     // Find the cheapest city and write the results to the output file
    findCheapestCity(&cityTable, out);
    fclose(out); // Close the output file

    // Cleanup memory
    for (int i = 0; i < CITY_HASH_TABLE_SIZE; i++) {
        if (cityTable.cities[i]) {
            free(cityTable.cities[i]);
        }
    }

    return 0;
}
