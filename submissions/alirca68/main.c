#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HCAP (4096)
#define MAX_PRODUCTS 5

struct product
{
    char name[100];
    double min_price;
};

struct city
{
    double total_cost;
    int product_count;
    struct product products[MAX_PRODUCTS];
    char name[100];
};

static unsigned int hash(const unsigned char *data, int n)
{
    unsigned int hash = 0;
    for (int i = 0; i < n; i++)
    {
        hash = (hash * 31) + data[i];
    }
    return hash;
}

static int cmpProduct(const void *ptr_a, const void *ptr_b)
{
    const double epsilon = 0.000001;
    double diff = ((struct product *)ptr_a)->min_price - ((struct product *)ptr_b)->min_price;
    if (diff < -epsilon)
    {
        return -1;
    }
    else if (diff > epsilon)
    {
        return 1;
    }
    else
    {
        return strcmp(((struct product *)ptr_a)->name, ((struct product *)ptr_b)->name);
    }
}

int main(int argc, const char **argv)
{
    const char *file = "input.txt";
    if (argc > 1)
    {
        file = argv[1];
    }

    FILE *fh = fopen(file, "r");
    if (!fh)
    {
        perror("error opening file");
        return EXIT_FAILURE;
    }

    struct city cities[HCAP];
    int nCities = 0;
    int map[HCAP];
    memset(map, -1, sizeof(map));
    struct city *lowestCostCity = NULL;
    char city[100], product[100];
    double price;

    while (fscanf(fh, "%99[^,],%99[^,],%lf", city, product, &price) == 3)
    {
        int h = hash((unsigned char *)city, strlen(city)) % HCAP;
        while (map[h] != -1 && strcmp(cities[map[h]].name, city) != 0)
        {
            h = (h + 1) % HCAP;
        }

        int c = map[h];
        if (c < 0)
        { // New city
            c = nCities++;
            strcpy(cities[c].name, city);
            cities[c].total_cost = 0;
            cities[c].product_count = 0;
            map[h] = c;
        }

        struct city *currentCity = &cities[c];
        int productIndex = -1;
        for (int i = 0; i < currentCity->product_count; i++)
        {
            if (strcmp(currentCity->products[i].name, product) == 0)
            {
                productIndex = i;
                break;
            }
        }

        if (productIndex != -1)
        { // Product exists, check if the new price is lower
            if (price < currentCity->products[productIndex].min_price)
            {
                currentCity->total_cost += price - currentCity->products[productIndex].min_price;
                currentCity->products[productIndex].min_price = price;
            }
        }
        else
        { // New product for the city
            if (currentCity->product_count < MAX_PRODUCTS)
            {
                productIndex = currentCity->product_count++;
                strcpy(currentCity->products[productIndex].name, product);
                currentCity->products[productIndex].min_price = price;
                currentCity->total_cost += price;
            }
        }

        if (!lowestCostCity || currentCity->total_cost < lowestCostCity->total_cost)
        {
            lowestCostCity = currentCity;
        }
    }

    fclose(fh);

    if (!lowestCostCity)
    {
        fprintf(stderr, "No valid data was processed.\n");
        return EXIT_FAILURE;
    }

    qsort(lowestCostCity->products, lowestCostCity->product_count, sizeof(struct product), cmpProduct);

    FILE *outputFile = fopen("output.txt", "w");
    if (!outputFile)
    {
        perror("error opening output file");
        return EXIT_FAILURE;
    }

    fprintf(outputFile, "%s %.2f\n", lowestCostCity->name, lowestCostCity->total_cost);
    for (int j = 0; j < lowestCostCity->product_count; j++)
    {
        fprintf(outputFile, "%s %.2f\n", lowestCostCity->products[j].name, lowestCostCity->products[j].min_price);
    }

    fclose(outputFile);

    return 0;
}