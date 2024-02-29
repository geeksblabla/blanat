#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void main() {
    FILE *fptr;
    fptr = fopen("input.txt", "r");
    char namecity[50], nameproduct[50], nameprice[50];
    int key = 0;
    int size = 100;

    char (*citys)[50] = malloc(size * sizeof(*citys));
    float *price = malloc(size * sizeof(*price));

    char myString[50];
    int i = 0;

    while(fgets(myString, 50, fptr)) {
        key = 0;
        memcpy(namecity, strtok(myString, ","),50);
        strtok(NULL, ",");
        memcpy(nameprice, strtok(NULL, ","),50);
        for (int j = 0; j < i; j++) {
            if (strcmp(namecity, citys[j]) == 0) {
                price[j] += atof(nameprice);
                key = 1;
                break;
            }
        }

        if (key == 0) {
            if (i >= size) {
                size *= 2;
                citys = realloc(citys, size * sizeof(*citys));
                price = realloc(price, size * sizeof(*price));
            }
            memcpy(citys[i], namecity,50);
            price[i] = atof(nameprice);
            i++;
        }
    }

    float minprice = (i > 0) ? price[0] : 0.0;
    int m_tindex = 0;

    for (int m = 0; m < i; m++) {
        if (price[m] <= minprice) {
            minprice = price[m];
            m_tindex = m;
        }
    }

    int psize = 100;
    int l = 0;
    char myString2[50];
    char (*citypro)[50] = malloc(psize * sizeof(*citypro));
    float *pprice = malloc(psize * sizeof(*pprice));
    fptr= fopen("input.txt", "r");
    while (fgets(myString2, 50, fptr)) {
        memcpy(namecity, strtok(myString2, ","),50);
        memcpy(nameproduct, strtok(NULL, ","),50);
        memcpy(nameprice, strtok(NULL, ","),50);
        if(l>=psize){
            psize *= 2;
            citypro = realloc(citypro, psize * sizeof(*citypro));
            pprice = realloc(pprice, psize * sizeof(*pprice));
        }
        if (strcmp(namecity, citys[m_tindex]) == 0) {
            memcpy(citypro[l], nameproduct,50);
            pprice[l] = atof(nameprice);
            l++;
        }
    }

    char ship_name[10][50];
    float ship_price[10];
    int nbr_price=0;
    float min_pprice = 10000;
    while (nbr_price < 10) {
        float min_pprice = 1000;
        int min_index = -1;

        for (int k = 0; k <l; k++) {
            if (pprice[k] != 0 && pprice[k] < min_pprice) {
                min_pprice = pprice[k];
                min_index = k;
            }
        }
        int lock=0;
        if (min_index != -1) {
            for(int g=0;g<nbr_price;g++){
                if(strcmp(ship_name[g],citypro[min_index]) == 0){
                    lock=1;
                    break;
                }
            }
            if(lock==0){
                memcpy(ship_name[nbr_price],citypro[min_index],50);
                ship_price[nbr_price]=pprice[min_index];
                nbr_price++;
            }
            pprice[min_index] = 10000;
        }
    }

    FILE *output=fopen("output.txt","w");
    fprintf(output,"%s : %.2f\n",citys[m_tindex],price[m_tindex]);
    int nbr_eq=0;
    char temp[50];
    char (*sort_queue)[50] = malloc(10 * sizeof(*sort_queue));
    float sort_price_queue[10];
    int calc=0;
    while (nbr_eq < sizeof(sort_queue)) {
        for(int dbs=0;dbs<sizeof(sort_queue);dbs++){
            memcpy(sort_queue[dbs],"none",10);
            sort_price_queue[dbs]=0;
        }
        memcpy(sort_queue[0], ship_name[nbr_eq],50);
        sort_price_queue[0]=ship_price[nbr_eq];
        int q;
        for (q = 0; nbr_eq + q < sizeof(sort_queue); q++) {
            if (ship_price[nbr_eq] == ship_price[nbr_eq + q]) {
                memcpy(sort_queue[q], ship_name[nbr_eq + q],50);
                sort_price_queue[q]=ship_price[nbr_eq + q];
            } else {
                break;
            }
        }
        nbr_eq += q;

        for(int d=0;d<sizeof(sort_queue);d++){
            for(int b=d+1;b<sizeof(sort_queue);b++){
                if(strcmp(sort_queue[d],sort_queue[b])>0){
                    memcpy(temp,sort_queue[d],50);
                    memcpy(sort_queue[d],sort_queue[b],50);
                    memcpy(sort_queue[b],temp,50);
                }
            }
        }

        for(int db=0;db<nbr_eq;db++){
            if(calc<5){
                if(strcmp(sort_queue[db],"none")!=0){
                    fprintf(output,"%s : %.2f\n",sort_queue[db],sort_price_queue[db]);
                    calc++;
                }
            }
        }
    }
}
