#pragma GCC optimize("O3")
#include <cstdio>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <thread>

struct Node {
    char *city;
    char *product[102];
    unsigned long price[102];
    unsigned long total;
};

unsigned int num_threads = 16;
size_t __offset;
char *the_main;
off_t file_size;

struct Node *hashMap[102];
struct Node *hashMap2[102];
struct Node *hashMap3[102];
struct Node *hashMap4[102];
struct Node *hashMap5[102];
struct Node *hashMap6[102];
struct Node *hashMap7[102];
struct Node *hashMap8[102];
struct Node *hashMap9[102];
struct Node *hashMap10[102];
struct Node *hashMap11[102];
struct Node *hashMap12[102];
struct Node *hashMap13[102];
struct Node *hashMap14[102];
struct Node *hashMap15[102];
struct Node *hashMap16[102];

char *city_names[101] = {"Fes","Assa","Safi","Sale","Taza","Ahfir","Azrou","Midar","Nador","Oujda","Rabat","Smara","Agadir","Aourir","Arfoud","Asilah",
"Azilal","Dakhla","Drarga","Figuig","Ifrane","Jerada","Meknes","Midelt","Saidia","Sefrou","Settat","Temara","Tichka","Tichla","Tiflet","Tiznit","Zagora",
"Berkane","Bouarfa","Demnate","Guelmim","Guercif","Kenitra","Larache","Layoune","Tan-Tan","Tangier","Tarfaya","Tetouan","Tinghir","Zemamra","Akhfenir",
"Bab_Taza","Boujdour","Goulmima","Imzouren","Inezgane","Khenifra","Laayoune","Ouazzane","Skhirate","Tafraout","Taourirt","Berrechid","Boulemane","Chichaoua",
"El_Jadida","Essaouira","Khemisset","Khouribga","Laâyoune","Marrakech","Sidi_Ifni","Taroudant","Al_Hoceima","Bab_Berred","Ben_guerir","Bir_Lehlou","Bni_Hadifa",
"Casablanca","Errachidia","Guerguerat","Mohammedia","Ouarzazate","Sidi_Kacem","Youssoufia","Ait_Melloul","Beni_Mellal","Chefchaouen","Oujda_Angad","Oulad_Teima",
"Sidi_Bouzid","Souk_Larbaa","had_soualem","Béni_Mellal","Bir_Anzerane","Fquih_Ben_Salah","Sidi_Bennour","Sidi_Slimane","Moulay_Bousselham","Guelta_Zemmur","Jorf_El_Melha",
"Ksar_es_Seghir","Kalaat_MGouna","Ksar_El_Kebir",};


char *product_names[94] = {"Fig","Yam","Beet","Date","Dill","Kale","Kiwi","Lime","Mint","Okra","Pear","Peas","Plum","Sage","Apple","Basil","Chard","Guava",
"Lemon","Mango","Onion","Peach","Thyme","Banana","Carrot","Celery","Cherry","Endive","Garlic","Ginger","Grapes","Jicama","Kiwano","Orange","Papaya","Potato",
"Radish","Tomato","Turnip","Apricot","Avocado","Cabbage","Coconut","Currant","Lettuce","Oregano","Parsley","Parsnip","Pumpkin","Rhubarb","Salsify","Spinach",
"Bok_Choy","Broccoli","Cilantro","Cucumber","Eggplant","Honeydew","Kohlrabi","Plantain","Rosemary","Rutabaga","Zucchini","Artichoke","Asparagus","Blueberry",
"Cranberry","Jackfruit","Nectarine","Persimmon","Pineapple","Raspberry","Starfruit","Blackberry","Cantaloupe","Clementine","Goji_Berry","Grapefruit","Strawberry",
"Watercress","Watermelon","Bell_Pepper","Cactus_Pear","Cauliflower","Green_Beans","Pomegranate","Acorn_Squash","Dragon_Fruit","Sweet_Potato","Butternut_Squash",
"Brussels_Sprouts","Passion_Fruit","Collard_Greens","Squash_Blossom",};


