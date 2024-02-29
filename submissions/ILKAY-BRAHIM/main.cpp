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

struct data_chunk
{
    int id_city;
    int id_product[94];
    unsigned long price[102];
    unsigned long total;
};


struct thread_data
{
    int procs_id;
    const char * file_content;
    unsigned long file_size;
    int num_procs;
    struct data_chunk **data;
};

struct mqadem
{
    struct thread_data * data1;
    struct thread_data * data2;
};


//========================================citys hash genrated by tool gperf========================================
struct stringIndexPair {
    const char* string;
    unsigned int index;
};

static unsigned int hash_city (const char *str, int len)
{
  static short asso_values[] =
    {307, 307, 307, 307, 307, 307, 307, 307, 307, 307,307, 307, 307, 307, 307, 307, 307, 307, 307, 307,307,
     307, 307, 307, 307, 307, 307, 307, 307, 307,307, 307, 307, 307, 307, 307, 307, 307, 307, 307,307, 307, 
     307, 307, 307, 307, 307, 307, 307, 307,307, 307, 307, 307, 307, 307, 307, 307, 307, 307,307, 307, 307, 
     307, 307, 307, 105,  50, 307,  25,307, 307, 307,  65, 307,  35,   5,   0,   5, 307,307, 307, 307,  25, 
     307,  25, 307, 307, 307, 307,307, 307, 307, 307, 307,  65,   0,   0,  40,  95, 30,   5,  25, 120,   0,   
     5,  55,  70,  60,  30, 10,  15,   5,  10,   0,  20,  30,   0,  60, 307,307,  10,  20,   0, 307, 307, 307, 
     307, 307, 307,307, 307, 307, 307, 307, 307, 307, 307, 307, 307,307, 307, 307, 307, 307, 307, 307, 307, 307, 
     307,307, 307, 307, 307, 307, 307, 307, 307, 307, 307,307, 307, 307, 307, 307, 307, 307, 307, 307,   0,307, 
     307, 307, 307, 307, 307, 307, 307, 307, 307,307, 307, 307, 307, 307, 307, 307, 307, 307, 307,307, 307, 307, 
     307, 307,  15, 307, 307, 307, 307,307, 307, 307, 307, 307, 307, 307, 307, 307, 307,307, 307, 307, 307, 307, 
     307, 307, 307, 307, 307,307, 307, 307, 307, 307, 307, 307, 307, 307, 307,307, 307, 307, 307, 307, 307, 307, 
     307, 307, 307,307, 307, 307, 307, 307, 307, 307, 307, 307, 307,307, 307, 307, 307, 307, 307, 307
    };
  int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[5]];
      case 5:
        hval += asso_values[(unsigned char)str[4]+1];
      case 4:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      case 2:
        hval += asso_values[(unsigned char)str[1]];
        break;
    }
  return hval;
}

