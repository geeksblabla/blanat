#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

// SPECIFY THE NUMBER OF ROWS IN HERE (EX FOR THE CHALLENGE TESTING DATA: 1000000000)
#define MAX_ROWS 8473
// SPECIFY THE MAXIMUM LENGTH OF city
#define MAX_CITY_NAME_LENGTH 50
// SPECIFY THE MAXIMUM LENGTH OF PRODUCT city
#define MAX_PRODUCT_NAME_LENGTH 50

#define MAX_LINE_SIZE 1048576

struct KeyValuePair {
    char product[MAX_PRODUCT_NAME_LENGTH];
    double price;
    struct KeyValuePair* next;
};

struct CityTotal {
    char city[MAX_CITY_NAME_LENGTH];
    double total_price;
};


struct CityTotalHashTable {
    int size;
    struct CityTotal** table;
};

// Function to create a new key-value pair
struct KeyValuePair* createKeyValuePair(const char* key, char product[MAX_PRODUCT_NAME_LENGTH], double price) {
    struct KeyValuePair* pair = (struct KeyValuePair*)malloc(sizeof(struct KeyValuePair));
    strcpy(pair->product, product);
    pair->price = price;
    pair->next = NULL;
    return pair;
}

struct CityTotalHashTable* createCityTotalHashTable(int size) {
    struct CityTotalHashTable* cityTotalHashTable = (struct CityTotalHashTable*)malloc(sizeof(struct CityTotalHashTable));
    cityTotalHashTable->size = size;
    cityTotalHashTable->table = (struct CityTotal**)calloc(size, sizeof(struct CityTotal*));
    return cityTotalHashTable;
};


// Function to calculate the hash code for a key
int hashCode(const char* key, int tableSize) {
    int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash += key[i];
    }
    return hash % tableSize;
}

void insertCityTotal(struct CityTotalHashTable* cityTotalHashTable, const char* key, double price) {
    int index = hashCode(key, cityTotalHashTable->size);
    struct CityTotal* pair = (struct CityTotal*)malloc(sizeof(struct CityTotal));
    strcpy(pair->city, key);
    pair->total_price = price;

    if (cityTotalHashTable->table[index] == NULL) {
        cityTotalHashTable->table[index] = pair;
    } else {
        cityTotalHashTable->table[index]->total_price += price;
    }
}

void free_city_total_hash_table(struct CityTotalHashTable* cityTotalHashTable) {
    for (int i = 0; i < cityTotalHashTable->size; i++) {
        struct CityTotal* current = cityTotalHashTable->table[i];
        free(current);
    }
    free(cityTotalHashTable->table);
    free(cityTotalHashTable);
}

// Function to find the top 5 cheapest products in that cheapest city
void findCheapestProducts(const char* key, FILE* fp) {
    struct KeyValuePair* cheapest_products_stack = NULL;
    int cheapest_products_stack_size = 0;

    FILE *file = fopen("input.txt", "r"); // SPECIFY THE INPUT FILE PATH
    if (file == NULL) {
        printf("Could not open file\n");
        exit(0);
    }

    char* line = (char*)malloc(MAX_LINE_SIZE * sizeof(char));
    int i = 0;

    while (fgets(line, MAX_LINE_SIZE, file) && i < MAX_ROWS) {
        char* city = strtok(line, ",");
        char* product = strtok(NULL, ",");
        double price = atof(strtok(NULL, ",\n"));

        if (strcmp(city, key) == 0) {
            if (cheapest_products_stack_size < 5) {
                struct KeyValuePair* pair = createKeyValuePair(city, product, price);
                pair->next = cheapest_products_stack;
                cheapest_products_stack = pair;
                cheapest_products_stack_size++;
            } else {
                struct KeyValuePair* current = cheapest_products_stack;
                struct KeyValuePair* prev = NULL;
                while (current != NULL) {
                    if (price < current->price) {
                        struct KeyValuePair* pair = createKeyValuePair(city, product, price);
                        if (prev == NULL) {
                            pair->next = cheapest_products_stack;
                            cheapest_products_stack = pair;
                        } else {
                            pair->next = current;
                            prev->next = pair;
                        }
                        prev = pair;
                        current = NULL;
                    } else {
                        prev = current;
                        current = current->next;
                    }
                }
            }
        }
        i++;
    }

    fclose(file);

    for (int i = 0; i < 5; i++) {
        fprintf(fp, "%s, %.2f\n", cheapest_products_stack->product, cheapest_products_stack->price);
        cheapest_products_stack = cheapest_products_stack->next;
    }
}


int main() {
    clock_t t; 
    t = clock(); 
    
    struct CityTotalHashTable* cityTotalHashTable = createCityTotalHashTable(102);

    FILE *file = fopen("input.txt", "r"); // SPECIFY THE INPUT FILE PATH
    if (file == NULL) {
        printf("Could not open file\n");
        return 1;
    }

    char* line = (char*)malloc(MAX_LINE_SIZE * sizeof(char));
    int i = 0;

    while (fgets(line, MAX_LINE_SIZE, file) && i < MAX_ROWS) {
        char* city = strtok(line, ",");
        char* product = strtok(NULL, ",");
        double price = atof(strtok(NULL, ",\n"));

        /* char* tok = strtok(line, ",");
        char city[MAX_CITY_NAME_LENGTH];
        strcpy(city, tok);
        tok = strtok(NULL, ",");
        char product[MAX_PRODUCT_NAME_LENGTH];
        strcpy(product, tok);
        tok = strtok(NULL, ",");
        double price = atof(tok); */

        insertCityTotal(cityTotalHashTable, city, price);
        i++;
    }

    fclose(file);

    char cheapest_city[MAX_CITY_NAME_LENGTH];
    double cheapest_price = 1.7976931348623158e+308;
    for (int i = 0; i < cityTotalHashTable->size; i++) {
        if (cityTotalHashTable->table[i] != NULL) {
            if (cityTotalHashTable->table[i]->total_price < cheapest_price) {
                cheapest_price = cityTotalHashTable->table[i]->total_price;
                strcpy(cheapest_city, cityTotalHashTable->table[i]->city);
            }
        }
    }

    free_city_total_hash_table(cityTotalHashTable);
    
    file = fopen("output.txt", "w"); // SPECIFY THE OUTPUT FILE PATH
    if (file == NULL) {
        printf("Could not open file\n");
        return 1;
    }

    fprintf(file, "%s %.2f\n", cheapest_city, cheapest_price);

    findCheapestProducts(cheapest_city, file);

    fclose(file);

    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC;
    printf("The program took %f seconds to execute\n", time_taken);
    return 0;
}