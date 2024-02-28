#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <windows.h> // YES WINDOWS :) IKR
#include <pthread.h>

// SPECIFY THE NUMBER OF ROWS IN HERE (EX FOR THE CHALLENGE TESTING DATA: 1000000000)
#define MAX_ROWS 1000000000
// SPECIFY THE MAXIMUM LENGTH OF city
#define MAX_CITY_NAME_LENGTH 50
// SPECIFY THE MAXIMUM LENGTH OF PRODUCT city
#define MAX_PRODUCT_NAME_LENGTH 50
// SPECIFY THE INPUT FILE PATH
#define INPUT_FILE_PATH "input.txt"
// SPECIFY THE OUTPUT FILE PATH
#define OUTPUT_FILE_PATH "output.txt"

#define MAX_LINE_SIZE 2048

#define NUM_THREADS 8

struct KeyValuePair {
    char product[MAX_PRODUCT_NAME_LENGTH];
    double price;
};

struct CityTotal {
    char city[MAX_CITY_NAME_LENGTH];
    double total_price;
    struct CityTotal* next;
};


struct CityTotalHashTable {
    int size;
    struct CityTotal** table;
    pthread_mutex_t* locks;
};

struct ThreadData {
    int start;
    int end;
};

// Function to create a new key-value pair
struct KeyValuePair* createKeyValuePair(const char* key, char product[MAX_PRODUCT_NAME_LENGTH], double price) {
    struct KeyValuePair* pair = (struct KeyValuePair*)malloc(sizeof(struct KeyValuePair));
    strcpy(pair->product, product);
    pair->price = price;
    return pair;
}

void mergeProducts(struct KeyValuePair* arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    struct KeyValuePair *L = (struct KeyValuePair*)malloc(n1 * sizeof(struct KeyValuePair));
    struct KeyValuePair *R = (struct KeyValuePair*)malloc(n2 * sizeof(struct KeyValuePair));
 
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i].price <= R[j].price) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSortProducts(struct KeyValuePair* arr, int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
 
        mergeSortProducts(arr, l, m);
        mergeSortProducts(arr, m + 1, r);
 
        mergeProducts(arr, l, m, r);
    }
}

struct CityTotalHashTable* createCityTotalHashTable(int size) {
    struct CityTotalHashTable* cityTotalHashTable = (struct CityTotalHashTable*)malloc(sizeof(struct CityTotalHashTable));
    cityTotalHashTable->size = size;
    cityTotalHashTable->table = (struct CityTotal**)calloc(size, sizeof(struct CityTotal*));
    cityTotalHashTable->locks = (pthread_mutex_t*)malloc(size * sizeof(pthread_mutex_t));
    for (int i = 0; i < size; i++) {
        pthread_mutex_init(&cityTotalHashTable->locks[i], NULL);
    }
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
    struct CityTotal* current = cityTotalHashTable->table[index];

    if (current == NULL) {
        current = (struct CityTotal*)malloc(sizeof(struct CityTotal));
        strcpy(current->city, key);
        current->total_price = price;
        current->next = NULL;
        cityTotalHashTable->table[index] = current;
    } else {
        while (current->next != NULL) {
            if (strcmp(current->city, key) == 0) {
                current->total_price += price;
                return;
            }
            current = current->next;
        }
        if (strcmp(current->city, key) == 0) {
            current->total_price += price;
            return;
        }
        current->next = (struct CityTotal*)malloc(sizeof(struct CityTotal));
        current = current->next;
        strcpy(current->city, key);
        current->total_price = price;
        current->next = NULL;
    }
    pthread_mutex_unlock(&cityTotalHashTable->locks[index]);
}

void free_city_total_hash_table(struct CityTotalHashTable* cityTotalHashTable) {
    struct CityTotal* current;
    for (int i = 0; i < cityTotalHashTable->size; i++) {
        if (cityTotalHashTable->table[i] != NULL) {
            current = cityTotalHashTable->table[i];
            while (current != NULL) {
                struct CityTotal* temp = current;
                current = current->next;
                free(temp);
            }
        }
    }
    free(cityTotalHashTable->table);
    free(cityTotalHashTable->locks);
    free(cityTotalHashTable);
}

