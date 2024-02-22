
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <emmintrin.h>

#include <sys/mman.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <time.h>

#include <thread>

#define ENABLE_ASSERTS 0

#if ENABLE_ASSERTS
# define AssertBreak (*(volatile int *)0 = 0)
# define Assert(b) do {if (!(b)) { printf("Assertion Assert(%s) failed", #b); AssertBreak; }} while (0)
#else
# define AssertBreak
# define Assert(b) (b)
#endif

#define MIN(a, b) (((a) < (b))? (a) : (b))
#define SWAP(a, b, T) do {T t = (a); (a) = (b); (b) = t;} while(0)

#define ArrayLength(arr) (sizeof(arr) / sizeof(*arr))

#define defer_loop(begin, end) for(int _i_ = ((begin), 0); _i_ < 1; _i_ += 1, (end))
#define compute_timing(res) defer_loop(start_timer(), res = end_timer())

static clock_t clock_record;
static void start_timer()
{
  clock_record = clock();
}
static double end_timer()
{
  double duration = (double)(clock() - clock_record ) / CLOCKS_PER_SEC;
  return duration;
}

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint8_t u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int8_t i8;

typedef int8_t b8;
typedef int32_t b32;

#define CHUNK_SIZE (1ull << 30)

#define CITIES_MAX 523
#define PRODUCTS_MAX 409

static u32 
convert_to_int(char *number, u32 number_len)
{
  u32 result = 0;
  for (u32 i = 0; i < number_len; i += 1)
  {
    result = result * 10 + (number[i] - '0');
  }
  return result;
}

static u32 cities_coefs[4] = {226, 371, 516, 292};
static u32 fruits_coefs[4] = {253, 46, 119, 133};

static u32
compute_product_index(char *product, u32 len)
{
  //printf("%c%c%c%c\n", product[0], product[1], product[2], product[len - 1]);
  Assert(len >= 3);
  
  u64 result = 0;
  result += product[0] * fruits_coefs[0];
  result += product[1] * fruits_coefs[1];
  result += product[2] * fruits_coefs[2];
  result += product[len - 1] * fruits_coefs[3];
  
  result %= PRODUCTS_MAX;
  return (u32)result;
}

static u32
compute_city_index(char *city, u32 len)
{
  Assert(len >= 3);

  u64 result = 0;
  result += city[1] * cities_coefs[0];
  result += city[2] * cities_coefs[1];
  result += city[len - 3] * cities_coefs[2];
  result += city[len - 2] * cities_coefs[3];

  result %= CITIES_MAX;
  
  return (u32)result;
}

typedef struct
{
  char *data;
  u32 len;
} String;

#define string_lit(x) { (x), ArrayLength(x) - 1}

