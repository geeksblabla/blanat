#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


typedef struct {
    char *city;
    char **product;
    float *price;
    float total;
    int offset;
} Store;


typedef struct {
    Store *store1;
    Store *store2;
} Store2;



void *collect__(void *arg)
{
    Store2 *store = (Store2 *)arg;
    int i = 0;
    while(store->store2[i].city != NULL)
    {
        int j = 0;
        int flag = 0;
        while(store->store1[j].city != NULL)
        {
            if (strcmp(store->store1[j].city, store->store2[i].city) == 0)
            {
                flag = 1;
                int k = 0;
                store->store1[j].total += store->store2[i].total;
                while(store->store2[i].product[k] != NULL)
                {
                    int l = 0;
                    while(store->store1[j].product[l] != NULL)
                    {
                        if (strcmp(store->store1[j].product[l], store->store2[i].product[k]) == 0)
                        {
                            if (store->store1[j].price[l] > store->store2[i].price[k])
                                store->store1[j].price[l] = store->store2[i].price[k];
                            break;
                        }
                        l++;
                    }
                    if (store->store1[j].product[l] == NULL)
                    {
                        store->store1[j].product[l] = store->store2[i].product[k];
                        store->store1[j].price[l] = store->store2[i].price[k];
                        store->store1[j].total += store->store2[i].total;
                    }
                    k++;
                }

            }
            j++;
        }
        if (flag == 0)
        {
            store->store1[j].city = store->store2[i].city;
            store->store1[j].product = store->store2[i].product;
            store->store1[j].price = store->store2[i].price;
            store->store1[j].total = store->store2[i].total;
        }
        i++;
        
    }
    pthread_exit(0);
    return NULL;
}


void min_product(Store *store, int pos, float min, FILE *fdd)
{
    float min_price = 0;
    char *min_product;
    int rank1 = 0;
    int rank2 = 0;
    while(rank1 < 100)
    {
        if (store[pos].product[rank1] != NULL)
        {
            if (min_price == 0)
            {
                min_price = store[pos].price[rank1];
                min_product = store[pos].product[rank1];
                rank2 = rank1;
            }
            if (store[pos].price[rank1] < min_price)
            {
                min_price = store[pos].price[rank1];
                min_product = store[pos].product[rank1];
                rank2 = rank1;
            }
            if (store[pos].price[rank1] == min_price)
            {
                if (strcmp(store[pos].product[rank1], min_product) < 0)
                {
                    min_price = store[pos].price[rank1];
                    min_product = store[pos].product[rank1];
                    rank2 = rank1;
                }
            
            }
        }
        else
            break;
        rank1++;
    }
    store[pos].price[rank2] = 1000000;
    fprintf(fdd, "%s %.2f", min_product, min_price);
}




char *ft_strdup(const char *s) {
    size_t len = strlen(s) + 1;
    void *new = malloc(len);
    return (char *)memcpy(new, s, len);
}

float custom_atof(const char *str) {
    float result = 0.0;
    float fraction = 0.1; 
    int decimal_flag = 0;
    while (*str != '\0') {
        if (*str >= '0' && *str <= '9') {
            if (!decimal_flag) 
                result = result * 10.0 + (*str - '0');
            else
            {
                result += fraction * (*str - '0');
                fraction /= 10.0;
            }
        } else
            decimal_flag = 1;
        str++;
    }

    return result;
}


void *runner(void *param) {
    Store *store = (Store *)param;
    FILE *file;
    file = fopen("input.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }
    char buffer[100];
    char *city_name;
    char *product_name;
    char *price_value1;
    float price_value;
    int tt = 0;
    int contt = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        if (tt == store[0].offset)
        {
            store[0].offset += 4;
            char *buffer2 = buffer;
            city_name = strsep(&buffer2, ",");
            product_name = strsep(&buffer2, ",");
            price_value1 = strsep(&buffer2, "\n");
            price_value = custom_atof(price_value1);
            int i = 0;
           while(i < 101)
            {
                if (i == contt)
                {
                    store[i].city = ft_strdup(city_name);
                    store[i].product[0] = ft_strdup(product_name);
                    store[i].price[0] = price_value;
                    store[i].total += price_value;
                    contt++;
                    break;
                }
                if (strcmp(store[i].city, city_name) == 0)
                {
                    int b = 0;
                    while(b < 100)
                    {
                        if (store[i].product[b] == NULL)
                        {
                            store[i].product[b] = ft_strdup(product_name);
                            store[i].price[b] = price_value;
                            store[i].total += price_value;
                            break;
                        }
                        if (strcmp(store[i].product[b], product_name) == 0)
                        {
                            if (store[i].price[b] > price_value)
                                store[i].price[b] = price_value;
                            store[i].total += price_value;
                            break;
                        }
                        b++;
                    }
                    break;
                    
                }
                i++;
            }
        }
        tt++;
    }
    pthread_exit(0);
    return NULL;
}

void put_flaot(float value, FILE *fdd)
{
    int int_value = (int)value;
    fprintf(fdd, "%d.", int_value);
    int_value = (value - int_value) * 100;
    fprintf(fdd, "%d\n", int_value);
}

int main() {
    int i;
    pthread_t tid;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;
    Store **store = (Store **)calloc(sizeof(Store *), 9);
    int l = 0;
    while(l < 4)
    {
        store[l] = (Store *)calloc(sizeof(Store), 101);
        int j = 0;
        while(j < 101)
        {
            store[l][j].city = NULL;
            store[l][j].product = (char **)calloc(sizeof(char *),100);
            store[l][j].price = (float *)calloc(sizeof(float),100);
            store[l][j].total = -0.00;
            store[l][j].offset = l;
            j++;
        }
        l++;
    }
    pthread_create(&tid, NULL, runner, store[0]);
    pthread_create(&tid2, NULL, runner, store[1]);
    pthread_create(&tid3, NULL, runner, store[2]);
    pthread_create(&tid4, NULL, runner, store[3]);
    pthread_join(tid, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    Store2 *store9 = (Store2 *)calloc(sizeof(Store2), 1);
    store9->store1 = store[0];
    store9->store2 = store[1];
    pthread_create(&tid, NULL, collect__, (void *)store9);
    Store2 *store10 = (Store2 *)calloc(sizeof(Store2), 1);
    store10->store1 = store[2];
    store10->store2 = store[3];
    pthread_create(&tid2, NULL, collect__, (void *)store10);
    pthread_join(tid, NULL);
    pthread_join(tid2, NULL);
    Store2 *store13 = (Store2 *)calloc(sizeof(Store2), 1);
    store13->store1 = store9->store1;
    store13->store2 = store10->store1;
    pthread_create(&tid, NULL, collect__, (void *)store13);
    pthread_join(tid, NULL);
    Store *main_store = store13->store1;
    int rank = 0;
    float min = 0;
    int pos;
    while(rank < 101)
    {
        if (main_store[rank].city != NULL)
        {
            if (min == 0)
            {
                min = main_store[rank].total;
                pos = rank;
            }
            if (main_store[rank].total < min)
            {
                min = main_store[rank].total;
                pos = rank;
            }
        }
        else
            break;
        rank++;
    }
    int k = 0;
    FILE *fout = fopen("output.txt", "w");
    int fdd = fileno(fout);
    fprintf(fout, "%s ", main_store[pos].city);
    
    put_flaot(main_store[pos].total, fout);

    while(k < 5)
    {
        min_product(main_store, pos, min, fout);
        if(k != 4)
            fprintf(fout, "\n");
        k++;
    }
    fclose(fout);
}