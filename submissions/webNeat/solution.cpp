#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <math.h>
#include <vector>
#include <thread>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <numeric>

#define BUFFER_LENGTH 1048576
#define CITIES_COUNT 101
#define PRODUCTS_COUNT 100
#define LINE_MAX_LENGTH 100
#define ALPHABET_SIZE 250
#define THREADS_COUNT 16

struct Node {
  Node* children[ALPHABET_SIZE];
  int index;
};

static Node cities_tree;
static char cities[][20] = {"Casablanca", "Rabat", "Marrakech", "Fes", "Tangier", "Agadir", "Meknes", "Oujda", "Kenitra", "Tetouan", "Safi", "El_Jadida", "Beni_Mellal", "Errachidia", "Taza", "Essaouira", "Khouribga", "Guelmim", "Jorf_El_Melha", "Laayoune", "Ksar_El_Kebir", "Sale", "Bir_Lehlou", "Arfoud", "Temara", "Mohammedia", "Settat", "Béni_Mellal", "Nador", "Kalaat_MGouna", "Chichaoua", "Chefchaouen", "Al_Hoceima", "Taourirt", "Taroudant", "Guelta_Zemmur", "Dakhla", "Laâyoune", "Tiznit","Tinghir", "Ifrane", "Azrou", "Bab_Taza", "Berrechid", "Sidi_Slimane", "Souk_Larbaa", "Tiflet", "Sidi_Bennour", "Larache", "Tan-Tan", "Sidi_Ifni", "Goulmima", "Midelt", "Figuig", "Azilal", "Jerada", "Youssoufia", "Ksar_es_Seghir", "Tichka", "Ait_Melloul", "Layoune", "Ben_guerir", "Ouarzazate", "Inezgane", "Oujda_Angad", "Sefrou", "Aourir", "Oulad_Teima", "Tichla", "Bni_Hadifa", "Fquih_Ben_Salah", "Guercif", "Bouarfa", "Demnate", "Ahfir", "Berkane", "Akhfenir", "Boulemane", "Khenifra", "Bir_Anzerane", "Assa", "Smara", "Boujdour", "Tarfaya", "Ouazzane", "Zagora", "had_soualem", "Saidia", "Bab_Berred", "Midar", "Moulay_Bousselham", "Khemisset", "Guerguerat", "Asilah", "Sidi_Bouzid", "Tafraout", "Imzouren", "Zemamra", "Sidi_Kacem", "Drarga", "Skhirate"};

static Node products_tree;
static char products[][20] = {"Apple", "Banana", "Orange", "Strawberry", "Grapes", "Watermelon", "Pineapple", "Mango", "Kiwi", "Peach", "Plum", "Cherry", "Pear", "Blueberry", "Raspberry", "Blackberry", "Cantaloupe", "Honeydew", "Coconut", "Pomegranate", "Lemon", "Lime", "Grapefruit", "Avocado", "Papaya", "Guava", "Fig", "Passion_Fruit", "Apricot", "Nectarine", "Cucumber", "Carrot", "Broccoli", "Spinach", "Kale", "Lettuce", "Tomato", "Bell_Pepper", "Zucchini", "Eggplant", "Cabbage", "Cauliflower", "Brussels_Sprouts", "Radish", "Beet", "Asparagus", "Artichoke", "Green_Beans", "Peas", "Celery", "Onion", "Garlic", "Potato", "Sweet_Potato", "Yam", "Butternut_Squash", "Acorn_Squash", "Pumpkin", "Cranberry", "Goji_Berry", "Currant", "Date", "Clementine", "Cranberry", "Rhubarb", "Chard", "Collard_Greens", "Parsley", "Cilantro", "Mint", "Basil", "Thyme", "Rosemary", "Sage", "Dill", "Oregano", "Cantaloupe", "Honeydew", "Coconut", "Pomegranate", "Jackfruit", "Starfruit", "Persimmon", "Ginger", "Turnip", "Jicama", "Kohlrabi", "Watercress", "Okra", "Artichoke", "Plantain", "Cactus_Pear", "Kiwano", "Squash_Blossom", "Dragon_Fruit", "Parsnip", "Rutabaga", "Salsify", "Bok_Choy", "Endive"};

static long double** cities_results;
static double*** products_results;

inline void results_init() {
  cities_results = (long double**) malloc(THREADS_COUNT * sizeof(long double));
  products_results = (double ***) malloc(THREADS_COUNT * sizeof(double **));
  for (int id = 0; id < THREADS_COUNT; ++id) {
    cities_results[id] = (long double *) malloc(CITIES_COUNT * sizeof(long double));
    products_results[id] = (double **) malloc(CITIES_COUNT * sizeof(double *));
    for (int i = 0; i < CITIES_COUNT; ++i) {
      cities_results[id][i] = 0;
      products_results[id][i] = (double *) malloc(PRODUCTS_COUNT * sizeof(double));
      for (int j = 0; j < PRODUCTS_COUNT; ++ j) products_results[id][i][j] = 0;
    }
  }
}

