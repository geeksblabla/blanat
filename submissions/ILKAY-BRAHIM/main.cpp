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

void save_data(struct thread_data * data, char *city, char *product, int len_city, int len_product, unsigned long price)
{
    struct stringIndexPair * _hash_city = find_index_city(city, len_city);
    struct stringIndexPair * _hash_product = find_index_product(product, len_product);
    int index_city = _hash_city->index;
    int index_product = _hash_product->index;
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
    char city[18];
    char product[17];
    for(;start < end; start++)
    {
        int len_city = 0;
        int len_product = 0;
        value = 0;
        int i = 0;
        for(;file_content[start] != ',';)
        {
            city[i] = file_content[start];
            len_city++;
            start++;
            i++;
        }
        city[i] = '\0';
        start++;
        i = 0;
        for(;file_content[start] != ',';)
        {
            product[i] = file_content[start];
            len_product++;
            start++;
            i++;
        }
        product[i] = '\0';
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
        save_data(my_data, city, product, len_city, len_product, value);
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
    while(start <= num_threads)
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
        {
           vec.push_back(first);
        }
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