// NOTE(fakhri): list of all possible products
// NOTE(fakhri): some fruits and vegies in the original generation code are duplicated... 
// NOTE(fakhri): duplicated names: Cantaloupe, Cranberry, Coconut, Honeydew, Pomegranate, Artichoke.
static String fuits_and_vegs[] = {
  string_lit("Acorn_Squash"),string_lit("Apple"),string_lit("Apricot"),string_lit("Artichoke"),
  string_lit("Asparagus"),string_lit("Avocado"),string_lit("Banana"),string_lit("Basil"),
  string_lit("Beet"),string_lit("Bell_Pepper"),string_lit("Blackberry"),string_lit("Blueberry"),
  string_lit("Bok_Choy"),string_lit("Broccoli"),string_lit("Brussels_Sprouts"),string_lit("Butternut_Squash"),
  string_lit("Cabbage"),string_lit("Cactus_Pear"),string_lit("Cantaloupe"),
  string_lit("Carrot"),string_lit("Cauliflower"),string_lit("Celery"),string_lit("Chard"),
  string_lit("Cherry"),string_lit("Cilantro"),string_lit("Clementine"),string_lit("Coconut"),
  string_lit("Collard_Greens"),string_lit("Cranberry"),string_lit("Cucumber"),string_lit("Currant"),
  string_lit("Date"),string_lit("Dill"),string_lit("Dragon_Fruit"),string_lit("Eggplant"),
  string_lit("Endive"),string_lit("Fig"),string_lit("Garlic"),string_lit("Ginger"),
  string_lit("Goji_Berry"),string_lit("Grapefruit"),string_lit("Grapes"),string_lit("Green_Beans"),
  string_lit("Guava"),string_lit("Honeydew"),string_lit("Jackfruit"),string_lit("Jicama"),
  string_lit("Kale"),string_lit("Kiwano"),string_lit("Kiwi"),string_lit("Kohlrabi"),
  string_lit("Lemon"),string_lit("Lettuce"),string_lit("Lime"),string_lit("Mango"),
  string_lit("Mint"),string_lit("Nectarine"),string_lit("Okra"),string_lit("Onion"),
  string_lit("Orange"),string_lit("Oregano"),string_lit("Papaya"),string_lit("Parsley"),
  string_lit("Parsnip"),string_lit("Passion_Fruit"),string_lit("Peach"),string_lit("Pear"),
  string_lit("Peas"),string_lit("Persimmon"),string_lit("Pineapple"),string_lit("Plantain"),
  string_lit("Plum"),string_lit("Pomegranate"),string_lit("Potato"),string_lit("Pumpkin"),
  string_lit("Radish"),string_lit("Raspberry"),string_lit("Rhubarb"),string_lit("Rosemary"),
  string_lit("Rutabaga"),string_lit("Sage"),string_lit("Salsify"),string_lit("Spinach"),
  string_lit("Squash_Blossom"),string_lit("Starfruit"),string_lit("Strawberry"),
  string_lit("Sweet_Potato"),string_lit("Thyme"),string_lit("Tomato"),string_lit("Turnip"),
  string_lit("Watercress"),string_lit("Watermelon"),string_lit("Yam"),string_lit("Zucchini")
};

// NOTE(fakhri): list of all possible cities
static String city_names[]= {
  string_lit("Agadir"),string_lit("Ahfir"),string_lit("Ait_Melloul"),string_lit("Akhfenir"),
  string_lit("Al_Hoceima"),string_lit("Aourir"),string_lit("Arfoud"),string_lit("Asilah"),
  string_lit("Assa"),string_lit("Azilal"),string_lit("Azrou"),string_lit("Béni_Mellal"),
  string_lit("Bab_Berred"),string_lit("Bab_Taza"),string_lit("Ben_guerir"),string_lit("Beni_Mellal"),
  string_lit("Berkane"),string_lit("Berrechid"),string_lit("Bir_Anzerane"),string_lit("Bir_Lehlou"),
  string_lit("Bni_Hadifa"),string_lit("Bouarfa"),string_lit("Boujdour"),string_lit("Boulemane"),
  string_lit("Casablanca"),string_lit("Chefchaouen"),string_lit("Chichaoua"),string_lit("Dakhla"),
  string_lit("Demnate"),string_lit("Drarga"),string_lit("El_Jadida"),string_lit("Errachidia"),
  string_lit("Essaouira"),string_lit("Fes"),string_lit("Figuig"),string_lit("Fquih_Ben_Salah"),
  string_lit("Goulmima"),string_lit("Guelmim"),string_lit("Guelta_Zemmur"),string_lit("Guercif"),
  string_lit("Guerguerat"),string_lit("had_soualem"),string_lit("Ifrane"),string_lit("Imzouren"),
  string_lit("Inezgane"),string_lit("Jerada"),string_lit("Jorf_El_Melha"),string_lit("Kalaat_MGouna"),
  string_lit("Kenitra"),string_lit("Khemisset"),string_lit("Khenifra"),string_lit("Khouribga"),
  string_lit("Ksar_El_Kebir"),string_lit("Ksar_es_Seghir"),string_lit("Laâyoune"),string_lit("Laayoune"),
  string_lit("Larache"),string_lit("Layoune"),string_lit("Marrakech"),string_lit("Meknes"),
  string_lit("Midar"),string_lit("Midelt"),string_lit("Mohammedia"),string_lit("Moulay_Bousselham"),
  string_lit("Nador"),string_lit("Ouarzazate"),string_lit("Ouazzane"),string_lit("Oujda_Angad"),
  string_lit("Oujda"),string_lit("Oulad_Teima"),string_lit("Rabat"),string_lit("Safi"),
  string_lit("Saidia"),string_lit("Sale"),string_lit("Sefrou"),string_lit("Settat"),
  string_lit("Sidi_Bennour"),string_lit("Sidi_Bouzid"),string_lit("Sidi_Ifni"),string_lit("Sidi_Kacem"),
  string_lit("Sidi_Slimane"),string_lit("Skhirate"),string_lit("Smara"),string_lit("Souk_Larbaa"),
  string_lit("Tafraout"),string_lit("Tan-Tan"),string_lit("Tangier"),string_lit("Taourirt"),
  string_lit("Tarfaya"),string_lit("Taroudant"),string_lit("Taza"),string_lit("Temara"),
  string_lit("Tetouan"),string_lit("Tichka"),string_lit("Tichla"),string_lit("Tiflet"),
  string_lit("Tinghir"),string_lit("Tiznit"),string_lit("Youssoufia"),string_lit("Zagora"),
  string_lit("Zemamra"),
};