// Function to find the top 5 cheapest products in that cheapest city
void findCheapestProducts(const char* key, FILE* fp) {
    struct KeyValuePair cheapest_products_stack[100];
    int cheapest_products_stack_size = 0;

    FILE *file = fopen(INPUT_FILE_PATH, "r"); // SPECIFY THE INPUT FILE PATH
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
            for (int j = 0; j < 100; j++) {
                if (strcmp(cheapest_products_stack[j].product, product) == 0) {
                    if (price < cheapest_products_stack[j].price) {
                        cheapest_products_stack[j].price = price;
                    }
                    break;
                } else if (j == cheapest_products_stack_size) {
                    strcpy(cheapest_products_stack[cheapest_products_stack_size].product, product);
                    cheapest_products_stack[cheapest_products_stack_size].price = price;
                    cheapest_products_stack_size++;
                    break;
                }
            }
        }
    }

    fclose(file);

    for (int i = 0; i < cheapest_products_stack_size; i++) {
        for (int j = i + 1; j < cheapest_products_stack_size; j++) {
            if (strcmp(cheapest_products_stack[i].product, cheapest_products_stack[j].product) > 0 ) {
                struct KeyValuePair temp = cheapest_products_stack[i];
                cheapest_products_stack[i] = cheapest_products_stack[j];
                cheapest_products_stack[j] = temp;
            }
        }
    }

    mergeSortProducts(cheapest_products_stack, 0, cheapest_products_stack_size - 1);

    for (int i = 0; i < 5; i++) {
        if (cheapest_products_stack[i].price != 0.00 && strcmp(cheapest_products_stack[i].product, "") != 0) {
            fprintf(fp, "%s %.2f\n", cheapest_products_stack[i].product, cheapest_products_stack[i].price);
        }
    }
}

// Function to process a batch of data
void* processBatch(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    
    struct CityTotalHashTable* cityTotalHashTable = createCityTotalHashTable(102);
    
    FILE *file = fopen(INPUT_FILE_PATH, "r"); // SPECIFY THE INPUT FILE PATH
    if (file == NULL) {
        printf("Could not open file\n");
        pthread_exit(NULL);
    }

    char* line = (char*)malloc(MAX_LINE_SIZE * sizeof(char));
    int i = 0;

    // Process data in the batch
    while (fgets(line, MAX_LINE_SIZE, file) && i < data->end) {
        if (i >= data->start) {
            char* city = strtok(line, ",");
            char* product = strtok(NULL, ",");
            double price = atof(strtok(NULL, ",\n"));

            insertCityTotal(cityTotalHashTable, city, price);
        }
        i++;
    }

    fclose(file);

    return (void*)cityTotalHashTable;
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    /* LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    double interval;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start); */
    
    struct CityTotalHashTable* cityTotalHashTable = createCityTotalHashTable(102);
    struct CityTotalHashTable** cityTotalHashTableArray = (struct CityTotalHashTable**)malloc(NUM_THREADS * sizeof(struct CityTotalHashTable*));

    pthread_t threads[NUM_THREADS];
    struct ThreadData threadData[NUM_THREADS];

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].start = i * (MAX_ROWS / NUM_THREADS);
        threadData[i].end = (i + 1) * (MAX_ROWS / NUM_THREADS);
        pthread_create(&threads[i], NULL, processBatch, &threadData[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], (void**)&cityTotalHashTableArray[i]);
    }

    struct CityTotal* current;

    // Merge the results from the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        for (int j = 0; j < cityTotalHashTableArray[i]->size; j++) {
            if (cityTotalHashTableArray[i]->table[j] != NULL) {
                current = cityTotalHashTableArray[i]->table[j];
                while (current != NULL) {
                    insertCityTotal(cityTotalHashTable, current->city, current->total_price);
                    current = current->next;
                }
            }
        }
    }

    // Find the cheapest products in the cheapest city
    char cheapest_city[MAX_CITY_NAME_LENGTH];
    double cheapest_price = 1.7976931348623158e+308;
    for (int i = 0; i < cityTotalHashTable->size; i++) {
        if (cityTotalHashTable->table[i] != NULL) {
            current = cityTotalHashTable->table[i];
            while (current != NULL) {
                if (current->total_price < cheapest_price) {
                    cheapest_price = current->total_price;
                    strcpy(cheapest_city, current->city);
                }
                current = current->next;
            }
        }
    }
    
    // free the memory
    for (int i = 0; i < NUM_THREADS; i++) {
        free_city_total_hash_table(cityTotalHashTableArray[i]);
    }
    free(cityTotalHashTableArray);
    free_city_total_hash_table(cityTotalHashTable);

    FILE* fp = fopen(OUTPUT_FILE_PATH, "w"); // SPECIFY THE OUTPUT FILE PATH

    fprintf(fp, "%s %.2f\n", cheapest_city, cheapest_price);

    findCheapestProducts(cheapest_city, fp);

    fclose(fp);

    /* QueryPerformanceCounter(&end);
    interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;

    printf("The program took %f seconds to execute\n", interval); */
    return 0;
}