struct stringIndexPair *find_index_city (const char *str, int len)
{
  static struct stringIndexPair wordlist[] =
    {{""}, {""}, {""}, {""}, {""}, {""},{"Drarga", 18},{""},{"Ouazzane", 55},{""},{"Ouarzazate", 79},{""}, 
    {""},{"Laayoune", 54},{""},{"Guerguerat", 77},{"Jerada", 21},{""},{"Guelta_Zemmur", 96},{"Chichaoua", 61},
    {"Bir_Lehlou", 73},{""},{"Kenitra", 38},{"Inezgane", 52},{"Taza", 4},{"Ben_guerir", 72},{"Beni_Mellal", 83},
    {"Guelmim", 36},{"Fes", 0},{"Safi", 2},{""},{"Souk_Larbaa", 88},{"Tinghir", 45},{""}, {""}, {""}, {""},{"Larache", 39},
    {"Goulmima", 50},{"Ksar_es_Seghir", 98},{"Errachidia", 76},{"Sefrou", 25},{"Tan-Tan", 41},{"Boujdour", 49},{"Assa", 1},
    {"Rabat", 10},{"Chefchaouen", 84},{"Guercif", 37},{"Taourirt", 58},{"Khouribga", 65},{"Ahfir", 5},{"Ifrane", 20},{"Zemamra", 46},
    {"Jorf_El_Melha", 97},{"Essaouira", 63},{"Nador", 8},{"Ait_Melloul", 82},{"Tarfaya", 43},{"Ksar_El_Kebir", 100},{"La\303\242youne", 66},
    {"Midar", 7},{"Temara", 27},{"Berkane", 33},{""},{"Sale", 3},{"Mohammedia", 78},{"Saidia", 24},{"Bouarfa", 34},{""}, {""},{"Youssoufia", 81},
    {"Asilah", 15},{"Sidi_Slimane", 94},{"Bab_Taza", 48},{""},{"Smara", 11},{"Aourir", 13},{"Tangier", 42},{""},{"Boulemane", 60},{"Sidi_Kacem", 80},
    {""},{"Moulay_Bousselham", 95},{""}, {""},{"Azrou", 6},{"had_soualem", 89},{"Layoune", 40},{"Tafraout", 57},{"Khemisset", 64},{"Bni_Hadifa", 74},
    {"Tiflet", 30},{""},{"Khenifra", 53},{""},{"Fquih_Ben_Salah", 92},{""}, {""},{"Skhirate", 56},{"Taroudant", 69},{"Oujda", 9},{"Midelt", 23},
    {"Tetouan", 44},{""}, {""},{"Bab_Berred", 71},{"Dakhla", 17},{""}, {""},{"Sidi_Ifni", 68},{""},{"Settat", 26},{"Demnate", 35},{"Akhfenir", 47},{""}, 
    {""},{"Tiznit", 31},{""},{"Imzouren", 51},{"Marrakech", 67},{""},{"Arfoud", 14},{""}, {""}, {""}, {""},{"Meknes", 22},{""}, {""}, {""}, {""},{"Azilal", 16},
    {"Bir_Anzerane", 91},{""},{"Berrechid", 59},{""},{"Tichla", 29},{""}, {""}, {""}, {""},{"Oulad_Teima", 86},{""},{"Kalaat_MGouna", 99},{""}, {""},{"Zagora", 32},
    {"B\303\251ni_Mellal", 90},{""}, {""}, {""},{"Sidi_Bouzid", 87},{"Sidi_Bennour", 93},{""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},{""}, {""}, {""}, {""},
    {"Tichka", 28},{""}, {""}, {""}, {""},{"Oujda_Angad", 85},{""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},{"Agadir", 12},{""}, {""}, {""},{"Casablanca", 75},{""}, 
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},{""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},{"El_Jadida", 62},{""}, {""}, {""}, {""}, {""}, {""}, 
    {""}, {""}, {""},{""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},{""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},{""}, {""}, {""},{"Al_Hoceima", 70},
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},{""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},{""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},{""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},{""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},{""}, {""}, {""}, {""}, {""}, {""}, {""},{"Figuig", 19}
    };

    int key = hash_city (str, len);
  return &wordlist[key];
}


//========================================products hash genrated by tool gperf========================================