unsigned long long index_cityy[101] = {70516,2050592,2569137,2569319,2599540,63228520,63776479,74338393,75024020,76548769,78715750,79996310,1959006674,
1967004116,1969325289,1970334684,1976799335,2039654831,2055066233,2104194543,2187620067,2215325383,2301016841,2304493957,2469028577,2472646948,2473065509,
2501468372,2504871056,2504871087,2504964106,2505561972,2669383902,61575352956,61864133204,63345832272,66458973180,66459142309,69559150798,70335576988,70342476193,
77430348597,77432096784,77435753992,77552404246,77661129155,82870537437,1886357559898,1904816379216,1917796033322,2055361213007,2108757596927,2109053614027,
2158742802418,2179938725930,2280227873885,2381587759577,2400175897318,2400436830828,59174117967394,59451735135980,60101132594851,61907367979025,62118378258070,
66920998634291,66930110701794,67431728718338,68445865463980,73770627028988,74416035097006,1813368543470432,1830528028577182,1834270817764056,1837791644690647,
1841808368017819,1857438062272225,1924788966779924,1979884434209161,2133472435601810,2191291887131382,2286889439586129,2451121714680089,56153619371702011,
56862662231455076,57753855147144453,67937317046695999,67938942970720662,70893572382993329,71066764562071602,87893993349480824,1624758695981406047,
1766117476405423410,1876134001560909403,2197700743579685957,2197700758862991511,2664128815900080819,3642356851986173317,5863372202564488027,5996469818771313383,
6290269328787513140,6739052488524054943,};

unsigned long long index_productt[94] = {70628,88645,2066514,2122702,2130149,2330991,2339024,2368501,2398338,2459947,2483462,2483463,2490804,2569164,63476538,
63955991,65071022,69152510,73304787,74109858,76339625,76986961,80793789,1982479237,2011243939,2014746882,2017321401,2080082493,2125754574,2133018664,
2140951720,2218572881,2247797897,2369983054,2383347274,2396395591,2440255593,2510703658,2516405892,61002712322,61171535651,62333295361,62735427221,
62921131605,70452523806,73472954845,73886136526,73886138563,74454011720,75863801870,76543103591,76969613540,1917498593072,1920280274733,1939717953918,
1950128758662,1992838803878,2082667310875,2165039631422,2299741693723,2357936237770,2363285866857,2582902117879,58680406567102,58704153770195,59368993897180,
60369468208026,65873684234101,69395551873671,71114625117714,71220717455306,72711151654314,74070868635874,1839887038027415,1857317333566666,1866445425426758,
1974896387389694,1977213864912329,2296650026040687,2386262018367519,2386262027222044,56861039073233118,57567473473743288,57582594402227919,61296746479992391,
68600899684962659,1735729339500077813,1823869699111280360,2209199170557807766,3949527336957630577,7595390516999049506,10234803258330294252,10694272144807399784,15788840526560230435,};

inline int binary_search(unsigned long long *arr, int low, int high, unsigned long target) {
    while (low <= high) {
        int mid = (low + high) / 2;

        if (arr[mid] == target) {
            return mid;
        }if (arr[mid] < target) {
            low = mid + 1;
            continue;
        }
        high = mid - 1;
    }

    return -1; // Target not found
}

inline void fill(struct Node **__hashMap_tmp, size_t __unit_start, size_t __unit_end)
{
    unsigned long city_hash;
    unsigned long product_hash;
    unsigned long  value;
    for(;__unit_start < __unit_end;)
    {
        city_hash = 0;
        product_hash = 0;
        value = 0;

        for(;the_main[__unit_start] != ',';)
        {
            city_hash = (city_hash * 31 + the_main[__unit_start]);
            __unit_start++;
        }
        __unit_start++;
        for(;the_main[__unit_start] != ',';)
        {
            product_hash = (product_hash * 31 + the_main[__unit_start]);
            __unit_start++;
        }
        __unit_start++;
        for(;the_main[__unit_start] != '\n';)
        {
            int start = 0;
            int end = 0;
            for(;the_main[__unit_start] && the_main[__unit_start] != '.';)
            {
                start = start * 10 + the_main[__unit_start] - '0';
                __unit_start++;
            }
            int cont = 0;
            value = start * 100;
            if(the_main[__unit_start] == '\n')
                break;
            __unit_start++;
            for(;the_main[__unit_start] != '\n';)
            {
                end = end * 10 + the_main[__unit_start] - '0';
                cont++;
                __unit_start++;
            }
            if (cont == 1)
                end *= 10;
            value += end;
        }
        __unit_start++;
        int city_index = binary_search(index_cityy,0, 101, city_hash);
        int product_index =  binary_search(index_productt,0, 94, product_hash);
        if (__hashMap_tmp[city_index] == NULL)
        {

            __hashMap_tmp[city_index] = (struct Node *)malloc(sizeof(struct Node));
            __hashMap_tmp[city_index]->total = value;
            __hashMap_tmp[city_index]->city = city_names[city_index];
            __hashMap_tmp[city_index]->product[product_index] = product_names[product_index];
            __hashMap_tmp[city_index]->price[product_index] = value;
            continue;
        }
            __hashMap_tmp[city_index]->total += value;
            if(__hashMap_tmp[city_index]->price[product_index] == 0)
            {
                __hashMap_tmp[city_index]->product[product_index] = product_names[product_index];
                __hashMap_tmp[city_index]->price[product_index] = value;
                continue;
            }
            if (__hashMap_tmp[city_index]->price[product_index] >  value)
                __hashMap_tmp[city_index]->price[product_index] = value;
    }
    pthread_exit(0);
}


