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
#include <x86intrin.h>

#define ENABLE_PROFILING 0
#define ENABLE_ASSERTS 0

#if ENABLE_ASSERTS
# define AssertBreak (*(volatile int *)0 = 0)
# define Assert(b) do {if (!(b)) { printf("Assertion Assert(%s) failed", #b); AssertBreak; }} while (0)
#else
# define AssertBreak
# define Assert(b) (b)
#endif

#define MIN(a, b) (((a) < (b))? (a) : (b))
#define MAX(a, b) (((a) > (b))? (a) : (b))
#define SWAP(a, b, T) do {T t = (a); (a) = (b); (b) = t;} while(0)

#define ArrayLength(arr) (sizeof(arr) / sizeof(*arr))

#define defer_loop(begin, end) for(int _i_ = ((begin), 0); _i_ < 1; _i_ += 1, (end))

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint8_t u8;
typedef int64_t i64;
typedef int32_t i32;
typedef int8_t i8;
typedef int8_t b8;
typedef int32_t b32;

#define MAX_THREDS_COUNT 16

u64 thread_cnt = std::thread::hardware_concurrency();
static volatile u32 running_threads_count;
static __thread u64 thread_id;

struct Timing
{
  u32 parent;
  u64 last_timing;
  u64 sum_timing;
  u32 samples;
};
struct Profile_Data
{
  char *name;
  Timing timings[MAX_THREDS_COUNT];
};
static u32 profil_active_scope[MAX_THREDS_COUNT];
static Profile_Data profile_data_points[1024];
static void start_profiling(int i, char *name)
{
  profile_data_points[i].name = name;;
  profile_data_points[i].timings[thread_id].parent = profil_active_scope[thread_id];
  profile_data_points[i].timings[thread_id].last_timing = __rdtsc();
  profil_active_scope[thread_id] = i;
}
static void end_profiling(int i)
{
  Timing *timing = &profile_data_points[i].timings[thread_id];
  
  timing->last_timing = __rdtsc() - timing->last_timing;
  timing->sum_timing += timing->last_timing;
  timing->samples += 1;
  
  profil_active_scope[thread_id] = timing->parent;
};

#if ENABLE_PROFILING
# define profile_code_idx(name, idx) defer_loop(start_profiling(idx, name), end_profiling(idx))
# define profile_code(name) profile_code_idx(name, __COUNTER__)
#else
# define profile_code_idx(name, idx)
# define profile_code(name)
#endif

static u64 THREAD_CHUNK_SIZE = (1ull << 30);

#define CITIES_MAX 523
#define PRODUCTS_MAX 409

struct Thread_Ctx
{
  char *chunk_buf;
  u64 chunk_size;
  
  u64 product_min_price_per_city[CITIES_MAX][PRODUCTS_MAX];
  u64 cities_prices[CITIES_MAX];
};

static Thread_Ctx threads_ctx[MAX_THREDS_COUNT];

static inline u32 
convert_to_int(char *number, u32 number_len)
{
  // NOTE(fakhri): this assumes number_len is always at least 1, and if it is 1 the next 2 bytes are 
  // always valid, in our case this holds, if we are at the last line and the fraction part length is 1
  // the next character will be '\n' and the next one will be the null terminator
  static u32 sub_zeros[4] = {0, '0', 11 * '0', 111 * '0'};
  static u32 coeffs[3][4] = {
    {0, 1, 10, 100},
    {0, 0, 1,   10},
    {0, 0, 0,    1},
  };
  u32 result = (
    coeffs[0][number_len] * number[0] + 
    coeffs[1][number_len] * number[1] + 
    coeffs[2][number_len] * number[2] -
    sub_zeros[number_len]
  );
  return result;
}

static u32 cities_coefs[4] = {226, 371, 516, 292};
static u32 fruits_coefs[4] = {253, 46, 119, 133};

static u32
compute_product_index(char *product, u32 len)
{
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

// NOTE(fakhri): THIS ASSUMES THE CITIES AND PRODUCTS WE WILL BE TESTED AGAINST
// ARE THE SAME AS THE ONE IN GEN.PY!!!!!!!

static void 
fill_cities_name_per_index_table()
{
    for (u32 i = 0; i < ArrayLength(city_names); i += 1)
    {
      u32 city_idx = compute_city_index(city_names[i].data, city_names[i].len);
      Assert(!cities_name_per_index[city_idx]);
      cities_name_per_index[city_idx] = city_names[i].data;
    }
}

static void 
fill_fruits_name_per_index_table()
{
    for (u32 i = 0; i < ArrayLength(fuits_and_vegs); i += 1)
    {
      u32 fruits_idx = compute_product_index(fuits_and_vegs[i].data, fuits_and_vegs[i].len);
      Assert(!fruits_name_per_index[fruits_idx]);
      fruits_name_per_index[fruits_idx] = fuits_and_vegs[i].data;
    }
}

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
  Thread_Ctx *thread_ctx = threads_ctx + thread_id; 
  
  u32 offset = 0;
  if (chunk_buf != mapped_file && chunk_buf[- 1] != '\n')
  {
    // NOTE(fakhri): we are not at the start of a new line, move to the next line
    for (offset = 0; (offset < chunk_size) && (chunk_buf[offset++] != '\n'););
  }
  
  //profile_code("Process All Line")
  {
    for (;offset < chunk_size;)
    {
      //profile_code("Process Line")
      {
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
        thread_ctx->cities_prices[city_idx] += price;
        thread_ctx->product_min_price_per_city[city_idx][product_idx] = MIN(price, thread_ctx->product_min_price_per_city[city_idx][product_idx]);
      }
    }
  }
}

