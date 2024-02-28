#pragma GCC optimize("Ofast")
#pragma GCC optimize("O3")
#include <iostream>
#include <sys/mman.h>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <vector>

unsigned int num_threads = std::thread::hardware_concurrency() * 2;

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

unsigned long long  index_productt[94] = {70628,88645,2066514,2122702,2130149,2330991,2339024,2368501,2398338,2459947,2483462,2483463,2490804,2569164,63476538,
63955991,65071022,69152510,73304787,74109858,76339625,76986961,80793789,1982479237,2011243939,2014746882,2017321401,2080082493,2125754574,2133018664,
2140951720,2218572881,2247797897,2369983054,2383347274,2396395591,2440255593,2510703658,2516405892,61002712322,61171535651,62333295361,62735427221,
62921131605,70452523806,73472954845,73886136526,73886138563,74454011720,75863801870,76543103591,76969613540,1917498593072,1920280274733,1939717953918,
1950128758662,1992838803878,2082667310875,2165039631422,2299741693723,2357936237770,2363285866857,2582902117879,58680406567102,58704153770195,59368993897180,
60369468208026,65873684234101,69395551873671,71114625117714,71220717455306,72711151654314,74070868635874,1839887038027415,1857317333566666,1866445425426758,
1974896387389694,1977213864912329,2296650026040687,2386262018367519,2386262027222044,56861039073233118,57567473473743288,57582594402227919,61296746479992391,
68600899684962659,1735729339500077813,1823869699111280360,2209199170557807766,3949527336957630577,7595390516999049506,10234803258330294252,10694272144807399784,15788840526560230435,};


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

struct data_chunk
{
    char *city;
    char *product[102];
    unsigned long price[102];
    unsigned long total;
};

struct thread_data
{
    int conter_product;
    int thread_id;
    char * file_content;
    unsigned long file_size;
    int num_threads;
    int target;
    struct data_chunk *data[102];
};

struct mqadem
{
    struct thread_data * data1;
    struct thread_data * data2;
};



