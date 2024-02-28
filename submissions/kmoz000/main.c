/*
 *  NB: still not sure about the term cheapest city! (cheap by total or by lower cheap_products indexes total<can be translated to cost of living>)
 *  made by love by @kmoz000 and Stack Overflow answers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_CITY_NAME_LEN 50
#define MAX_PRODUCT_NAME_LEN 50

typedef struct
{
    char city[MAX_CITY_NAME_LEN];
    char product[MAX_PRODUCT_NAME_LEN];
    double price;
} ProductEntry;
typedef struct
{
    char name[MAX_CITY_NAME_LEN];
    double totalPrices;
} CityEntry;
void readDataFromFile(const char *filename, ProductEntry **data, int *numEntries)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    ProductEntry *tempData = NULL;
    while (1)
    {
        ProductEntry *newTempData = realloc(tempData, (count + 1) * sizeof(ProductEntry));
        if (!newTempData)
        {
            perror("Memory allocation error");
            free(newTempData);
            return;
        }
        else
        {
            tempData = newTempData;
        }
        if (fscanf(file, "%49[^,],%49[^,],%lf\n", tempData[count].city, tempData[count].product, &tempData[count].price) != 3)
        {
            break;
        }
        count++;
    }
    *numEntries = count;
    *data = tempData;
    fclose(file);
}

int compareProducts(const void *a, const void *b)
{
    return ((ProductEntry *)a)->price - ((ProductEntry *)b)->price;
}

void sortProducts(ProductEntry *data, int numEntries)
{
    qsort(data, numEntries, sizeof(ProductEntry), compareProducts);
}
int compareCitiesByTotalPrices(const void *a, const void *b)
{
    return (int)(((CityEntry *)a)->totalPrices - ((CityEntry *)b)->totalPrices);
}
void sortCitiesByTotalPrices(CityEntry *cities, int numCities)
{
    qsort(cities, numCities, sizeof(CityEntry), compareCitiesByTotalPrices);
}
void calculateTotalCostByCity(ProductEntry *data, int numEntries, CityEntry **cities, int *numCities)
{
    // Assuming that cities num is less than 101
    char cityNames[101][MAX_CITY_NAME_LEN];
    double totalCosts[1000] = {0};

    for (int i = 0; i < numEntries; i++)
    {
        int cityIndex = -1;
        for (int j = 0; j < *numCities; j++)
        {
            if (strcmp(data[i].city, cityNames[j]) == 0)
            {
                cityIndex = j;
                break;
            }
        }

        if (cityIndex == -1)
        {
            cityIndex = *numCities;
            strncpy(cityNames[*numCities], data[i].city, MAX_CITY_NAME_LEN);
            (*numCities)++;
        }
        totalCosts[cityIndex] += data[i].price;
    }
    *cities = malloc(*numCities * sizeof(CityEntry));
    if (!*cities)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < *numCities; i++)
    {
        strncpy((*cities)[i].name, cityNames[i], MAX_CITY_NAME_LEN);
        (*cities)[i].totalPrices = totalCosts[i];
    }
}
int main(int argc, char *argv[])
{
    if (argc != 3 || (strcmp(argv[1], "-i") != 0 && strcmp(argv[1], "--input") != 0))
    {
        printf("Usage: %s -i/--input <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[2];
    clock_t start_time = clock();
    int numEntries;
    ProductEntry *data;
    int numCities = 0;
    CityEntry *cities;

    readDataFromFile(filename, &data, &numEntries);
    sortProducts(data, numEntries);
    sortCitiesByTotalPrices(cities, numCities);
    calculateTotalCostByCity(data, numEntries, &cities, &numCities);

    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    printf("Cheapest city:\n");
    printf("%s %.2lf\n", cities[0].name, cities[0].totalPrices);
    fprintf(outputFile, "%s %.2lf\n", cities[0].name, cities[0].totalPrices);
    printf("Cheapest 5 products across all cities:\n");
    for (int i = 0; i < numEntries && i < 5; i++)
    {
        printf("%s %.2lf\n", data[i].product, data[i].price);
        fprintf(outputFile, "%s %.2lf\n", data[i].product, data[i].price);
    }
    clock_t total_cost_time = clock();
    double total_cost_elapsed = ((double)(total_cost_time - start_time)) / CLOCKS_PER_SEC;
    fclose(outputFile);
    free(data);
    free(cities);
    printf("Time taken to calculate (%d) Entries: %f seconds\n", numEntries, total_cost_elapsed);
    return 0;
}
/*
Results i got from the 1 000 000 entries in repo (input):
    Cheapest city:
    Khemisset 499672.12
    Cheapest 5 products across all cities:
    Goji_Berry 1.33
    Date 1.07
    Okra 1.28
    Plantain 1.27
    Garlic 1.01
    Time taken to calculate (1000000) Entries: 0.853328 seconds

Device:
  Processor Name:	Quad-Core Intel Core i7
  Processor Speed:	2.2 GHz
  Number of Processors:	1
  Total Number of Cores:	4
  L2 Cache (per Core):	256 KB
  L3 Cache:	6 MB
  Hyper-Threading Technology:	Enabled
  Memory:	16 GB

Estimation for 1 billion enteries after mutiple runs: ~14min13s (±15s)
*/