static void tree_insert(Node* tree, char* s, int value) {
  if (*s == '\0') {
    tree->index = value;
    return;
  }
  if (!tree->children[*s]) {
    tree->children[*s] = (Node*) malloc(sizeof(Node));
  }
  return tree_insert(tree->children[*s], s + 1, value);
}

static void tree_init() {
  for (int i = 0; i < CITIES_COUNT; ++i) tree_insert(&cities_tree, cities[i], i);
  for (int i = 0; i < PRODUCTS_COUNT; ++i) tree_insert(&products_tree, products[i], i);
}

static inline int tree_get(Node* tree, char* s, size_t& pos) {
  Node* p = tree;
  while (s[pos] != ',') {
    p = p->children[s[pos]];
    ++pos;
  }
  ++pos;
  return p->index;
}

inline static double parse_float(char* s, size_t& pos) {
  int x = s[pos] - '0';
  int divisor = 1;
  ++ pos;
  while (s[pos] != '.') {
    x = 10 * x + (s[pos] - '0');
    ++ pos;
  }
  ++ pos;
  while (s[pos] != '\n') {
    x = 10 * x + (s[pos] - '0');
    divisor *= 10;
    ++ pos;
  }
  ++pos;
  return x / (double) divisor;
}

static inline bool compare_product_pairs(const std::pair<int, double>& a, const std::pair<int, double>& b) {
  if (a.second != b.second) return a.second < b.second;
  return strcmp(products[a.first], products[b.first]) < 0;
}

void handle (char* start, char* end, long double* totals, double** prices) {
  int city_index, product_index;
  double price, min_price;
  size_t pos;
  while (start < end) {
    pos = 0;
    city_index = tree_get(&cities_tree, start, pos);
    product_index = tree_get(&products_tree, start, pos);
    price = parse_float(start, pos);
    totals[city_index] += price;
    min_price = prices[city_index][product_index];
    if (min_price == 0 || min_price > price) prices[city_index][product_index] = price;
    start += pos;
  }
}

int main() {
  tree_init();
  results_init();

  const char* filename = "input.txt";
  int fd = open(filename, O_RDONLY);
  struct stat sb;
  fstat(fd, &sb);
  size_t file_size = sb.st_size;
  size_t part_size = file_size / THREADS_COUNT;
  char* buffer = static_cast<char*>(mmap(nullptr, sb.st_size, PROT_READ, MAP_SHARED, fd, 0));
  std::vector<std::thread> threads;
  char* start = buffer;
  char* end;
  char* limit = buffer + sb.st_size;
  for (int id = 0; id < THREADS_COUNT; ++id) {
    end = start + part_size;
    if (end >= limit) end = limit;
    else end = (char *) memchr(end, '\n', 100) + 1;
    threads.emplace_back(handle, start, end, cities_results[id], products_results[id]);
  }
  for (auto& thread : threads) thread.join();
  close(fd);
  munmap(buffer, file_size);

  long double totals[CITIES_COUNT];
  for (int i = 0; i < CITIES_COUNT; ++i) totals[i] = 0;

  for (int id = 0; id < THREADS_COUNT; ++id) {
    for (int i = 0; i < CITIES_COUNT; ++i) totals[i] += cities_results[id][i];
  }

  int cheapest_city_index = 0;
  double cheapest_total = totals[0];
  for (int i = 1; i < CITIES_COUNT; ++i) {
    if (totals[i] < cheapest_total || (totals[i] == cheapest_total && strcmp(cities[i], cities[cheapest_city_index]) < 0)) {
      cheapest_total = totals[i];
      cheapest_city_index = i;
    }
  }

  for (int id = 1; id < THREADS_COUNT; ++id) {
    for (int j = 0; j < PRODUCTS_COUNT; ++j) {
      products_results[0][cheapest_city_index][j] = std::min(products_results[0][cheapest_city_index][j], products_results[id][cheapest_city_index][j]);
    }
  }

  double* products_prices = products_results[0][cheapest_city_index];

  std::vector<std::pair<int, double>> pairs;
  for (int j = 0; j < PRODUCTS_COUNT; ++j) pairs.emplace_back(j, products_prices[j]);
  std::sort(pairs.begin(), pairs.end(), compare_product_pairs);
  
  FILE* out = fopen("output.txt", "w");
  fprintf(out, "%s %.2lf\n", cities[cheapest_city_index], cheapest_total);
  for (int i = 0; i < 5; ++i) {
    fprintf(out, "%s %.2f\n", products[pairs[i].first], pairs[i].second);
  }
  fclose(out);

  return 0;
}