
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <future>
#include <map>
#include <vector>
#include <limits.h>
#include <algorithm>
#include <set>
#include <limits.h>
#include <algorithm>
#include <signal.h>
 #include <sys/types.h>
#include <set>
#include <sys/wait.h>
using namespace std;
#define SPARSE 196726
#define min(a, b) (a ^ ((b ^ a) & -(b < a)));
const int THREADS = 16;

unsigned long * revh;
struct Result {
  long sum[5000];
  int *minprodprice[5000];
    
};
struct Chunk {
  size_t begin;
  size_t end;
   char *data;
};

Result process_chunk(void *ptr) {

  struct Chunk *ch = (struct Chunk *)ptr;
  if (ch->begin > 0) {
    while (ch->data[ch->begin - 1] != '\n') {
      ch->begin++;
    }
  }
  while (ch->data[ch->end - 1] != '\n') {
    ch->end++;
  }

  struct Result *result = (Result *)malloc(sizeof(*result));
  memset(result->sum, 0, 5000 * sizeof(long));
    for(int i = 0; i < 5000; i++) {
        result->minprodprice[i] = (int *) malloc(5000 * sizeof(int));
    }
    for (int i=0; i<5000; i++) {
        for (int j=0; j<5000; j++) {
            result->minprodprice[i][j] = 65533;
        }
    }
   char *chars = &ch->data[ch->begin];
   char *end = &ch->data[ch->end];
    uint64_t  word;
   int lines=0;
   while (chars != end) {
       lines++;
       word = *reinterpret_cast<uint64_t* >(chars);
       char *ret = (char *)rawmemchr(chars, ',');
       int sp = ret-chars;
       if(sp<8) {
            word = word & ((1ULL<<(sp*8))-1);
       }
       
      int hash = (int)(word^(word >> 30));
      hash  =(hash ^ (hash>>11));
      hash  = hash & ((1ULL<<12)-1);
      chars+=sp+1;
      memcpy(&word, chars, 8);
        ret = (char *)rawmemchr(chars, ',');
        sp = ret-chars;
       if(sp<8) {
           word = word & ((1L<<(sp*8))-1) ;
       }
      int hash2 = (int)(word^(word >> 32));
      hash2  =(hash2 ^ (hash2>>15));
      hash2  = hash2 & ((1<<12)-1);
      chars+=sp+1;
      //cout<<"------------->"<<*(chars)<<endl;
      //cout<<"hash  ---> " << hash<<endl;
       
       memcpy(&word, chars, 8);
       ret = (char *)rawmemchr(chars, '\n');
       sp = ret-chars;
       word = word & ((1L<<(sp*8))-1) ;
       
       word =word^(word >> 7);
       word=(int)(word^(word >> 13))& ((1<<19)-1);
       chars+=sp+1;
       result->sum[hash] += revh[word];

      result->minprodprice[hash][hash2]= min(revh[word],result->minprodprice[hash][hash2] );
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
    


    int allhashes_cities[] = {3600,3279,49,2858,15,753,1421,2436,495,2365,3486,671,3925,2354,3609,3116,347,2792,600,4071,3130,3294,1548,3944,3329,1347,2656,3440,3083,760,1919,1495,2208,69,3711,904,2473,3697,1926,2060,3470,1499,1391,682,3722,37,1078,3586,3129,3832,903,747,1099,2109,1757,3751,1678,938,164,1227,1282,4009,440,73,1323,3096,2160,3967,184,1150,870,3920,2197,3031,1349,60,2926,1228,1136,829,3374,251,1737,2593,3000,2127,2718,2538,309,1033,3033,250,3790,4062,2818,2730,2408,506,2054,3015,2471};


    
    
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

    int allhashes_vigies[] = {2244,752,3050,10,480,3591,2829,4094,2981,1018,1722,1531,402,1838,50,3105,3872,3879,1027,2863,3064,918,3588,1056,713,2532,2440,1539,3374,827,2338,456,2343,3888,2963,3611,762,3343,2088,3643,556,280,1795,3049,3464,2589,3604,3431,914,1769,243,2762,716,3149,387,1299,2647,3133,3847,2822,3886,2988,2564,3613,3301,2866,554,273,401,968,707,1597,2973,412,570,1792,29,7,1534,2521,2785,1069,553,2475,3380,2638,1227,3956,3671,3642,1046,272,2825,2555};


    //char *file = "/Users/idwysa/dockk/blanat/input_small.txt";
    //char *file = "input_billion.txt";
    FILE *file_out = fopen("outputswa.txt", "w");
   // FILE *file_out = fopen("/Users/idwysa/dockk/blanat/fasto.txt", "w");
    //char *file = "/Users/idwysa/dockk/output2.csv";
    int file_desc = open("input_billion.txt", O_RDONLY);
    //int file_desc = open("/Users/idwysa/dockk/trc.txt", O_RDONLY);
    //int file_desc = open("/Users/idwysa/dockk/output33.csv", O_RDONLY);

    
    struct stat sb;
    fstat(file_desc, &sb);
    size_t sz = (size_t)sb.st_size;
    char *data = (char *)mmap(NULL, sz, PROT_READ, MAP_SHARED, file_desc, 0);
    vector<future<Result>> future_results;
     vector<Result> results1;
    std::vector<std::thread> workers1;
      struct Chunk chunks[THREADS];
      size_t chunk_size = sz / (size_t)THREADS;
      for (int i = 0; i < THREADS; i++) {
        chunks[i].data = data;
        chunks[i].begin = chunk_size * (size_t)i;
        chunks[i].end = chunk_size * ((size_t)i + 1);
          std::vector<std::thread> workers1;
          future_results.emplace_back(async(process_chunk, &chunks[i]));
      }


    for (auto &fr : future_results) {
        results1.emplace_back(fr.get());
    }
    
    for (int j = 1; j < THREADS; j++) {
        for (int i =0; i<101; i++) {
            results1[0].sum[allhashes_cities[i]] += results1[j].sum[allhashes_cities[i]];
        }
    }

    string min_city = "";
    long min_sum =0;
    int small_idx_citi = -1;
    for (int i =0; i<101; i++) {
        if (results1[0].sum[allhashes_cities[i]] !=0) {
            if(min_sum==0 or min_sum>results1[0].sum[allhashes_cities[i]] ) {
                min_sum=results1[0].sum[allhashes_cities[i]];
                min_city =moroccan_cities[i];
                small_idx_citi = i;
            }
            else if (min_sum==results1[0].sum[allhashes_cities[i]] and min_city>moroccan_cities[i]) {
                min_sum=results1[0].sum[allhashes_cities[i]];
                min_city =moroccan_cities[i];
                small_idx_citi = i;
            }
        }
    }
    fprintf(file_out, "%s %.2f\n", min_city.c_str(), min_sum / 100.0);
   // cout<<min_city<<" ......... "<<min_sum;
    for (int j = 1; j < THREADS; j++) {
        for (int i =0; i<94; i++) {
         results1[0].minprodprice[allhashes_cities[small_idx_citi]][allhashes_vigies[i]] = min(results1[j].minprodprice[allhashes_cities[small_idx_citi]][allhashes_vigies[i]], results1[0].minprodprice[allhashes_cities[small_idx_citi]][allhashes_vigies[i]]);
            //cout<<results1[0].minprodprice[allhashes_cities[small_idx_citi]][allhashes_vigies[i]]<<"???????"<<endl;
        }
    }
    vector<pair<long, string>> prods;

        for (int i =0; i<94; i++) {
            prods.push_back({results1[0].minprodprice[allhashes_cities[small_idx_citi]][allhashes_vigies[i]] , vigies[i]});
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
        unsigned long aa;
        unsigned long fullones = 0xFFFFFFFFFFFFFFFFUL;
        const char* str ;
        revh = (unsigned long *)malloc(560000 * sizeof(unsigned long));
    vector<string> dig = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
        vector<string> double_dig;
        for (int i = 0; i<dig.size(); i++) {
            for (int j=0; j<dig.size(); j++) {
                double_dig.push_back(dig[i]+dig[j]);
            }
        }
    set<string> total;
    string temp;
        for (int i=0; i<dig.size(); i++) {
            for (int j=0; j<double_dig.size(); j++) {
                temp=dig[i]+"."+double_dig[j];
                
                str = temp.c_str();
                memcpy(&aa,str,8);
                aa = aa&(fullones>>(64-(8*temp.size())));
                aa =aa^(aa >> 7);
                aa=(int)(aa^(aa >> 13))& ((1<<19)-1);
                revh[aa]=(dig[i][0]-'0')*100+(double_dig[j][0]-'0')*10+(double_dig[j][1]-'0');
                temp=double_dig[j]+"."+dig[i];
                
                str = temp.c_str();
                memcpy(&aa,str,8);
                aa = aa&(fullones>>(64-(8*temp.size())));
                aa =aa^(aa >> 7);
                aa=(int)(aa^(aa >> 13))& ((1<<19)-1);
                revh[aa]=(double_dig[j][0]-'0')*1000+(double_dig[j][1]-'0')*100+(dig[i][0]-'0')*10;
            }
        }
        for (int i=0; i<double_dig.size(); i++) {
            for (int j=0; j<double_dig.size(); j++) {
                temp=double_dig[i]+"."+double_dig[j];
                
                str = temp.c_str();
                memcpy(&aa,str,8);
                aa = aa&(fullones>>(64-(8*temp.size())));
                aa =aa^(aa >> 7);
                aa=(int)(aa^(aa >> 13))& ((1<<19)-1);
                revh[aa]=(double_dig[i][0]-'0')*1000+(double_dig[i][1]-'0')*100+(double_dig[j][0]-'0')*10 + (double_dig[j][1]-'0');
            }
        }
    for (int i=0; i<double_dig.size(); i++) {
            temp=double_dig[i];
            str = temp.c_str();
            memcpy(&aa,str,8);
            aa = aa&(fullones>>(64-(8*temp.size())));
            aa =aa^(aa >> 7);
            aa=(int)(aa^(aa >> 13))& ((1<<19)-1);
            revh[aa]=(double_dig[i][0]-'0')*1000+(double_dig[i][1]-'0')*100;
        
    }
        for (int i=0; i<dig.size(); i++) {
            for (int j=0; j<dig.size(); j++) {
                temp=dig[i]+"."+dig[j];
                str = temp.c_str();
                memcpy(&aa,str,8);
                aa = aa&(fullones>>(64-(8*temp.size())));
                aa =aa^(aa >> 7);
                aa=(int)(aa^(aa >> 13))& ((1<<19)-1);
                revh[aa]=(dig[i][0]-'0')*100+(dig[j][0]-'0')*10;
            }
        }
    temp="100";
    str = temp.c_str();
    memcpy(&aa,str,8);
    aa = aa&(fullones>>(64-(8*temp.size())));
    aa =aa^(aa >> 7);
    aa=(int)(aa^(aa >> 13))&((1<<19)-1);
    revh[aa]=10000;
    temp="100.0";
    str = temp.c_str();
    memcpy(&aa,str,8);
    aa = aa&(fullones>>(64-(8*temp.size())));
    aa =aa^(aa >> 7);
    aa=(int)(aa^(aa >> 13))&((1<<19)-1);
    revh[aa]=10000;
    process(argc, argv);
    return 0;
}
