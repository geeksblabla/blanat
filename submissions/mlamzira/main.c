#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct City City;
typedef struct Product Product;
typedef struct Buffer Buffer;

struct Product {
    char name[30];
    double price;
    Product *next;
    Product *previous;
};

struct Buffer {
    char name[30];
    char productName[30];
    double priceSum;
};

struct City {
    char name[30];
    double priceSum;
    double mostExpensivePrice;
    Product *first;
    Product *tail;
    int productCount;
};

float parseFloat(char floatValue[]);

void readCity(char *str, Buffer *buffer);


City *binarySearch(City *cities[], int n, char *targetName);

void addProduct(City *pCity, char *product, double price);

int main() {
    FILE *input = fopen("./input.txt", "r");
    FILE *sortedCities = fopen("./sorted-cities.txt", "r");
    FILE *output = fopen("./output.txt", "w");

    char cityLine[50];
    City *cities[101];
    int k = 0;
    while (fgets(cityLine, sizeof(cityLine), sortedCities) != NULL) {
        City *city = (City *) malloc(sizeof(City));
        cityLine[strcspn(cityLine, "\n")] = 0;
        strcpy(city->name, cityLine);
        city->priceSum = 0;
        strcpy(city->name, cityLine);
        city->mostExpensivePrice = 100;
        cities[k] = city;
        k++;
    }

    Buffer *buffer = (Buffer *) malloc(sizeof(Buffer *));
    City *cheapestCity = NULL;

    char line[256];  // Buffer for each line
    while (fgets(line, sizeof(line), input) != NULL) {
        line[strcspn(line, "\n")] = 0;
        readCity(line, buffer);

        City *currentCity = binarySearch(cities, 101, buffer->name);
        currentCity->priceSum += buffer->priceSum;
        addProduct(currentCity, buffer->productName, buffer->priceSum);
    }

    for (int i = 0; i < 101; ++i) {
        City *currentCity = cities[i];
        if (cheapestCity == NULL || cheapestCity->priceSum > currentCity->priceSum) {
            cheapestCity = currentCity;
        }

    }

    fprintf(output, "%s %.2f\n", cheapestCity->name, cheapestCity->priceSum);

    Product *tmp = cheapestCity->first;
    while (tmp != NULL) {
        fprintf(output, "%s %.2f\n", tmp->name, tmp->price);
        tmp = tmp->next;
    }

    fclose(sortedCities);
    fclose(input);
    fclose(output);
    return 0;
}

void addProduct(City *pCity, char *productName, double price) {
    if (pCity->productCount < 5) {
        Product *newProduct = (Product *) malloc(sizeof(Product));
        strcpy(newProduct->name, productName);
        newProduct->price = price;

        if (pCity->productCount == 0) {
            pCity->first = newProduct;
            pCity->tail = newProduct;
        } else if (pCity->productCount == 1) {
            if ((pCity->first->price > price) || (pCity->first->price == price && strcmp(productName, pCity->first->name) < 0)) {
                Product *oldFirst = pCity->first;
                pCity->first = newProduct;
                newProduct->next = oldFirst;
                oldFirst->previous = newProduct;
                pCity->tail = oldFirst;
            } else {
                pCity->first->next = newProduct;
                newProduct->previous = pCity->first;
                pCity->tail = newProduct;
            }
        } else {
            Product *tmp1 = pCity->first;
            Product *tmp2 = tmp1->next;

            if ((tmp1->price > price) || (tmp1->price == price && strcmp(productName, tmp1->name) < 0)) {
                pCity->first = newProduct;
                tmp1->previous = newProduct;
                newProduct->next = tmp1;
            } else {
                while (tmp2 != NULL) {
                    if ((tmp1->price < price && tmp2->price > price) || (tmp1->price < price && tmp2->price == price && strcmp(productName, tmp2->name) < 0)) {
                        tmp1->next = newProduct;
                        newProduct->previous = tmp1;
                        tmp2->previous = newProduct;
                        newProduct->next = tmp2;
                        break;
                    }
                    if (tmp2->next == NULL) {
                        tmp2->next = newProduct;
                        newProduct->previous = tmp2;
                        pCity->tail = newProduct;
                        break;
                    }
                    tmp1 = tmp2;
                    tmp2 = tmp2->next;
                }
            }

        }
        pCity->productCount++;
    } else {
        if (price <= pCity->mostExpensivePrice) {
            Product *last = pCity->tail;
            pCity->tail = pCity->tail->previous;
            pCity->tail->next = NULL;
            free(last);
            last = NULL;

            Product *newProduct = (Product *) malloc(sizeof(Product));
            strcpy(newProduct->name, productName);
            newProduct->price = price;

            Product *tmp1 = pCity->first;
            Product *tmp2 = tmp1->next;

            if ((tmp1->price > price) || (tmp1->price == price && strcmp(productName, tmp1->name) < 0)) {
                pCity->first = newProduct;
                tmp1->previous = newProduct;
                newProduct->next = tmp1;
            } else {
                while (tmp2 != NULL) {
                    if ((tmp1->price < price && tmp2->price > price) || (tmp1->price < price && tmp2->price == price && strcmp(productName, tmp2->name) < 0)) {
                        tmp1->next = newProduct;
                        newProduct->previous = tmp1;
                        tmp2->previous = newProduct;
                        newProduct->next = tmp2;
                        break;
                    }
                    if (tmp2->next == NULL) {
                        tmp2->next = newProduct;
                        newProduct->previous = tmp2;
                        pCity->tail = newProduct;
                        break;
                    }
                    tmp1 = tmp2;
                    tmp2 = tmp2->next;
                }
            }
        }
    }
    pCity->mostExpensivePrice = pCity->tail->price;
}

City *binarySearch(City *cities[], int n, char *targetName) {
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        int res = strcmp(cities[mid]->name, targetName);

        if (res == 0) {
            return cities[mid];
        }

        if (res < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
}

short cTos(char digitC) {
    return digitC - '0';
}

float parseFloat(char *floatValue) {

    unsigned long length = strlen(floatValue);
    if (length == 6) {
        return 100;
    }

    int commandIndex = -1;
    for (int i = 0; i < length; ++i) {
        if (floatValue[i] == '.') {
            commandIndex = i;
            break;
        }
    }

    if (length == 5 && commandIndex == 2) {
        return (cTos(floatValue[0]) * 10)
               + cTos(floatValue[1])
               + (cTos(floatValue[3]) * 0.1)
               + (cTos(floatValue[4]) * 0.01);
    }

    if (length == 4 && commandIndex == 2) {
        return (cTos(floatValue[0]) * 10)
               + cTos(floatValue[1])
               + (cTos(floatValue[3]) * 0.1);
    }

    if (length == 4 && commandIndex == 1) {
        return cTos(floatValue[0])
               + (cTos(floatValue[2]) * 0.1)
               + (cTos(floatValue[3]) * 0.01);
    }

    if (length == 3 && commandIndex == 1) {
        return cTos(floatValue[0])
               + (cTos(floatValue[2]) * 0.1);
    }
    return 0;

}

void readCity(char *str, Buffer *buffer) {
    char *token;
    token = strtok(str, ",");
    strcpy(buffer->name, token);
    token = strtok(NULL, ",");
    strcpy(buffer->productName, token);
    token = strtok(NULL, ",");
    buffer->priceSum = parseFloat(token);
}