static int hash_product (const char *str, int len)
{
  static unsigned char asso_values[] =
    {140, 140, 140, 140, 140, 140, 140, 140, 140, 140,140, 140, 140, 140, 140, 140, 140, 140, 140, 140,140, 140, 140, 140, 140, 140, 140,
     140, 140, 140,140, 140, 140, 140, 140, 140, 140, 140, 140, 140,140, 140, 140, 140, 140, 140, 140, 140, 140, 140,140, 140, 140, 140,
      140, 140, 140, 140, 140, 140,140, 140, 140, 140, 140,  40,  40,   0,  75,   0,  0,  15,  20, 140,  55,  50,  20,  30,  35,  95,  0,
       140,  10,  25,  65, 140, 140,  20, 140,   5,  5, 140, 140, 140, 140, 140, 140,   0,   0,  65, 60,   0, 140,  65,  40,   5,  10,   0,
         30,  50,  5,   0,  30, 140,   5,  20,   0,   0, 140,  55,140,  20, 140, 140, 140, 140, 140, 140, 140, 140,140, 140, 140, 140, 140,
          140, 140, 140, 140, 140,140, 140, 140, 140, 140, 140, 140, 140, 140, 140,140, 140, 140, 140, 140, 140, 140, 140, 140, 140,140, 140,
           140, 140, 140, 140, 140, 140, 140, 140,140, 140, 140, 140, 140, 140, 140, 140, 140, 140,140, 140, 140, 140, 140, 140, 140, 140, 140,
            140,140, 140, 140, 140, 140, 140, 140, 140, 140, 140,140, 140, 140, 140, 140, 140, 140, 140, 140, 140,140, 140, 140, 140, 140, 140,
             140, 140, 140, 140,140, 140, 140, 140, 140, 140, 140, 140, 140, 140,140, 140, 140, 140, 140, 140, 140, 140, 140, 140,140, 140, 140,
              140, 140, 140, 140, 140, 140, 140,140, 140, 140, 140, 140, 140
    };
  return len + asso_values[(unsigned char)str[2]] + asso_values[(unsigned char)str[0]] + asso_values[(unsigned char)str[len - 1]];
}

struct stringIndexPair *find_index_product (const char *str, int len)
{
  static struct stringIndexPair wordlist[] =
    {{""}, {""}, {""}, {""}, {""}, {""},{"Potato", 35},{"Cabbage", 41},{""},{"Pear", 10},{"Clementine", 75},{"Carrot", 24},
    {"Currant", 43},{"Plantain", 59},{"Pineapple", 70},{"Cantaloupe", 74},{"Cauliflower", 83},{"Rhubarb", 49},{"Rutabaga", 61},
    {"Persimmon", 69},{"Guava", 17},{""}, {""}, {""},{"Peas", 11},{"Grapefruit", 77},{"Cherry", 26},{"Lettuce", 44},{""},{"Cranberry", 66},
    {""},{"Ginger", 29},{"Parsley", 46},{"Passion_Fruit", 91},{"Starfruit", 72},{"Watermelon", 80},{"Papaya", 34},{"Sweet_Potato", 88},
    {"Cilantro", 54},{"Mint", 8},{"Mango", 19},{"Grapes", 30},{"Parsnip", 47},{""},{"Beet", 2},{"Peach", 21},{"Green_Beans", 84},{"Avocado", 40},
    {""},{"Artichoke", 63},{"Watercress", 79},{"Banana", 23},{"Apricot", 39},{"Broccoli", 53},{"Plum", 12},{"Goji_Berry", 76},{"Celery", 25},{""},
    {"Rosemary", 60},{"Raspberry", 71},{"Strawberry", 78},{"Pomegranate", 85},{"Pumpkin", 48},{""},{"Collard_Greens", 92},{"Chard", 16},{"Endive", 27},
    {""},{"Bok_Choy", 52},{"Blueberry", 65},{"Blackberry", 73},{""},{"Coconut", 42},{"Eggplant", 56},{"Lime", 7},{"Apple", 14},{"Brussels_Sprouts", 90},
    {"Spinach", 51},{"Cucumber", 55},{"Date", 3},{"Lemon", 18},{"Cactus_Pear", 82},{"Salsify", 50},{"Zucchini", 62},{"Kale", 5},{""},{"Bell_Pepper", 81},
    {"Dragon_Fruit", 87},{"Honeydew", 57},{"Squash_Blossom", 93},{"Thyme", 22},{"Garlic", 28},{"Acorn_Squash", 86},{""},{"Sage", 13},{"Basil", 15},
    {"Butternut_Squash", 89},{""}, {""},{"Asparagus", 64},{""},{"Orange", 33},{"Oregano", 45},{"Kohlrabi", 58},{"Okra", 9},{""},{"Turnip", 38},{""},
    {"Yam", 1},{"Nectarine", 68},{"Onion", 20},{"Kiwano", 32},{""}, {""},{"Kiwi", 6},{""},{"Radish", 36},{""}, {""}, {""}, {""},{"Tomato", 37},{""}, 
    {""}, {""}, {""},{"Jicama", 31},{""}, {""},{"Jackfruit", 67},{""}, {""}, {""},{"Fig", 0},{""}, {""}, {""}, {""}, {""},{"Dill", 4}
    };