static char *cities_name_per_index[CITIES_MAX];
static char *fruits_name_per_index[PRODUCTS_MAX];

#define GENERATE_CITIES_PH_COEFICIENTS 0
#define GENERATE_FRUITS_PH_COEFICIENTS 0

// NOTE(fakhri): THIS ASSUMES THE CITIES AND PRODUCTS WE WILL BE TESTED AGAINST
// ARE THE SAME AS THE ONE IN GEN.PY!!!!!!!

static void 
fill_cities_name_per_index_table()
{
#if GENERATE_CITIES_PH_COEFICIENTS
  time_t t;
  srand(time(&t));
  for(;;)
  {
    // NOTE(fakhri): pick random coeffs and check if the universal hash function
    // has collisions
    for (int i = 0; i < ArrayLength(cities_coefs); i += 1)
    {
      cities_coefs[i] = rand() % CITIES_MAX;
    }
    memset(cities_name_per_index, 0, sizeof(cities_name_per_index));
    b32 success = 1;
#endif
    
    for (u32 i = 0; i < ArrayLength(city_names); i += 1)
    {
      u32 city_idx = compute_city_index(city_names[i].data, city_names[i].len);

#if GENERATE_CITIES_PH_COEFICIENTS
      // NOTE(fakhri): make sure we don't have collisions
      if (cities_name_per_index[city_idx])
      {
        printf("%s is conflicting with %s\n", city_names[i].data, cities_name_per_index[city_idx]);
        success = 0;
        break;
      }
#endif
      Assert(!cities_name_per_index[city_idx]);
      cities_name_per_index[city_idx] = city_names[i].data;
    }

#if GENERATE_CITIES_PH_COEFICIENTS
    if (success)
    {
      printf("found good coefficients for perfect hashing: ");
      for (int c = 0; c < 4; c += 1)
      {
        printf("%d ", cities_coefs[c]);
      }
      printf("\n");
      break;
    }
  }
#endif
}

