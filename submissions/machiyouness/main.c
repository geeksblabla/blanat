// X account: @machiyouness
// Note: If I win, please send the prize to @laytoun, wah alyam wah

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdbool.h>

#define MAX_CITY_LENGTH 50
#define MAX_PRODUCT_LENGTH 50
#define MAX_LINE_LENGTH 150
#define INITIAL_CAPACITY 1000
#define LOAD_FACTOR 0.75

struct Entry {
    char city[MAX_CITY_LENGTH];
    float total_price;
    struct Entry *next;
};

struct HashMap {
    struct Entry **buckets;
    int capacity;
    int size;
};

unsigned int hash(const char *str, int capacity) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % capacity;
}

struct HashMap *create_hash_map(int capacity) {
    struct HashMap *map = (struct HashMap *)malloc(sizeof(struct HashMap));
    map->capacity = capacity;
    map->size = 0;
    map->buckets = (struct Entry **)calloc(capacity, sizeof(struct Entry *));
    return map;
}

void resize(struct HashMap *map) {
    int old_capacity = map->capacity;
    map->capacity *= 2;
    struct Entry **old_buckets = map->buckets;
    map->buckets = (struct Entry **)calloc(map->capacity, sizeof(struct Entry *));
    map->size = 0;

    for (int i = 0; i < old_capacity; i++) {
        struct Entry *entry = old_buckets[i];
        while (entry != NULL) {
            struct Entry *next = entry->next;
            int index = hash(entry->city, map->capacity);
            entry->next = map->buckets[index];
            map->buckets[index] = entry;
            map->size++;
            entry = next;
        }
    }

    free(old_buckets);
}

void put(struct HashMap *map, const char *city, float price) {
    if ((float)map->size / map->capacity >= LOAD_FACTOR) {
        resize(map);
    }

    int index = hash(city, map->capacity);
    struct Entry *entry = map->buckets[index];
    while (entry != NULL) {
        if (strcmp(entry->city, city) == 0) {
            entry->total_price += price;
            return;
        }
        entry = entry->next;
    }

    struct Entry *new_entry = (struct Entry *)malloc(sizeof(struct Entry));
    strncpy(new_entry->city, city, MAX_CITY_LENGTH);
    new_entry->total_price = price;
    new_entry->next = map->buckets[index];
    map->buckets[index] = new_entry;
    map->size++;
}

float get(struct HashMap *map, const char *city) {
    int index = hash(city, map->capacity);
    struct Entry *entry = map->buckets[index];
    while (entry != NULL) {
        if (strcmp(entry->city, city) == 0) {
            return entry->total_price;
        }
        entry = entry->next;
    }
    return 0.0;
}

void free_hash_map(struct HashMap *map) {
    for (int i = 0; i < map->capacity; i++) {
        struct Entry *entry = map->buckets[i];
        while (entry != NULL) {
            struct Entry *next = entry->next;
            free(entry);
            entry = next;
        }
    }
    free(map->buckets);
    free(map);
}

void find_top_cheapest_products(FILE *file, const char *city, FILE *outputFile) {
    char line[MAX_LINE_LENGTH];
    struct Product {
        float price;
        char name[MAX_PRODUCT_LENGTH];
    } products[5] = { { FLT_MAX, "" }, { FLT_MAX, "" }, { FLT_MAX, "" }, { FLT_MAX, "" }, { FLT_MAX, "" } };

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, city) != NULL) {
            char *token = strtok(line, ",");
            char tempCity[MAX_CITY_LENGTH];
            strncpy(tempCity, token, MAX_CITY_LENGTH);
            tempCity[MAX_CITY_LENGTH - 1] = '\0';

            token = strtok(NULL, ",");
            char productName[MAX_PRODUCT_LENGTH];
            strncpy(productName, token, MAX_PRODUCT_LENGTH);
            productName[MAX_PRODUCT_LENGTH - 1] = '\0';

            token = strtok(NULL, ",");
            float price = atof(token);

            bool duplicate = false;
            for (int i = 0; i < 5; ++i) {
                if (strcmp(products[i].name, productName) == 0) {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate) {
                int maxIndex = 0;
                float maxPrice = 0;
                for (int i = 0; i < 5; ++i) {
                    if (products[i].price > maxPrice) {
                        maxPrice = products[i].price;
                        maxIndex = i;
                    }
                }
                if (price < maxPrice) {
                    strncpy(products[maxIndex].name, productName, MAX_PRODUCT_LENGTH);
                    products[maxIndex].price = price;
                }
            }
        }
    }

    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            if (products[i].price > products[j].price) {
                struct Product temp = products[i];
                products[i] = products[j];
                products[j] = temp;
            }
        }
    }

    for (int i = 0; i < 5; ++i) {
        fprintf(outputFile, "%s %.2f\n", products[i].name, products[i].price);
    }
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    struct HashMap *city_prices = create_hash_map(INITIAL_CAPACITY);

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        if (token != NULL) {
            char city[MAX_CITY_LENGTH];
            strncpy(city, token, MAX_CITY_LENGTH);
            city[MAX_CITY_LENGTH - 1] = '\0';

            token = strtok(NULL, ",");
            char product[MAX_PRODUCT_LENGTH];
            strncpy(product, token, MAX_PRODUCT_LENGTH);
            product[MAX_PRODUCT_LENGTH - 1] = '\0';

            token = strtok(NULL, ",");
            float price = atof(token);

            put(city_prices, city, price);
        }
    }

    char lowest_city[MAX_CITY_LENGTH];
    float lowest_price = FLT_MAX;
    for (int i = 0; i < city_prices->capacity; i++) {
        struct Entry *entry = city_prices->buckets[i];
        while (entry != NULL) {
            if (entry->total_price < lowest_price) {
                strncpy(lowest_city, entry->city, MAX_CITY_LENGTH);
                lowest_price = entry->total_price;
            }
            entry = entry->next;
        }
    }

    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Failed to open the output file.\n");
        free_hash_map(city_prices);
        return 1;
    }

    fprintf(outputFile, "%s %.2f\n", lowest_city, lowest_price);

    rewind(file);

    find_top_cheapest_products(file, lowest_city, outputFile);

    fclose(file);

    free_hash_map(city_prices);

    return 0;
}