static void *thread_main(void *args)
{
  thread_id = (u64)args;
  Thread_Ctx *thread_ctx = threads_ctx + thread_id;
  memset(thread_ctx->product_min_price_per_city, -1, sizeof(thread_ctx->product_min_price_per_city));
  profile_code("File Chunk Processing")
  {
    process_file_chunk(thread_ctx->chunk_buf, thread_ctx->chunk_size);
  }
  __atomic_fetch_sub(&running_threads_count, 1, __ATOMIC_SEQ_CST);
  return 0;
}

int main()
{
  thread_id = 0;
  profile_code("Main Code")
  {
    fill_cities_name_per_index_table();
    fill_fruits_name_per_index_table();
    
    int fd = open("input.txt", O_RDONLY);
    Assert(fd != -1);
    map_file_to_memory(fd);
    
    THREAD_CHUNK_SIZE = mapped_file_size / thread_cnt;
    running_threads_count = thread_cnt;
    char *chunk_buf = mapped_file;
    
    profile_code("Main Work")
    {
      for (u64 i = 1; i < thread_cnt; i += 1)
      {
        Thread_Ctx *thread_ctx = threads_ctx + i;
        thread_ctx->chunk_buf = chunk_buf; 
        chunk_buf += THREAD_CHUNK_SIZE;
        thread_ctx->chunk_size = THREAD_CHUNK_SIZE;
        pthread_t pid;
        pthread_create(&pid, 0, thread_main, (void*)i);
      }
      
      
      // NOTE(fakhri): main thread handles the last chunk and the remaining data
      {
        Thread_Ctx *thread_ctx = &threads_ctx[0];
        thread_ctx->chunk_buf = chunk_buf;
        thread_ctx->chunk_size = mapped_file_size - THREAD_CHUNK_SIZE * (thread_cnt - 1);
        thread_main((void*)thread_id);
      }
      
      // NOTE(fakhri): wait for all threads to finishe
      while (running_threads_count)
      {
        _mm_pause();
      }
    }
    
    u64 min_price = (u64)-1;
    u64 min_city_idx = (u64)-1;
    
    u64 min_fruits_prices[6] = {(u64)-1, (u64)-1, (u64)-1, (u64)-1, (u64)-1, (u64)-1};
    u32 min_fruits_idx[6];
    u32 fruits_cnt = 0;
    
    // NOTE(fakhri): find cheapest city
    profile_code("Combine Results From Threads")
    {
      for (u32 i = 0; i < ArrayLength(city_names); i += 1)
      {
        u32 city_idx = compute_city_index(city_names[i].data, city_names[i].len);
        
        u64 price = 0;
        for (u32 tid = 0; tid < thread_cnt; tid += 1)
        {
          price += threads_ctx[tid].cities_prices[city_idx];
        }
        
        if (price && price < min_price)
        {
          min_price = price;
          min_city_idx = city_idx;
        }
      }
      
      for (u32 i = 0; i < ArrayLength(fuits_and_vegs); i += 1)
      {
        u32 fruit_idx = compute_product_index(fuits_and_vegs[i].data, fuits_and_vegs[i].len);
        u64 price = (u64)-1;
        for (u32 tid = 0; tid < thread_cnt; tid += 1)
        {
          price = MIN(price, threads_ctx[tid].product_min_price_per_city[min_city_idx][fruit_idx]);
        }
        
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
    fclose(fout);

  }
  
  profile_code("Unmapping timing")
  {
    munmap(mapped_file, mapped_file_size);
  }

#if ENABLE_PROFILING
  for (int i = 0; i < __COUNTER__; i += 1)
  {
    printf("[timings: %s]:\n", profile_data_points[i].name);
    for (u32 t = 0; t < thread_cnt; t += 1)
    {
      Timing *timing = &profile_data_points[i].timings[t];
      if (timing->samples)
      {
        Timing *parent_timing = &profile_data_points[timing->parent].timings[t];
        
        double avg_timing = (double)timing->sum_timing / (double)timing->samples;
        double parent_avg_timing = (double)parent_timing->sum_timing / (double)parent_timing->samples;
        
        printf("\t[thread:%d---avg:%f---perc_par:%f]\n", t, (double)timing->sum_timing / (double)timing->samples, avg_timing / parent_avg_timing);
      }
    }
  }
#endif

  return 0;
}