int binary_search(unsigned long long *arr, int low, int high, unsigned long target) {
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
void save_data(struct thread_data * data, unsigned long price, unsigned long _hash_city, unsigned long _hash_product)
{
    int index_city = binary_search(index_cityy, 0, 101, _hash_city);
    int index_product = binary_search(index_productt, 0, 94, _hash_product);
    if(data->data[index_city] == NULL)
    {
        data->data[index_city] = (struct data_chunk *)malloc(sizeof(struct data_chunk));
        for(int i = 0; i < 102; i++)
        {
            data->data[index_city]->product[i] = NULL;
            data->data[index_city]->price[i] = 0;
        }
        data->data[index_city]->city = city_names[index_city];
        data->data[index_city]->total = price;
        data->data[index_city]->product[index_product] = product_names[index_product];
        data->data[index_city]->price[index_product] = price;
    }
    else
    {
        data->data[index_city]->total += price;
        if(data->data[index_city]->price[index_product] == 0)
        {
            data->data[index_city]->product[index_product] = product_names[index_product];
            data->data[index_city]->price[index_product] = price;
        }
        else
        {
            if(data->data[index_city]->price[index_product] > price)
                data->data[index_city]->price[index_product] = price;
        }
    }
}


void * read_file(void * threadarg)
{
    struct thread_data * my_data;
    my_data = (struct thread_data *) threadarg;
    int thread_id = my_data->thread_id;
    char * file_content = my_data->file_content;
    unsigned long file_size = my_data->file_size;
    unsigned long num_threads = my_data->num_threads;
    unsigned long chunk_size = file_size / num_threads;
    unsigned long start = thread_id * chunk_size;
    unsigned long end = (thread_id + 1) * chunk_size;
    if (thread_id == num_threads - 1)
        end = file_size;
    else
        for(;file_content[end] != '\n'; end++);
    if(thread_id != 0)
    {
        for(;file_content[start] != '\n'; start++);
        start++;
    }
    else
        start = 0;
    unsigned long value;
    for(;start < end; start++)
    {
        unsigned long hash_city = 0;
        unsigned long hash_product = 0;
        value = 0;
        int i = 0;
        for(;file_content[start] != ',';i++)
        {
            hash_city = (hash_city * 31 + file_content[start]);
            start++;
        }
        start++;
        i = 0;
        for(;file_content[start] != ',';i++)
        {
            hash_product = (hash_product * 31 + file_content[start]);
            start++;
        }
        start++;
        i = 0;
        for(;file_content[start] != '\n';i++)
        {
            unsigned long __start = 0;
            unsigned long __end = 0;
            for(;file_content[start] && file_content[start] != '.';)
            {
                __start = __start * 10 + file_content[start] - '0';
                start++;
            }
            int cont = 0;
            value = __start * 100;
            if(file_content[start] == '\n')
                break;
            start++;
            for(;file_content[start] != '\n';)
            {
                __end = __end * 10 + file_content[start] - '0';
                cont++;
                start++;
            }
            if (cont == 1)
                __end *= 10;
            value += __end;
        }
        save_data(my_data, value, hash_city, hash_product);
    }
    pthread_exit(NULL);
}

void *merge(void *arg)
{
    struct mqadem * my_data;
    my_data = (struct mqadem *) arg;
    struct thread_data * data1 = my_data->data1;
    struct thread_data * data2 = my_data->data2;
    for(int i = 0; i < 101; i++)
    {
        if(data1->data[i] == NULL)
        {
            data1->data[i] = data2->data[i];
            continue;
        }
        if(data2->data[i] == NULL)
            continue;
        data1->data[i]->total += data2->data[i]->total;
        for(int j = 0; j < 94; j++)
        {
            if(data2->data[i]->product[j] == NULL)
                continue;
            if(data1->data[i]->product[j] == NULL)
            {
                data1->data[i]->product[j] = data2->data[i]->product[j];
                data1->data[i]->price[j] = data2->data[i]->price[j];
            }
            else
            {
                if(data1->data[i]->price[j] > data2->data[i]->price[j])
                    data1->data[i]->price[j] = data2->data[i]->price[j];
            }
        }
    }
    pthread_exit(NULL);
}

void the_cheapest_product(struct data_chunk * data, int fd, FILE *out)
{
    unsigned long min = 0;
    int index = 0;
    for(int i = 0; i < 101; i++)
    {
        if(data->price[i] != 0)
        {
            if(min == 0)
            {
                min = data->price[i];
                index = i;
                continue;
            }
            if(min > data->price[i])
            {
                min = data->price[i];
                index = i;
                continue;
            }
            if(min == data->price[i])
            {
                if(strcmp(data->product[index], data->product[i]) > 0)
                {
                    min = data->price[i];
                    index = i;
                }
            }
        
        }
    }
    data->price[index] = 200;
    unsigned long fractinal = min % 100;
    if(fractinal < 10)
        fprintf(out, "%s %lu.0%lu", data->product[index], min / 100, fractinal);
    else
        fprintf(out, "%s %lu.%lu", data->product[index], min / 100, fractinal);
    
}

void the_cheapest_city(struct data_chunk * data[102])
{
    FILE *out = fopen("output.txt", "w");
    int fd = fileno(out);
    unsigned long min = 0;
    int index = 0;
    for(int i = 0; i < 101; i++)
    {
        if(data[i] == NULL)
            continue;
        if(min == 0)
        {
            min = data[i]->total;
            index = i;
        }
        else
        {
            if(min > data[i]->total)
            {
                min = data[i]->total;
                index = i;
            }
        }
    }
    fprintf(out, "%s ", data[index]->city);
    if(min %100 < 10)
        fprintf(out, "%lu.0%lu\n", min / 100, min % 100);
    else
        fprintf(out, "%lu.%lu\n", min / 100, min % 100);
    for(int i = 0; i < 5; i++)
    {
        the_cheapest_product(data[index], fd, out);
        if(i != 4)
            fprintf(out, "\n");
    }
    fclose(out);
}

int main()
{
    int fd = open("input.txt", O_RDONLY);
    pthread_t threads[num_threads];
    struct thread_data td[num_threads];
    off_t file_size = lseek(fd, 0, SEEK_END);
    char * file_content = static_cast<char *>(mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0));
    for(int i = 0; i < num_threads; i++)
    {
        td[i].thread_id = i;
        td[i].file_content = file_content;
        td[i].file_size = file_size;
        td[i].num_threads = num_threads;
        for(int j = 0; j < 102; j++)
        {
            td[i].data[j] = NULL;
        }
        pthread_create(&threads[i], NULL, read_file, (void *)&td[i]);
    }
    for(int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    munmap(file_content, file_size);
    std::vector<int> vec;
    int start = 2;
    int thre = num_threads;
    while(start < num_threads)
    {
        struct mqadem data[num_threads/start];   
        int first = 0;
        int second = first + 1 * (start / 2);
        int i = 0;
        while(second < num_threads)
        {
            if (second + start / 2 > num_threads)
                break;
            data[i].data1 = &td[first];
            data[i].data2 = &td[second];
            pthread_create(&threads[i], NULL, merge, (void *)&data[i]);
            first += start;
            second = first + 1 * (start / 2);
            i++;
        }
        for(int j = 0; j < i; j++)
        {
            pthread_join(threads[j], NULL);
        }
        if (thre % 2 != 0)
           vec.push_back(first);
        thre /= 2;
        start*=2;
    }
    for(int i = 0; i < vec.size(); i++)
    {
        struct mqadem data;
        data.data1 = &td[0];
        data.data2 = &td[vec[i]];
        pthread_create(&threads[i], NULL, merge, (void *)&data);
        pthread_join(threads[i], NULL);
    }
    the_cheapest_city(td[0].data);
    close(fd);
    return 0;
}