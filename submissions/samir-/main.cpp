#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <future>
#include <map>
#include <vector>
#include <limits.h>
#include <algorithm>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
using namespace std;

// V0 (multithread , mmap, special hash, skip some)
#define SPARSE 196726
#define min(a, b) (a ^ ((b ^ a) & -(b < a)));
const int THREADS = thread::hardware_concurrency();
int glob_citi_hash_to_idx[SPARSE];
int glob_vigi_hash_to_idx[SPARSE];
struct Chunk {
  size_t begin;
  size_t end;
  const char *data;
};
struct OutputChunk {
  long sum[160];
  int *minprodprice[160];
};
// to optimize
static inline const char *parse_number(const char *s, int * sink) {
    *sink = *s - ('0');
    s++;
    int left=100;
    while (*s != '\n' and *s != '.') {
        *sink = (*sink * 10) + (*s) - ('0');
        s++;
    }
    s += (*s=='.') ;
    while (*s != '\n') {
        *sink = (*sink * 10) + (*s) - ('0');
        s++;
        left/=10;
    }
     *sink = (*sink)*left;
  return s+1;
}

OutputChunk process_chunk(void *ptr) {
  struct Chunk *ch = (struct Chunk *)ptr;
    int temp, temp2;
    //calibrate
  if (ch->begin > 0) {
    while (ch->data[ch->begin - 1] != '\n') {
      ch->begin++;
    }
  }
  while (ch->data[ch->end - 1] != '\n') {
    ch->end++;
  }
  struct OutputChunk *result = (OutputChunk *)malloc(sizeof(*result));
  memset(result->sum, 0, 160 * sizeof(long));
    for(int i = 0; i < 160; i++) {
        result->minprodprice[i] = (int *) malloc(160 * sizeof(int));
    }
    for (int i=0; i<150; i++) {
        for (int j=0; j<150; j++) {
            result->minprodprice[i][j] = 65533;
        }
    }
  const char *s = &ch->data[ch->begin];
  const char *end = &ch->data[ch->end];
  int h, h1, price, len;
  while (s != end) {
    len = 3;
    h = *s;
    while (s[len] != ',') {
      h = h + s[len]*s[len];
      len++;
    }
      len++;
      h1 = s[len];
      len+=3;
      while (s[len] != ',') {
          h1 = h1 + s[len]*s[len];
          len++;
      }
    s = parse_number(s + len + 1, &price);
      temp =glob_citi_hash_to_idx[h];
      temp2 =glob_vigi_hash_to_idx[h1];
      result->sum[temp] += price;
      result->minprodprice[temp][temp2]= min(price,result->minprodprice[temp][temp2] );
  }
    return *result;
}



