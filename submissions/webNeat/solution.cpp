#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include <vector>
#include <thread>
#include <algorithm>

#define BUFFER_LENGTH 1048576
#define CITIES_COUNT 101
#define PRODUCTS_COUNT 100
#define LINE_MAX_LENGTH 100
#define ALPHABET_SIZE 26
#define THREADS_COUNT 8

struct TrieNode {
  struct TrieNode *children[ALPHABET_SIZE];
  int index;
};

int ascii_diff = 'a' - 'A';
char cities[][20] = {
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
char products[][20] = {
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
    "Currant", "Date", "Clementine", "Cranberry", "Rhubarb",
    "Chard", "Collard_Greens", "Parsley", "Cilantro", "Mint",
    "Basil", "Thyme", "Rosemary", "Sage", "Dill",
    "Oregano", "Cantaloupe", "Honeydew", "Coconut", "Pomegranate",
    "Jackfruit", "Starfruit", "Persimmon", "Ginger", "Turnip",
    "Jicama", "Kohlrabi", "Watercress", "Okra", "Artichoke",
    "Plantain", "Cactus_Pear", "Kiwano", "Squash_Blossom", "Dragon_Fruit",
    "Parsnip", "Rutabaga", "Salsify", "Bok_Choy", "Endive"
};

TrieNode *city_to_index;
char** index_to_city;
int index_to_city_length;
long double city_results[THREADS_COUNT][CITIES_COUNT];

TrieNode *product_to_index;
char** index_to_product;
int index_to_product_length;
double*** product_results;

static inline TrieNode* create_trie_node() {
  TrieNode *node = (TrieNode *) malloc(sizeof(TrieNode));
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    node->children[i] = NULL;
  }
  node->index = -1;
  return node;
}

static inline int char_index(char c) {
  if (c >= 'A' && c <= 'Z') c += ascii_diff;
  if (c < 'a' || c > 'z') return -1;
  return c - 'a';
}

inline static int get_index(TrieNode *root, char** index_to_item, int* index_to_item_length, char *line, size_t *pos) {
  TrieNode *pointer = root;
  int start = *pos;
  while (line[*pos] != ',' && line[*pos] != '\0') {
    int c_index = char_index(line[*pos]);
    if (c_index != -1) {
      if (!pointer->children[c_index]) {
        pointer->children[c_index] = create_trie_node();
      }
      pointer = pointer->children[c_index];
    }
    (*pos)++;
  }
  if (pointer->index == -1) {
    pointer->index = *index_to_item_length;
    strncpy(index_to_item[*index_to_item_length], line + start, *pos - start);
    index_to_item[*index_to_item_length][*pos - start] = '\0';
    (*index_to_item_length) ++;
  }
  (*pos)++;
  return pointer->index;
}

inline static double parse_float(char* s) {
  int x = s[0] - '0';
  int divisor = 1;
  ++ s;
  while (*s != '.') {
    x = 10 * x + (*s - '0');
    ++ s;
  }
  ++ s;
  while (*s != '\n') {
    x = 10 * x + (*s - '0');
    divisor *= 10;
    ++ s;
  }
  return x / (double) divisor;
}

void handle (size_t start, size_t end, long double* totals, double** prices) {
  FILE *file = fopen("input.txt", "r");
  char buffer[BUFFER_LENGTH + 1];
  int city_index, product_index;
  double price, min_price;
  size_t bytesRead, buffer_pos;
  fseek(file, start, SEEK_SET);
  while (start < end && (bytesRead = fread(buffer, sizeof(char), std::min((size_t) BUFFER_LENGTH, end - start), file)) > 0) {
    buffer[bytesRead] = '\0';
    buffer_pos = 0;
    while (buffer_pos < bytesRead) {
      city_index = get_index(city_to_index, index_to_city, &index_to_city_length, buffer, &buffer_pos);
      product_index = get_index(product_to_index, index_to_product, &index_to_product_length, buffer, &buffer_pos);
      price = parse_float(buffer + buffer_pos);
      totals[city_index] += price;
      min_price = prices[city_index][product_index];
      if (min_price == 0 || min_price > price) prices[city_index][product_index] = price;
      while (buffer[buffer_pos] != '\n') buffer_pos++;
      buffer_pos++;
      if (bytesRead == BUFFER_LENGTH && bytesRead < buffer_pos + LINE_MAX_LENGTH) {
        fseek(file, buffer_pos - bytesRead, SEEK_CUR);
        break;
      }
    }
    start += buffer_pos;
  }
  fclose(file);
}

