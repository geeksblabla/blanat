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

typedef struct {
    char city[MAX_CITY_NAME_LENGTH];
    char product[MAX_PRODUCT_NAME_LENGTH];
    double price;
} Row;

typedef struct {
    char product[MAX_PRODUCT_NAME_LENGTH];
    double price;
} ProductRow;

typedef struct {
    char city[MAX_CITY_NAME_LENGTH];
    double total_price;
} CityTotal;

int compare_rows(const void *a, const void *b) {
    const ProductRow *row1 = (const ProductRow *)a;
    const ProductRow *row2 = (const ProductRow *)b;
    if (row1->price < row2->price) return -1;
    else if (row1->price > row2->price) return 1;
    else return 0;
}

int main() {
    clock_t t; 
    t = clock(); 

    FILE *file = fopen("input.txt", "r"); // SPECIFY THE INPUT FILE PATH
    if (file == NULL) {
        printf("Could not open file\n");
        return 1;
    }

    Row *data = malloc(MAX_ROWS * sizeof(Row));
    int i = 0;

    while (fscanf(file, "%49[^,],%49[^,],%lf\n", data[i].city, data[i].product, &data[i].price) == 3) {
        i++;
        if (i >= MAX_ROWS) {
            break;
        }
    }

    fclose(file);

    CityTotal *city_totals = malloc(sizeof(CityTotal));
    int size = 1;
    int city_count = 0;

    for (int i = 0; i < MAX_ROWS; i++) {
        int found = 0;
        for (int j = 0; j < city_count; j++) {
            if (strcmp(data[i].city, city_totals[j].city) == 0) {
                city_totals[j].total_price += data[i].price;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (city_count == size) {
                size *= 2;
                city_totals = realloc(city_totals, size * sizeof(CityTotal));
            }
            strcpy(city_totals[city_count].city, data[i].city);
            city_totals[city_count].total_price = data[i].price;
            city_count++;
        }
    }

    double cheapest_price = city_totals[0].total_price;
    char cheapest_city[MAX_CITY_NAME_LENGTH];
    strcpy(cheapest_city, city_totals[0].city);

    for (int i = 0; i < city_count; i++) {
        if (city_totals[i].total_price < cheapest_price) {
            cheapest_price = city_totals[i].total_price;
            strcpy(cheapest_city, city_totals[i].city);
        }
    }

    free(city_totals);

    // Find items in the cheapest city
    ProductRow *cheapest_city_rows = malloc(sizeof(ProductRow));
    size = 1;
    int cheapest_city_rows_count = 0;

    for (int i = 0; i < MAX_ROWS; i++) {
        if (strcmp(data[i].city, cheapest_city) == 0) {
            if (cheapest_city_rows_count == size) {
                size *= 2;
                cheapest_city_rows = realloc(cheapest_city_rows, size * sizeof(ProductRow));
            }
            strcpy(cheapest_city_rows[cheapest_city_rows_count].product, data[i].product);
            cheapest_city_rows[cheapest_city_rows_count].price = data[i].price;
            cheapest_city_rows_count++;
        }
    }
    
    // Sort items by price
    qsort(cheapest_city_rows, cheapest_city_rows_count, sizeof(ProductRow), compare_rows);

    FILE* fp = fopen("output.txt", "w"); // SPECIFY THE OUTPUT FILE PATH
    if (NULL == fp) {
        printf("Cannot create/open file %s. Make sure you have permission to create/open a file in the directory\n", "output.txt");
        exit(0);
    }

    fprintf(fp, "%s %.2f\n", cheapest_city, cheapest_price);
    for (int i = 0; i < 5 && i < MAX_ROWS; i++) {
        fprintf(fp, "%s %.2f\n", cheapest_city_rows[i].product, cheapest_city_rows[i].price);
    }

    fclose(fp);

    free(data);
    free(cheapest_city_rows);

    t = clock() - t;
    double time_taken = ((double)t);
    printf("The program took %f milli-seconds to execute\n", time_taken);
    
    return 0;
}
