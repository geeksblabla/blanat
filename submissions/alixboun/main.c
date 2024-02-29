#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HCAP (4096)
#define MAX_PRODUCTS 5
//las9 lmofid alix was here
struct product {
  char name[100];
  double min_price;
};

struct city {
  double total_cost;
  int product_count;
  struct product products[MAX_PRODUCTS];
  char name[100];
};
static unsigned int hash(const unsigned char *data, int n) {
  unsigned int hash = 0;
  for (int i = 0; i < n; i++) {
    hash = (hash * 31) + data[i];
  }
  return hash;
}
static int cmp(const void *ptr_a, const void *ptr_b) {
  return strcmp(((struct city *)ptr_a)->name, ((struct city *)ptr_b)->name);
}
static int cmpProduct(const void *ptr_a, const void *ptr_b) {
  const double epsilon = 0.000001;
  double diff = ((struct product *)ptr_a)->min_price - ((struct product *)ptr_b)->min_price;
  if (diff < -epsilon) {
    return -1;
  } else if (diff > epsilon) {
    return 1;
  } else {
    return strcmp(((struct product *)ptr_a)->name, ((struct product *)ptr_b)->name);
  }
}
int main(int argc, const char **argv) {
  const char *file = "input.txt";
  if (argc > 1) {
    file = argv[1];
  }

  FILE *fh = fopen(file, "r");
  if (!fh) {
    perror("error opening file");
    exit(EXIT_FAILURE);
  }
  struct city cities[HCAP];
  int nCities = 0;
  int map[HCAP];
  memset(map, -1, HCAP * sizeof(int));
  while (1) {
    char city[100], product[100];
    double price;
    if (fscanf(fh, "%99[^,],%99[^,],%lf", city, product, &price) != 3) {
      break;
    }
    int h = hash((unsigned char *)city, strlen(city)) & (HCAP - 1);
    while (map[h] != -1 && strcmp(cities[map[h]].name, city) != 0) {
      h = (h + 1) & (HCAP - 1);
    }
    int c = map[h];
    if (c < 0) {
      strcpy(cities[nCities].name, city);
      cities[nCities].total_cost = 0;
      cities[nCities].product_count = 0;
      for (int i = 0; i < MAX_PRODUCTS; i++) {
        strcpy(cities[nCities].products[i].name, "");
        cities[nCities].products[i].min_price = 0.0;
      }
      strcpy(cities[nCities].products[cities[nCities].product_count].name, product);
      cities[nCities].products[cities[nCities].product_count].min_price = price;
      cities[nCities].product_count++;
      cities[nCities].total_cost += price;
      map[h] = nCities;
      nCities++;
    } else {
      int p;
      for (p = 0; p < cities[c].product_count; p++) {
        if (strcmp(cities[c].products[p].name, product) == 0) {
          if (price < cities[c].products[p].min_price) {
            cities[c].products[p].min_price = price;
          }
          break;
        }
      }
      if (p == cities[c].product_count) {
        if (cities[c].product_count < MAX_PRODUCTS) {
          strcpy(cities[c].products[p].name, product);
          cities[c].products[p].min_price = price;
          cities[c].product_count++;
        } else {
          int highestPriceIndex = 0;
          for (int k = 1; k < cities[c].product_count; k++) {
            if (cities[c].products[k].min_price > cities[c].products[highestPriceIndex].min_price) {
              highestPriceIndex = k;
            }
          }
          if (price < cities[c].products[highestPriceIndex].min_price) {
            strcpy(cities[c].products[highestPriceIndex].name, product);
            cities[c].products[highestPriceIndex].min_price = price;
          }
        }
      }

      cities[c].total_cost += price;
    }
  }

  qsort(cities, (size_t)nCities, sizeof(*cities), cmp);
  int lowestCostIndex = 0;
  for (int i = 1; i < nCities; i++) {
    if (cities[i].total_cost < cities[lowestCostIndex].total_cost) {
      lowestCostIndex = i;
    }
  }
  qsort(cities[lowestCostIndex].products, (size_t)cities[lowestCostIndex].product_count, sizeof(struct product), cmpProduct);
  FILE *outputFile = fopen("output.txt", "w");
  if (!outputFile) {
    perror("error opening output file");
    exit(EXIT_FAILURE);
  }
  fprintf(outputFile, "%s %.2f\n", cities[lowestCostIndex].name, cities[lowestCostIndex].total_cost);
  for (int j = 0; j < cities[lowestCostIndex].product_count && j < MAX_PRODUCTS; j++) {
    fprintf(outputFile, "%s %.2f\n", cities[lowestCostIndex].products[j].name, cities[lowestCostIndex].products[j].min_price);
  }

  fclose(outputFile);

  return 0;
}