static void 
fill_fruits_name_per_index_table()
{
#if GENERATE_FRUITS_PH_COEFICIENTS
  time_t t;
  srand(time(&t));
  for(;;)
  {
    // NOTE(fakhri): pick random coeffs and check if the universal hash function
    // has collisions
    for (int i = 0; i < ArrayLength(fruits_coefs); i += 1)
    {
      fruits_coefs[i] = rand() % PRODUCTS_MAX;
    }
    memset(fruits_name_per_index, 0, sizeof(fruits_name_per_index));
    b32 success = 1;
#endif
    
    for (u32 i = 0; i < ArrayLength(fuits_and_vegs); i += 1)
    {
      u32 fruits_idx = compute_product_index(fuits_and_vegs[i].data, fuits_and_vegs[i].len);
      
#if GENERATE_FRUITS_PH_COEFICIENTS
      // NOTE(fakhri): make sure we don't have collisions
      if (fruits_name_per_index[fruits_idx])
      {
        //printf("%s is conflicting with %s\n", fuits_and_vegs[i].data, fruits_name_per_index[fruits_idx]);
        success = 0;
        break;
      }
#endif

      Assert(!fruits_name_per_index[fruits_idx]);
      fruits_name_per_index[fruits_idx] = fuits_and_vegs[i].data;
    }
    
#if GENERATE_FRUITS_PH_COEFICIENTS
    if (success)
    {
      printf("found good coefficients for perfect hashing: ");
      for (int c = 0; c < ArrayLength(fruits_coefs); c += 1)
      {
        printf("%d ", fruits_coefs[c]);
      }
      printf("\n");
      break;
    }
  }
#endif
}

static u64 product_min_price_per_city[CITIES_MAX][PRODUCTS_MAX]; // about 1.5MB

static u64 cities_prices[CITIES_MAX];
static b8 valid_cities[CITIES_MAX];

static char *mapped_file;
static u64 mapped_file_size;
static u64 offset_into_mapped_file;