inline bool compare_product_pairs(const std::pair<int, double>& a, const std::pair<int, double>& b) {
  if (a.second != b.second) return a.second < b.second;
  return strcmp(index_to_product[a.first], index_to_product[b.first]) < 0;
}

int main() {
  city_to_index = create_trie_node();
  index_to_city = (char **) malloc(CITIES_COUNT * sizeof(char*));
  for (int i = 0; i < CITIES_COUNT; ++i) {
    index_to_city[i] = (char *) malloc(20 * sizeof(char*));
  }
  size_t pos;
  for (int i = 0; i < CITIES_COUNT; ++i) {
    pos = 0;
    get_index(city_to_index, index_to_city, &index_to_city_length, cities[i], &pos);
  }

  product_to_index = create_trie_node();
  index_to_product = (char **) malloc(PRODUCTS_COUNT * sizeof(char*));
  for (int i = 0; i < PRODUCTS_COUNT; ++i) {
    index_to_product[i] = (char *) malloc(20 * sizeof(char*));
  }
  for (int i = 0; i < PRODUCTS_COUNT; ++i) {
    pos = 0;
    get_index(product_to_index, index_to_product, &index_to_product_length, products[i], &pos);
  }

  product_results = (double ***) malloc(THREADS_COUNT * sizeof(double **));
  for (int i = 0; i < THREADS_COUNT; i++) {
    product_results[i] = (double **) malloc(CITIES_COUNT * sizeof(double *));
    for (int city_i = 0; city_i < CITIES_COUNT; ++ city_i) {
      product_results[i][city_i] = (double *) malloc(PRODUCTS_COUNT * sizeof(double));
      for (int product_i = 0; product_i < PRODUCTS_COUNT; ++ product_i) {
        product_results[i][city_i][product_i] = 0;
      }
    }
  }

  FILE* file = fopen("input.txt", "r");
  struct stat sb;
  fstat(fileno(file), &sb);
  size_t file_size = sb.st_size;

  size_t part_size = file_size / THREADS_COUNT;
  char line[LINE_MAX_LENGTH];
  std::vector<std::thread> threads; 
  pos = 0;
  for (int i = 0; i < THREADS_COUNT; ++i) {
    size_t offset = pos;
    pos += part_size;
    fseek(file, pos, SEEK_SET);
    if (fgets(line, LINE_MAX_LENGTH, file)) pos += strlen(line);
    if (pos > file_size) pos = file_size;
    threads.emplace_back(handle, offset, pos, city_results[i], product_results[i]);
  }
  fclose(file);
  for (auto& thread : threads) thread.join();

  long double totals[CITIES_COUNT];
  for (int i = 0; i < CITIES_COUNT; ++i) totals[i] = 0;

  for (int part_id = 0; part_id < THREADS_COUNT; ++part_id) {
    for (int i = 0; i < CITIES_COUNT; ++i) totals[i] += city_results[part_id][i];
  }

  int cheapest_city_index = -1;
  double cheapest_total = pow(10, 11);
  for (int i = 0; i < CITIES_COUNT; i++) {
    if ((totals[i] > 0 && totals[i] < cheapest_total) || (totals[i] == cheapest_total && strcmp(index_to_city[i], index_to_city[cheapest_city_index]) < 0)) {
      cheapest_total = totals[i];
      cheapest_city_index = i;
    }
  }

  for (int i = 1; i < THREADS_COUNT; ++i) {
    for (int j = 0; j < PRODUCTS_COUNT; ++j) {
      if (product_results[i][cheapest_city_index][j] == 0) continue;
      product_results[0][cheapest_city_index][j] = std::min(product_results[0][cheapest_city_index][j], product_results[i][cheapest_city_index][j]);
    }
  }

  double* product_prices = product_results[0][cheapest_city_index];

  std::vector<std::pair<int, double>> pairs;
  for (int i = 0; i < PRODUCTS_COUNT; ++i) pairs.emplace_back(i, product_prices[i]);
  std::sort(pairs.begin(), pairs.end(), compare_product_pairs);
  
  FILE* out = fopen("output.txt", "w");
  fprintf(out, "%s %.2lf\n", index_to_city[cheapest_city_index], cheapest_total);
  int count = 0;
  for (int i = 0; count < 5; ++i) {
    if (pairs[i].second == 0) continue;
    fprintf(out, "%s %.2f\n", index_to_product[pairs[i].first], pairs[i].second);
    count ++;
  }

  return 0;
}