    int key = hash_product (str, len);
  return &wordlist[key];
}

unsigned int num_threads = std::thread::hardware_concurrency() * 2;


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

void save_data(struct thread_data *data, char *city, char *product, int len_city, int len_product, unsigned long long price)
{
    struct stringIndexPair *city_index = find_index_city(city, len_city);
    struct stringIndexPair *product_index = find_index_product(product, len_product);
    int index_city = city_index->index;
    int index_product = product_index->index;
    if(data->data[index_city]->total == 0)
    {
        for(int i = 0; i < 102; i++)
        {
            data->data[index_city]->id_product[i] = 0;
            data->data[index_city]->price[i] = 0;
        }
        data->data[index_city]->id_city = index_city;
        data->data[index_city]->id_product[index_product] = index_product;
        data->data[index_city]->price[index_product] = price;
        data->data[index_city]->total = price;
    }
    else
    {
        data->data[index_city]->total += price;
        if(data->data[index_city]->id_product[index_product] == 0)
        {
            data->data[index_city]->id_product[index_product] = index_product;
            data->data[index_city]->price[index_product] = price;
        }
        else
        {
            if(data->data[index_city]->price[index_product] > price)
                data->data[index_city]->price[index_product] = price;
        }
    }
    // printf ("%d %d %d %lu\n", data->data[index_city]->id_city,index_city, data->data[index_city]->id_product[index_product], data->data[index_city]->price[index_product]);

}

void read_file(struct thread_data *data)
{
    int procs_id = data->procs_id;
    const char *file_content = data->file_content;
    unsigned long file_size = data->file_size;
    int num_procs = data->num_procs;
    unsigned long chank_size = file_size / num_procs;
    unsigned long start = procs_id * chank_size;
    unsigned long end = (procs_id + 1) * chank_size;
    if(procs_id == num_procs - 1)
        end = file_size;
    else
        for(;file_content[end] != '\n'; end++);
    if(procs_id != 0)
    {
        for(;file_content[start] != '\n'; start++);
        start++;
    }
    unsigned long long value = 0;
    char line[50] = {0};
    unsigned long long offset[4] = {0, '0', 11 * '0', 111 * '0'};
    unsigned long long multi[3][4] = {{0, 1, 10, 100},{0, 0, 1,  10},{0, 0, 0,  1},};
    int i = 0;
    for(;start <= end; start++)
    {
        value = 0;
        if(file_content[start] == ',')
        {
            line[i] = '\0';
            i++;
        }
        else if(file_content[start] == '\n')
        {
            line[i] = '\0';
            i = 0;
            char *city = line;
            char *product =(char *)memchr(line, '\0', 50);
            product++;
            char *price = (char *)memchr(product, '\0', 50);
            price++;
            char *end = (char *)memchr(price, '\0', 50);
            int len_city = product - city - 1;
            int len_product = price - product - 1;
            int len_price = end - price;
            char *part_decemal = (char *)memchr(price, '.', len_price);
            int len_real = part_decemal - price;
            char *part_fractinal = (char *)memchr(part_decemal, '\0', len_price);
            int len_fractinal = part_fractinal - part_decemal - 1;
            part_decemal++;
            unsigned long long real = (multi[0][len_real] * price[0] + multi[1][len_real] * price[1] + multi[2][len_real] * price[2] - offset[len_real]);
            unsigned long long fractinal = (multi[0][len_fractinal] * part_decemal[0] + multi[1][len_fractinal] * part_decemal[1] + multi[2][len_fractinal] * part_decemal[2] - offset[len_fractinal]);
            if(len_fractinal == 1)
                fractinal *= 10;
            value = real * 100 + fractinal;
            save_data(data, city, product, len_city, len_product, value);
        }
        else
        {
            line[i] = file_content[start];
            i++;
        }
    }
}