static void
map_file_to_memory(int fd)
{
  struct stat buf;
  fstat(fd, &buf);
  mapped_file_size = buf.st_size;
  
  mapped_file = (char*)mmap(0, mapped_file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  Assert(mapped_file != MAP_FAILED);
}

static void 
process_file_chunk(char *chunk_buf, u64 chunk_size)
{
  u32 offset = 0;
  if (chunk_buf != mapped_file && chunk_buf[- 1] != '\n')
  {
    // NOTE(fakhri): we are not at the start of a new line, move to the next line
    for (offset = 0; (offset < chunk_size) && (chunk_buf[offset++] != '\n'););
  }
  
  for (;offset < chunk_size;)
  {
    // TODO(fakhri): attempt prefetching?
    char *city = chunk_buf + offset;
    u32 city_len = 0;
    for (;city[city_len] != ',';city_len++);
    
    char *fruit = city + city_len + 1;
    u32 fruit_len = 0;
    for (;fruit[fruit_len] != ',';fruit_len++);
    
    char *price_decimal_str = fruit + fruit_len + 1;
    u32 price_decimal_len = 0;
    for (;price_decimal_str[price_decimal_len] != '.';price_decimal_len++);
    u32 price_decimal = convert_to_int(price_decimal_str, price_decimal_len);
    
    char *price_fractional_str = price_decimal_str + price_decimal_len + 1;
    u32 price_fractional_len = 0;
    for (;price_fractional_str[price_fractional_len] != '\n';price_fractional_len++);
    u32 price_fractional = convert_to_int(price_fractional_str, price_fractional_len);
    price_fractional *= (price_fractional_len < 2)? 10:1; 
    
    u32 price = price_decimal * 100 + price_fractional;
    
    u32 city_idx = compute_city_index(city, city_len);
    u32 product_idx = compute_product_index(fruit, fruit_len);
    
    offset += city_len + fruit_len + price_decimal_len + price_fractional_len + 4;
    
    __atomic_store_n(&valid_cities[city_idx], 1, __ATOMIC_SEQ_CST);
    __atomic_fetch_add(&cities_prices[city_idx], price, __ATOMIC_SEQ_CST);
    
    u64 stored_price = 0;
    u64 new_min = 0;
    b32 success = 0;
    do
    {
      stored_price = __atomic_load_n(&product_min_price_per_city[city_idx][product_idx], __ATOMIC_SEQ_CST);
      if (stored_price < price) break;
      new_min = MIN(stored_price, price);
      success = __atomic_compare_exchange_n(&product_min_price_per_city[city_idx][product_idx], &stored_price, new_min, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    } while (!success);
    product_min_price_per_city[city_idx][product_idx] = MIN(product_min_price_per_city[city_idx][product_idx], price);
  }
}

static void *thread_main(void *args)
{
  volatile u32 *running_threads_count = (volatile u32 *)args;
  for (;offset_into_mapped_file < mapped_file_size;)
  {
    u64 my_offset = __atomic_fetch_add(&offset_into_mapped_file, CHUNK_SIZE, __ATOMIC_SEQ_CST);
    u64 chunk_size = MIN(CHUNK_SIZE, mapped_file_size - my_offset);
    process_file_chunk(mapped_file + my_offset, chunk_size);
  }
  
  __atomic_fetch_sub(running_threads_count, 1, __ATOMIC_SEQ_CST);
  return 0;
}

int main()
{
  double timing = 0;
  compute_timing(timing)
  {
    fill_cities_name_per_index_table();
    fill_fruits_name_per_index_table();
    
    int fd = open("input.txt", O_RDONLY);
    Assert(fd != -1);
    map_file_to_memory(fd);
    
    memset(product_min_price_per_city, -1, sizeof(product_min_price_per_city));
    
    u32 thread_cnt = std::thread::hardware_concurrency();
    volatile u32 thread_cnt_arg = thread_cnt;
    
    for (u32 i = 1; i < thread_cnt; i += 1)
    {
      pthread_t pid;
      pthread_create(&pid, 0, thread_main, (void*)&thread_cnt_arg);
    }
    
    thread_main((void*)&thread_cnt_arg);

    // NOTE(fakhri): wait for all threads to finishe
    while (thread_cnt_arg)
    {
      _mm_pause();
    }
    
    // NOTE(fakhri): find cheapest city
    u64 min_price = (u64)-1;
    u64 min_city_idx = (u64)-1;
    
    for (u32 i = 0; i < ArrayLength(city_names); i += 1)
    {
      u32 city_idx = compute_city_index(city_names[i].data, city_names[i].len);
      if (valid_cities[city_idx] && cities_prices[city_idx] < min_price)
      {
        min_price = cities_prices[city_idx];
        min_city_idx = city_idx;
      }
    }
    
    u64 min_fruits_prices[6] = {(u64)-1, (u64)-1, (u64)-1, (u64)-1, (u64)-1, (u64)-1};
    u32 min_fruits_idx[6];
    u32 fruits_cnt = 0;
    
    for (u32 i = 0; i < ArrayLength(fuits_and_vegs); i += 1)
    {
      u32 fruit_idx = compute_product_index(fuits_and_vegs[i].data, fuits_and_vegs[i].len);
      u64 price = product_min_price_per_city[min_city_idx][fruit_idx];
      if (price < (u64)(-1))
      {
        min_fruits_prices[5] = price;
        min_fruits_idx[5] = fruit_idx;
        u32 correct_place = 5;
        while (
          correct_place > 0 && 
          min_fruits_prices[correct_place] < min_fruits_prices[correct_place - 1]
        ) {
          SWAP(min_fruits_prices[correct_place], min_fruits_prices[correct_place - 1], u64);
          SWAP(min_fruits_idx[correct_place], min_fruits_idx[correct_place - 1], u32);
          
          correct_place -= 1;
        }
      }
    }
    
    FILE *fout = fopen("output.txt", "w");
    fprintf(fout, "%s %lu.%.2lu\n", cities_name_per_index[min_city_idx], min_price / 100, min_price % 100);
    for (int i = 0; i < 5; i += 1)
    {
      if (min_fruits_prices[i] == (u64)-1) break;
      
      u32 fruit_idx = min_fruits_idx[i];
      u64 fruit_price = min_fruits_prices[i];
      fprintf(fout, "%s %lu.%.2lu\n", fruits_name_per_index[fruit_idx], fruit_price / 100, fruit_price % 100);
    }
  }
  
  munmap(mapped_file, mapped_file_size);
  printf("timing: %fs\n", timing);
  return 0;
}