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

struct KeyValuePair {
    char* key;
    char product[MAX_PRODUCT_NAME_LENGTH];
    double price;
    struct KeyValuePair* next;
};

struct CityTotal {
    char city[MAX_CITY_NAME_LENGTH];
    double total_price;
};

struct HashTable {
    int size;
    struct KeyValuePair** table;
};

// Function to create a new key-value pair
struct KeyValuePair* createKeyValuePair(const char* key, char product[MAX_PRODUCT_NAME_LENGTH], double price) {
    struct KeyValuePair* pair = (struct KeyValuePair*)malloc(sizeof(struct KeyValuePair));
    pair->key = strdup(key);
    strcpy(pair->product, product);
    pair->price = price;
    pair->next = NULL;
    return pair;
}

// Function to create a new hash table
struct HashTable* createHashTable(int size) {
    struct HashTable* hashTable = (struct HashTable*)malloc(sizeof(struct HashTable));
    hashTable->size = size;
    hashTable->table = (struct KeyValuePair**)calloc(size, sizeof(struct KeyValuePair*));
    return hashTable;
}

// Function to calculate the hash code for a key
int hashCode(const char* key, int tableSize) {
    int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash += key[i];
    }
    return hash % tableSize;
}

// Function to insert a key-value pair into the hash table
void insert(struct HashTable* hashTable, const char* key, char product[MAX_PRODUCT_NAME_LENGTH], double price) {
    int index = hashCode(key, hashTable->size);
    struct KeyValuePair* pair = createKeyValuePair(key, product, price);

    if (hashTable->table[index] == NULL) {
        hashTable->table[index] = pair;
    } else {
        struct KeyValuePair* current = hashTable->table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = pair;
    }
}

// Function to print a bucket
void print_bucket(struct HashTable* hashTable, const char* key) {
    int index = hashCode(key, hashTable->size);
    struct KeyValuePair* current = hashTable->table[index];
    while (current != NULL) {
        printf("%s: %s, %.2f\n", current->key, current->product, current->price);
        current = current->next;
    }
}

// Function to delete a key-value pair from the hash table
void delete(struct HashTable* hashTable, const char* key) {
    int index = hashCode(key, hashTable->size);
    struct KeyValuePair* current = hashTable->table[index];
    struct KeyValuePair* prev = NULL;

    // Traverse the linked list in the bucket
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            // Key found, remove the pair
            if (prev == NULL) {
                // If it's the first in the list
                hashTable->table[index] = current->next;
            } else {
                prev->next = current->next;
            }

            free(current->key);
            free(current);
            return;
        }

        prev = current;
        current = current->next;
    }
}

void free_hash_table(struct HashTable* hashTable) {
    for (int i = 0; i < hashTable->size; i++) {
        struct KeyValuePair* current = hashTable->table[i];
        while (current != NULL) {
            struct KeyValuePair* next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
    }
    free(hashTable->table);
    free(hashTable);
}

// Sort the key-value pairs in a bucket
void sortBucket(struct HashTable* hashTable, const char* key) {
    int index = hashCode(key, hashTable->size);
    struct KeyValuePair* current = hashTable->table[index];
    struct KeyValuePair* next = NULL;
    char temp_key[MAX_CITY_NAME_LENGTH];
    char temp_product[MAX_PRODUCT_NAME_LENGTH];
    double temp_price;

    while (current != NULL) {
        next = current->next;

        while (next != NULL) {
            if (current->price > next->price) {
                strcpy(temp_key, current->key);
                strcpy(current->key, next->key);
                strcpy(next->key, temp_key);

                strcpy(temp_product, current->product);
                strcpy(current->product, next->product);
                strcpy(next->product, temp_product);

                temp_price = current->price;
                current->price = next->price;
                next->price = temp_price;
            }
            next = next->next;
        }
        current = current->next;
    }
}

// Function to conpute city totals
void computeCityTotals(struct KeyValuePair* current, char current_city[MAX_CITY_NAME_LENGTH], double current_price) {
    current_price = 0.0;

    while (current != NULL) {
        if (strcmp(current->key, current_city) == 0) {
            current_price += current->price;
        }
        current = current->next;
    }
}

// Function to find the cheapest city to live in
void findCheapestCity(struct HashTable* hashTable, int size, double cheapest_price, char cheapest_city[MAX_CITY_NAME_LENGTH], FILE* fp) {
    cheapest_price = 0.0;
    struct KeyValuePair* current = NULL;

    for (int i = 0; i < size; i++) {
        current = hashTable->table[i];
        while (current != NULL) {
            double current_price = 0.0;
            computeCityTotals(current, current->key, current_price);
            if (current_price < cheapest_price || cheapest_price == 0.0) {
                cheapest_price = current_price;
                strcpy(cheapest_city, current->key);
            }
            current = current->next;
        }
    }
}

// Function to find the top 5 cheapest products in that cheapest city
void findCheapestProducts(struct HashTable* hashTable, const char* key, FILE* fp) {
    int index = hashCode(key, hashTable->size);
    struct KeyValuePair* current = hashTable->table[index];

    sortBucket(hashTable, key);

    for (int i = 0; i < 5; i++) {
        fprintf(fp, "%s %.2f\n", current->product, current->price);
        current = current->next;
    }

    
}

int main() {
    clock_t t; 
    t = clock(); 
    
    struct HashTable* hashTable = createHashTable(102);

    FILE *file = fopen("input.txt", "r"); // SPECIFY THE INPUT FILE PATH
    if (file == NULL) {
        printf("Could not open file\n");
        return 1;
    }

    char city[MAX_CITY_NAME_LENGTH];
    char product[MAX_PRODUCT_NAME_LENGTH];
    double price;
    int i = 0;

    while (fscanf(file, "%49[^,],%49[^,],%lf\n", city, product, &price) == 3) {
        insert(hashTable, city, product, price);
        i++;
        if (i >= MAX_ROWS) {
            break;
        }
    }

    fclose(file);

    FILE* fp = fopen("output2.txt", "w"); // SPECIFY THE OUTPUT FILE PATH
    if (NULL == fp) {
        printf("Cannot create/open file %s. Make sure you have permission to create/open a file in the directory\n", "output.txt");
        exit(0);
    }

    double cheapest_price = 0.0;
    char cheapest_city[MAX_CITY_NAME_LENGTH];
    findCheapestCity(hashTable, hashTable->size, cheapest_price, cheapest_city, fp);

    findCheapestProducts(hashTable, cheapest_city, fp);

    fclose(fp);

    free_hash_table(hashTable);
    
    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC;
    printf("The program took %f seconds to execute\n", time_taken);
    return 0;
}