inline void *runner(void *arg)
{
    int i = *((int *)arg);
    struct Node **__hashMap_tmp;
    size_t __unit_start = 0;
    size_t __unit_end = 0;
    if (i == 1)
    {
        __unit_end = __offset;
        for(;the_main[__unit_end] != '\n'; __unit_end++);
        __hashMap_tmp = hashMap;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    if (i == 2)
    {
        __unit_start = __offset;
        __unit_end = __offset * 2;
        for(;the_main[__unit_start] != '\n'; __unit_start++);
        __unit_start++;
        for(;the_main[__unit_end] != '\n'; __unit_end++);
        __hashMap_tmp = hashMap2;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    if (i == 3)
    {
        __unit_start = __offset * 2;
        __unit_end = __offset * 3;
        for(;the_main[__unit_start] != '\n'; __unit_start++);
        __unit_start++;
        for(;the_main[__unit_end] != '\n'; __unit_end++);
        __hashMap_tmp = hashMap3;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    if (i == 4)
    {
        __unit_start = __offset * 3;
        __unit_end = __offset * 4;
        for(;the_main[__unit_start] != '\n'; __unit_start++);
        __unit_start++;
        for(;the_main[__unit_end] != '\n'; __unit_end++);
        __hashMap_tmp = hashMap4;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    if(i == 5)
    {
        __unit_start = __offset * 4;
        __unit_end = __offset * 5;
        for(;the_main[__unit_start] != '\n'; __unit_start++);
        __unit_start++;
        for(;the_main[__unit_end] != '\n'; __unit_end++);
        __hashMap_tmp = hashMap5;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    if(i == 6)
    {
        __unit_start = __offset * 5;
        __unit_end = __offset * 6;
        for(;the_main[__unit_start] != '\n'; __unit_start++);
        __unit_start++;
        for(;the_main[__unit_end] != '\n'; __unit_end++);
        __hashMap_tmp = hashMap6;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    if(i == 7)
    {
        __unit_start = __offset * 6;
        __unit_end = __offset * 7;
        for(;the_main[__unit_start] != '\n'; __unit_start++);
        __unit_start++;
        for(;the_main[__unit_end] != '\n'; __unit_end++);
        __hashMap_tmp = hashMap7;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    if(i == 8)
    {
        __unit_start = __offset * 7;
        __unit_end = __offset * 8;
        for(;the_main[__unit_start] != '\n'; __unit_start++);
        __unit_start++;
        for(;the_main[__unit_end] != '\n'; __unit_end++);
        __hashMap_tmp = hashMap8;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    if(i == 9)
    {
        __unit_start = __offset * 8;
        __unit_end = __offset * 9;
        for(;the_main[__unit_start] != '\n'; __unit_start++);
        __unit_start++;
        for(;the_main[__unit_end] != '\n'; __unit_end++);
        __hashMap_tmp = hashMap9;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    if(i == 10)
    {
        __unit_start = __offset * 9;
        __unit_end = __offset * 10;
        for(;the_main[__unit_start] != '\n'; __unit_start++);
        __unit_start++;
        for(;the_main[__unit_end] != '\n'; __unit_end++);
        __hashMap_tmp = hashMap10;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    if(i == 11)
    {
        __unit_start = __offset * 10;
        __unit_end = __offset * 11;
        for(;the_main[__unit_start] != '\n'; __unit_start++);
        __unit_start++;
        for(;the_main[__unit_end] != '\n'; __unit_end++);
        __hashMap_tmp = hashMap11;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    if(i == 12)
    {
        __unit_start = __offset * 11;
        __unit_end = __offset * 12;
        for(;the_main[__unit_start] != '\n'; __unit_start++);
        __unit_start++;
        for(;the_main[__unit_end] != '\n'; __unit_end++);
        __hashMap_tmp = hashMap12;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    if(i == 13)
    {
        __unit_start = __offset * 12;
        __unit_end = __offset * 13;
        for(;the_main[__unit_start] != '\n'; __unit_start++);
        __unit_start++;
        for(;the_main[__unit_end] != '\n'; __unit_end++);
        __hashMap_tmp = hashMap13;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    if(i == 14)
    {
        __unit_start = __offset * 13;
        __unit_end = __offset * 14;
        for(;the_main[__unit_start] != '\n'; __unit_start++);
        __unit_start++;
        for(;the_main[__unit_end] != '\n'; __unit_end++);
        __hashMap_tmp = hashMap14;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    if(i == 15)
    {
        __unit_start = __offset * 14;
        __unit_end = __offset * 15;
        for(;the_main[__unit_start] != '\n'; __unit_start++);
        __unit_start++;
        for(;the_main[__unit_end] != '\n'; __unit_end++);
        __hashMap_tmp = hashMap15;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    if(i == 16)
    {
        __unit_start = __offset * 15;
        __unit_end = file_size;
        for(;the_main[__unit_start] != '\n'; __unit_start++);
        __unit_start++;
        __hashMap_tmp = hashMap16;
        fill(__hashMap_tmp, __unit_start, __unit_end);
    }
    pthread_exit(0);

}

inline void merging(struct Node **__hashMap_tmp1, struct Node **__hashMap_tmp2)
{
    int i = 0;
    for(int x = 0; x < 101; x++)
    {
        if(__hashMap_tmp2[x] != NULL)
        {
            if(__hashMap_tmp1[x] == NULL)
            {
                __hashMap_tmp1[x] = (struct Node *)malloc(sizeof(struct Node));
                __hashMap_tmp1[x]->total = __hashMap_tmp2[x]->total;
                __hashMap_tmp1[x]->city = __hashMap_tmp2[x]->city;
                for(int y = 0; y < 94; y++)
                {
                    __hashMap_tmp1[x]->product[y] = __hashMap_tmp2[x]->product[y];
                    __hashMap_tmp1[x]->price[y] = __hashMap_tmp2[x]->price[y];
                }
                continue;
            }
            else
            {
                __hashMap_tmp1[x]->total += __hashMap_tmp2[x]->total;
                for(int y = 0; y < 94; y++)
                {
                    if(__hashMap_tmp2[x]->price[y] != 0)
                    {
                        if(__hashMap_tmp1[x]->price[y] == 0)
                        {
                            __hashMap_tmp1[x]->product[y] = __hashMap_tmp2[x]->product[y];
                            __hashMap_tmp1[x]->price[y] = __hashMap_tmp2[x]->price[y];
                            continue;
                        }
                        if (__hashMap_tmp1[x]->price[y] >  __hashMap_tmp2[x]->price[y])
                            __hashMap_tmp1[x]->price[y] = __hashMap_tmp2[x]->price[y];
                    }
                }
            }
        }
    }
    return;
}

inline void *__collector(void *arg)
{
    int i = *((int *)arg);
    struct Node **__hashMap_tmp1;
    struct Node **__hashMap_tmp2;
    if (i == 1)
    {
        __hashMap_tmp1 = hashMap;
        __hashMap_tmp2 = hashMap2;
    }
    else if (i == 2)
    {
        __hashMap_tmp1 = hashMap3;
        __hashMap_tmp2 = hashMap4;
    }
    else if (i == 3)
    {
        __hashMap_tmp1 = hashMap5;
        __hashMap_tmp2 = hashMap6;
    }
    else if (i == 4)
    {
        __hashMap_tmp1 = hashMap7;
        __hashMap_tmp2 = hashMap8;
    }
    else if(i == 5)
    {
        __hashMap_tmp1 = hashMap9;
        __hashMap_tmp2 = hashMap10;
    }
    else if(i == 6)
    {
        __hashMap_tmp1 = hashMap11;
        __hashMap_tmp2 = hashMap12;
    }
    else if(i == 7)
    {
        __hashMap_tmp1 = hashMap13;
        __hashMap_tmp2 = hashMap14;
    }
    else if(i == 8)
    {
        __hashMap_tmp1 = hashMap15;
        __hashMap_tmp2 = hashMap16;
    }
    merging(__hashMap_tmp1, __hashMap_tmp2);
    pthread_exit(0);
    
}

inline void *__collector2(void *arg)
{
    int i = *((int *)arg);
    struct Node **__hashMap_tmp1;
    struct Node **__hashMap_tmp2;
    if (i == 1)
    {
        __hashMap_tmp1 = hashMap;
        __hashMap_tmp2 = hashMap3;
    }
    else if (i == 2)
    {
        __hashMap_tmp1 = hashMap5;
        __hashMap_tmp2 = hashMap7;
    }
    else if (i == 3)
    {
        __hashMap_tmp1 = hashMap9;
        __hashMap_tmp2 = hashMap11;
    }
    else if (i == 4)
    {
        __hashMap_tmp1 = hashMap13;
        __hashMap_tmp2 = hashMap15;
    }
    merging(__hashMap_tmp1, __hashMap_tmp2);
    pthread_exit(0);
    
}

inline void *__collector3(void *arg)
{
    int i = *((int *)arg);
    struct Node **__hashMap_tmp1;
    struct Node **__hashMap_tmp2;
    if (i == 1)
    {
        __hashMap_tmp1 = hashMap;
        __hashMap_tmp2 = hashMap5;
    }
    else if (i == 2)
    {
        __hashMap_tmp1 = hashMap9;
        __hashMap_tmp2 = hashMap13;
    }
    merging(__hashMap_tmp1, __hashMap_tmp2);
    pthread_exit(0);
    
}

inline void the_cheapest_product(struct Node *node, int fd, FILE *file)
{
    unsigned long min = 0;
    int index = 0;
    for(int i = 0; i < 102; i++)
    {
        if(node->price[i] != 0)
        {
            if(min == 0)
            {
                min = node->price[i];
                index = i;
                continue;
            }
            if (min > node->price[i])
            {
                min = node->price[i];
                index = i;
                continue;
            }
            if(min == node->price[i])
            {
                if(strcmp(node->product[i], node->product[index]) < 0)
                {
                    index = i;
                }
            }
        }
    }
    node->price[index] = 200;
    int fractinal = min % 100;
    if(fractinal < 10)
        fprintf(file, "%s %lu.0%lu", node->product[index], min/100, min%100);
    else
        fprintf(file, "%s %lu.%lu", node->product[index], min/100, min%100);

}

inline void the_cheapest_city()
{
    FILE *out = fopen("output.txt", "w");
    int fd = fileno(out);
    unsigned long min = 0;
    int index = 0;
    for(int i = 0; i < 102; i++)
    {
        if(hashMap[i] != NULL)
        {
            if(min == 0)
            {
                min = hashMap[i]->total;
                index = i;
                continue;
            }
            if (min > hashMap[i]->total)
            {
                min = hashMap[i]->total;
                index = i;
            }
        }
    }
    fprintf(out, "%s ", hashMap[index]->city);
    if(min%100 < 10)
        fprintf(out, "%lu.0%lu\n", min/100, min%100);
    else
        fprintf(out, "%lu.%lu\n", min/100, min%100);
    for(int i = 0; i < 5; i++)
    {
        the_cheapest_product(hashMap[index], fd, out);
        if(i != 4)
            fprintf(out, "\n");
    }
    fclose(out);
}


int main()
{
    pthread_t tid[num_threads];
    const char *file_path = "input.txt";
    int fd = open(file_path, O_RDONLY);
    file_size = lseek(fd, 0, SEEK_END);
    char * file_content = static_cast<char *>(mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0));
    size_t __unit = 0;
    __offset = file_size / num_threads;
    the_main = file_content;
    int tiid[num_threads];
    for(int i = 1; i <= num_threads; i++)
    {
        tiid[i - 1] = i;
    }
    int i = 0;
    while (i < num_threads) {
        pthread_create(&tid[i], NULL, runner, &tiid[i]);
        i++;
    }
    i = 0;
    while (i < num_threads) {
        pthread_join(tid[i], NULL);
        i++;
    }
    i = 0;
    while (i < num_threads/2) {
        pthread_create(&tid[i], NULL, __collector, &tiid[i]);
        i++;
    }
    i = 0;
    while (i < num_threads/2) {
        pthread_join(tid[i], NULL);
        i++;
    }
    for(int i = 0; i < num_threads/4; i++)
    {
        pthread_create(&tid[i], NULL, __collector2, &tiid[i]);
    }
    for(int i = 0; i < num_threads/4; i++)
    {
        pthread_join(tid[i], NULL);
    }
    for(int i = 0; i < num_threads/8; i++)
    {
        pthread_create(&tid[i], NULL, __collector3, &tiid[i]);
    }
    for(int i = 0; i < num_threads/8; i++)
    {
        pthread_join(tid[i], NULL);
    }
    merging(hashMap, hashMap9);
    the_cheapest_city();
    munmap(file_content, file_size);
    close(fd);
    return 0;
}