int process(int argc, char* argv[])
{

    vector<string> moroccan_cities = {
        "Casablanca", "Rabat", "Marrakech", "Fes", "Tangier",
        "Agadir", "Meknes", "Oujda", "Kenitra", "Tetouan",
        "Safi", "El_Jadida", "Beni_Mellal", "Errachidia",
        "Taza", "Essaouira", "Khouribga", "Guelmim",
        "Jorf_El_Melha", "Laayoune", "Ksar_El_Kebir", "Sale", "Bir_Lehlou",
        "Arfoud", "Temara", "Mohammedia", "Settat",
        "Béni_Mellal", "Nador", "Kalaat_MGouna",
        "Chichaoua", "Chefchaouen", "Al_Hoceima", "Taourirt",
        "Taroudant", "Guelta_Zemmur", "Dakhla", "Laâyoune",
        "Tiznit","Tinghir", "Ifrane", "Azrou", "Bab_Taza",
        "Berrechid", "Sidi_Slimane", "Souk_Larbaa", "Tiflet", "Sidi_Bennour",
        "Larache", "Tan-Tan", "Sidi_Ifni", "Goulmima",
        "Midelt", "Figuig", "Azilal", "Jerada", "Youssoufia",
        "Ksar_es_Seghir", "Tichka", "Ait_Melloul",
        "Layoune", "Ben_guerir", "Ouarzazate", "Inezgane",
        "Oujda_Angad", "Sefrou", "Aourir",
        "Oulad_Teima", "Tichla", "Bni_Hadifa",
        "Fquih_Ben_Salah", "Guercif", "Bouarfa", "Demnate",
        "Ahfir", "Berkane", "Akhfenir", "Boulemane",
        "Khenifra", "Bir_Anzerane", "Assa", "Smara", "Boujdour",
        "Tarfaya", "Ouazzane", "Zagora", "had_soualem",
        "Saidia", "Bab_Berred", "Midar", "Moulay_Bousselham",
        "Khemisset", "Guerguerat", "Asilah", "Sidi_Bouzid", "Tafraout",
        "Imzouren", "Zemamra", "Sidi_Kacem", "Drarga", "Skhirate"
    };



    int allhashes_cities[] = {71463,22947,64749,70,44915,34086,35603,19488,46961,47603,11108,55388,80647,71554,9493,68918,67407,46522,92972,63028,96997,10284,73558,36075,31898,73883,36404,92747,25395,99863,65512,88808,69887,64246,71059,112373,31957,71864,36665,45530,31783,26075,49849,64905,93302,77160,35405,97896,40303,30674,58991,55931,35398,35393,32802,28892,83387,117083,31758,86222,48387,80607,85318,57276,74856,39089,37082,78085,31973,64522,114805,44297,42284,45234,24086,43225,56791,65522,56009,95207,9474,22488,60308,43947,61557,34816,91519,30517,69841,22482,155603,73088,83427,31954,86408,61955,61380,43785,67050,33082,57170};
    for (int i =0; i<moroccan_cities.size(); i++) {
        glob_citi_hash_to_idx[allhashes_cities[i]]  = i;
    }
    vector<string> vigies = {
        "Apple", "Banana", "Orange", "Strawberry", "Grapes",
            "Watermelon", "Pineapple", "Mango", "Kiwi", "Peach",
            "Plum", "Cherry", "Pear", "Blueberry", "Raspberry",
            "Blackberry", "Cantaloupe", "Honeydew", "Coconut", "Pomegranate",
            "Lemon", "Lime", "Grapefruit", "Avocado", "Papaya",
            "Guava", "Fig", "Passion_Fruit", "Apricot", "Nectarine",
            "Cucumber", "Carrot", "Broccoli", "Spinach", "Kale",
            "Lettuce", "Tomato", "Bell_Pepper", "Zucchini", "Eggplant",
            "Cabbage", "Cauliflower", "Brussels_Sprouts", "Radish", "Beet",
            "Asparagus", "Artichoke", "Green_Beans", "Peas", "Celery",
            "Onion", "Garlic", "Potato", "Sweet_Potato", "Yam",
            "Butternut_Squash", "Acorn_Squash", "Pumpkin", "Cranberry", "Goji_Berry",
            "Currant", "Date", "Clementine", "Rhubarb",
            "Chard", "Collard_Greens", "Parsley", "Cilantro", "Mint",
            "Basil", "Thyme", "Rosemary", "Sage", "Dill",
            "Oregano",
            "Jackfruit", "Starfruit", "Persimmon", "Ginger", "Turnip",
            "Jicama", "Kohlrabi", "Watercress", "Okra",
            "Plantain", "Cactus_Pear", "Kiwano", "Squash_Blossom", "Dragon_Fruit",
            "Parsnip", "Rutabaga", "Salsify", "Bok_Choy", "Endive"
    };

    int allhashes_vigies[] = {21930,30984,32989,84091,36041,81451,66643,23007,11100,20697,11961,40700,13076,70705,73064,81754,83351,59276,51633,88461,24497,10277,84386,41596,33539,23404,70,113842,46668,69265,58438,38840,54678,42209,10276,47223,35270,85641,54857,59242,39890,94503,155751,35148,13522,69402,65678,80688,13305,37905,24500,32561,35266,100128,89,151786,100983,47198,72605,75311,48028,10269,81031,41695,23063,116925,49811,60349,13533,22755,22166,59210,10284,11732,44518,73093,74649,72513,33877,35753,30773,54773,82732,9488,58170,88468,33905,121551,100189,48974,48522,49378,51358,35219};
    for (int i =0; i<vigies.size(); i++) {
        glob_vigi_hash_to_idx[allhashes_vigies[i]]  = i;
    }
    FILE *file_out = fopen("output.txt", "w");
    int file_desc = open("input.txt", O_RDONLY);
    struct stat sb;
    fstat(file_desc, &sb);
    size_t sz = (size_t)sb.st_size;
    const char *data = (char *)mmap(NULL, sz, PROT_READ, MAP_SHARED, file_desc, 0);
    vector<future<OutputChunk>> future_results;
     vector<OutputChunk> results1;
    std::vector<std::thread> workers1;
    struct OutputChunk *results[THREADS];
      struct Chunk chunks[THREADS];
      size_t chunk_size = sz / (size_t)THREADS;
      for (int i = 0; i < THREADS; i++) {
        chunks[i].data = data;
        chunks[i].begin = chunk_size * (size_t)i;
        chunks[i].end = chunk_size * ((size_t)i + 1);
          std::vector<std::thread> workers1;
          future_results.emplace_back(async(process_chunk, &chunks[i]));
      }

    struct OutputChunk *result11;

    for (auto &fr : future_results) {
        results1.emplace_back(fr.get());
    }
    for (int j = 1; j < THREADS; j++) {
        for (int i =0; i<110; i++) {
            results1[0].sum[i] += results1[j].sum[i];
        }
    }
    string min_city = "";
    long min_sum =0;
    int small_idx_citi = -1;
    for (int i =0; i<101; i++) {
        if (results1[0].sum[i] !=0) {
            if(min_sum==0 or min_sum>results1[0].sum[i] ) {
                min_sum=results1[0].sum[i];
                min_city =moroccan_cities[i];
                small_idx_citi = i;
            }
            else if (min_sum==results1[0].sum[i] and min_city>moroccan_cities[i]) {
                min_sum=results1[0].sum[i];
                min_city =moroccan_cities[i];
                small_idx_citi = i;
            }
        }
    }
    fprintf(file_out, "%s %.2f\n", min_city.c_str(), min_sum / 100.0);
    for (int j = 1; j < THREADS; j++) {
        for (int i =0; i<94; i++) {
            results1[0].minprodprice[small_idx_citi][i] = min(results1[j].minprodprice[small_idx_citi][i], results1[0].minprodprice[small_idx_citi][i]);
        }
    }
    vector<pair<long, string>> prods;
        for (int i =0; i<94; i++) {
            prods.push_back({results1[0].minprodprice[small_idx_citi][i] , vigies[i]});
        }
    sort(prods.begin(), prods.end());

    for (int i =0; i<5; i++) {
        if(prods[i].first != 65533) fprintf(file_out, "%s %.2f\n", prods[i].second.c_str(), prods[i].first / 100.0);
    }
    fflush(file_out);
    fclose(file_out);
  return 0;
}

int main(int argc, char* argv[]) {
    process(argc, argv);
    return 0;
}
