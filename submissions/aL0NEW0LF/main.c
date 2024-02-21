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

void merge(CityTotal* arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    CityTotal *L = (CityTotal*)malloc(n1 * sizeof(CityTotal));
    CityTotal *R = (CityTotal*)malloc(n2 * sizeof(CityTotal));
 
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i].total_price <= R[j].total_price) {
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

void mergeSort(CityTotal* arr, int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
 
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
 
        merge(arr, l, m, r);
    }
}

void mergeProducts(ProductRow* arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    ProductRow *L = (ProductRow*)malloc(n1 * sizeof(ProductRow));
    ProductRow *R = (ProductRow*)malloc(n2 * sizeof(ProductRow));
 
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


void mergeSortProducts(ProductRow* arr, int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
 
        mergeSortProducts(arr, l, m);
        mergeSortProducts(arr, m + 1, r);
 
        mergeProducts(arr, l, m, r);
    }
}

int main() {
    clock_t t; 
    t = clock(); 

    CityTotal *city_totals = malloc(101 * sizeof(CityTotal));
    int city_count = 0;

    FILE *file = fopen("input.txt", "r"); // SPECIFY THE INPUT FILE PATH
    if (file == NULL) {
        printf("Could not open file\n");
        return 1;
    }

    Row *data = malloc(MAX_ROWS * sizeof(Row));
    int i = 0;

    while (fscanf(file, "%49[^,],%49[^,],%lf\n", data[i].city, data[i].product, &data[i].price) == 3) {
        int found = 0;
        for (int j = 0; j < city_count; j++) {
            if (strcmp(data[i].city, city_totals[j].city) == 0) {
                city_totals[j].total_price += data[i].price;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(city_totals[city_count].city, data[i].city);
            city_totals[city_count].total_price = data[i].price;
            city_count++;
        }
        i++;
        if (i >= MAX_ROWS) {
            break;
        }
    }

    fclose(file);

    double cheapest_price = city_totals[0].total_price;
    char cheapest_city[MAX_CITY_NAME_LENGTH];
    strcpy(cheapest_city, city_totals[0].city);

    mergeSort(city_totals, 0, city_count - 1);

    FILE* fp = fopen("output.txt", "w"); // SPECIFY THE OUTPUT FILE PATH
    if (NULL == fp) {
        printf("Cannot create/open file %s. Make sure you have permission to create/open a file in the directory\n", "output.txt");
        exit(0);
    }

    strcpy(cheapest_city, city_totals[0].city);
    cheapest_price = city_totals[0].total_price;

    free(city_totals);

    // Find items in the cheapest city
    ProductRow *cheapest_city_rows = malloc(MAX_ROWS * sizeof(ProductRow));
    int cheapest_city_rows_count = 0;

    for (int i = 0; i < MAX_ROWS; i++) {
        if (strcmp(data[i].city, cheapest_city) == 0) {
            strcpy(cheapest_city_rows[cheapest_city_rows_count].product, data[i].product);
            cheapest_city_rows[cheapest_city_rows_count].price = data[i].price;
            cheapest_city_rows_count++;
        }
    }
    
    mergeSortProducts(cheapest_city_rows, 0, cheapest_city_rows_count - 1);

    rewind(fp);

    fprintf(fp, "%s %.2f\n", cheapest_city, cheapest_price);
    for (int i = 0; i < 5 && i < MAX_ROWS; i++) {
        fprintf(fp, "%s %.2f\n", cheapest_city_rows[i].product, cheapest_city_rows[i].price);
    }

    fclose(fp);

    free(data);
    free(cheapest_city_rows);

    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC;
    printf("The program took %f seconds to execute\n", time_taken);
    
    return 0;
}