void *merge(void *arg)
{
    struct mqadem * my_data;
    my_data = (struct mqadem *) arg;
    struct thread_data * data1 = my_data->data1;
    struct thread_data * data2 = my_data->data2;
    for(int i = 0; i < 101; i++)
    {
        if(data1->data[i]->total == 0)
        {
            data1->data[i] = data2->data[i];
            continue;
        }
        if(data2->data[i]->total == 0)
            continue;
        data1->data[i]->total += data2->data[i]->total;
        for(int j = 0; j < 94; j++)
        {
            if(data2->data[i]->id_product[j] == 0)
                continue;
            if(data1->data[i]->id_product[j] == 0)
            {
                data1->data[i]->id_product[j] = data2->data[i]->id_product[j];
                data1->data[i]->price[j] = data2->data[i]->price[j];
            }
            else
            {
                if(data1->data[i]->price[j] > data2->data[i]->price[j])
                    data1->data[i]->price[j] = data2->data[i]->price[j];
            }
        }
    }
    for(int i = 0; i < 102; i++)
        munmap((void *)data2->data[i], sizeof(struct data_chunk));
    munmap((void *)data2, sizeof(struct thread_data));
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
                if(strcmp(product_names[index], product_names[i]) > 0)
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
        fprintf(out, "%s %lu.0%lu", product_names[index], min / 100, fractinal);
    else
        fprintf(out, "%s %lu.%lu", product_names[index], min / 100, fractinal);
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
    fprintf(out, "%s ", city_names[index]);
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
    off_t file_size = lseek(fd, 0, SEEK_END);
    const char * file_content = (const char *)(mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0));
    unsigned long num_procs = std::thread::hardware_concurrency() * 2;
    struct thread_data *procs_data = (struct thread_data *)mmap(NULL, sizeof(struct thread_data) * num_procs, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    pid_t pids[num_procs];
    for(int i = 0; i < num_procs; i++)
    {
        procs_data[i].procs_id = i;
        procs_data[i].file_content = file_content;
        procs_data[i].file_size = file_size;
        procs_data[i].num_procs = num_procs;
        procs_data[i].data = (struct data_chunk **)mmap(NULL, sizeof(struct data_chunk *) * 102, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        for(int j = 0; j < 102; j++)
        {
            procs_data[i].data[j] = (struct data_chunk *)mmap(NULL, sizeof(struct data_chunk), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
            memset(procs_data[i].data[j], 0, sizeof(struct data_chunk));
        }
        pids[i] = fork();
        if(pids[i] == 0)
        {
            read_file(&procs_data[i]);
            exit(0);
        }
    }
    for(int i = 0; i < num_procs; i++)
    {
        waitpid(pids[i], NULL, 0);
    }
    munmap((void *)file_content, file_size);
    std::vector<int> vec;
    int start = 2;
    int thre = num_procs;
    while(start <= num_procs)
    {
        struct mqadem data[num_procs/start];   
        int first = 0;
        int second = first + 1 * (start / 2);
        int i = 0;
        while(second < num_procs)
        {
            if (second + start / 2 > num_procs)
                break;
            data[i].data1 = &procs_data[first];
            data[i].data2 = &procs_data[second];
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
        {
           vec.push_back(first);
        }
        thre /= 2;
        start*=2;
    }
    for(int i = 0; i < vec.size(); i++)
    {
        struct mqadem data;
        data.data1 = &procs_data[0];
        data.data2 = &procs_data[vec[i]];
        pthread_create(&threads[i], NULL, merge, (void *)&data);
        pthread_join(threads[i], NULL);
    }
    the_cheapest_city(procs_data[0].data);
    close(fd);
    return 0;
}
