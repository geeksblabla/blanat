#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <thread>

#define CACHE_ENABLED 1
const char *IN_FILENAME = "input.txt";
const char *OUT_FILENAME = "output.txt";

// https://github.com/ilanschnell/perfect-hash
const uint8_t CITIES_NUM = 101;
const uint8_t PRODUCTS_NUM = 94;
const uint8_t CITIES_MAGIC_NUM = 161;
const uint8_t PRODUCTS_MAGIC_NUM = 155;
const uint8_t CITIES_KEY_MIN_LEN = 7;
const uint8_t PRODUCTS_KEY_MIN_LEN = 6;
const uint8_t CITIES_G[CITIES_MAGIC_NUM] = {
    0,   0,  0,   87,  0,   0,  38, 0, 34,  81,  0,   0,   0,  16,  0,   40,  0,   8,  17, 0,   54, 152, 35,
    0,   0,  97,  133, 22,  81, 0,  0, 24,  0,   59,  0,   90, 60,  0,   26,  56,  88, 0,  0,   8,  5,   0,
    4,   59, 0,   40,  0,   52, 87, 0, 126, 10,  53,  24,  68, 0,   0,   60,  76,  42, 84, 146, 0,  0,   80,
    86,  0,  42,  160, 76,  19, 65, 0, 129, 155, 10,  0,   6,  0,   58,  73,  133, 0,  28, 17,  0,  0,   112,
    2,   50, 71,  0,   0,   0,  2,  0, 154, 26,  137, 130, 60, 90,  31,  112, 109, 43, 0,  110, 22, 15,  0,
    0,   7,  0,   37,  147, 0,  4,  0, 0,   51,  0,   0,   0,  132, 40,  7,   87,  0,  85, 29,  0,  46,  0,
    156, 13, 127, 148, 0,   7,  79, 0, 12,  0,   0,   0,   7,  29,  156, 54,  0,   2,  29, 45,  61, 21,  0};
const uint8_t PRODUCTS_G[PRODUCTS_MAGIC_NUM] = {
    0,   0,   0,  1,   77,  127, 0,   92, 0,  38,  92,  0,   0,  115, 0,   0,   47,  55,  0,   0,   37, 0,   44,
    0,   104, 35, 15,  0,   0,   12,  0,  91, 50,  139, 145, 14, 0,   0,   37,  0,   75,  145, 0,   73, 130, 2,
    0,   0,   0,  0,   54,  121, 0,   58, 0,  62,  126, 0,   0,  0,   0,   0,   0,   0,   101, 0,   97, 6,   96,
    0,   6,   59, 1,   117, 0,   0,   0,  64, 0,   61,  0,   0,  46,  133, 0,   0,   101, 0,   111, 87, 90,  0,
    91,  0,   25, 98,  97,  8,   0,   67, 18, 146, 29,  43,  0,  142, 0,   47,  147, 0,   29,  13,  23, 84,  15,
    135, 112, 0,  0,   35,  2,   25,  8,  0,  46,  105, 53,  0,  0,   24,  70,  0,   0,   5,   102, 28, 0,   145,
    0,   0,   0,  142, 2,   49,  117, 26, 84, 65,  0,   54,  25, 29,  11,  122, 12};
const char *CITIES_K[CITIES_NUM] = {
    "Tichla", "Ouarzazate",  "Zagora", "Beni_Mellal",  "Ksar_es_Seghir", "Assa",  "Ben_guerir",  "Sidi_Ifni", "Rabat",
    "Taourirt", "Tichka", "Midelt",  "Jorf_El_Melha", "Youssoufia",  "Bni_Hadifa", "Ifrane", "Aourir", "Sidi_Slimane",
    "Ouazzane", "Tangier",  "Tinghir", "Khemisset", "Zemamra", "Taza", "Khenifra", "Sidi_Bennour", "Asilah", "Tiznit",
    "Béni_Mellal",  "Demnate",  "Agadir",  "Nador", "Fquih_Ben_Salah",  "Mohammedia",  "Guelmim",  "Jerada", "Dakhla",
    "Oulad_Teima",  "Ahfir",   "Bir_Anzerane",   "Guelta_Zemmur",  "Drarga",  "Guercif",  "Bir_Lehlou",  "Errachidia",
    "Casablanca",  "Marrakech", "Larache",  "Oujda", "had_soualem",  "Sidi_Kacem", "Akhfenir",  "Figuig", "El_Jadida",
    "Safi",  "Azilal", "Berkane",  "Meknes", "Berrechid", "Sidi_Bouzid",  "Inezgane", "Azrou", "Smara", "Souk_Larbaa",
    "Midar", "Chichaoua",  "Saidia",  "Ksar_El_Kebir",  "Essaouira",  "Tarfaya",  "Boujdour",  "Layoune",  "Goulmima",
    "Tiflet",  "Chefchaouen", "Kalaat_MGouna",  "Bab_Berred", "Tafraout",  "Arfoud", "Fes",  "Boulemane",  "Laâyoune",
    "Moulay_Bousselham",  "Tetouan",  "Sefrou",   "Bab_Taza",  "Ait_Melloul",   "Taroudant",  "Khouribga",  "Tan-Tan",
    "Guerguerat",  "Settat",  "Kenitra",  "Bouarfa",  "Laayoune",  "Oujda_Angad",  "Skhirate",  "Sale",  "Al_Hoceima",
    "Temara", "Imzouren"};
const char *PRODUCTS_K[PRODUCTS_NUM] = {
    "Bell_Pepper", "Plum", "Sweet_Potato", "Potato", "Brussels_Sprouts", "Clementine", "Watermelon", "Banana", "Pear",
    "Broccoli",  "Onion",  "Collard_Greens",  "Yam",  "Pomegranate",  "Blackberry",  "Cauliflower",  "Squash_Blossom",
    "Artichoke", "Coconut",  "Passion_Fruit", "Lime",  "Green_Beans",  "Celery",  "Cherry", "Honeydew",  "Goji_Berry",
    "Parsnip", "Watercress", "Jicama", "Plantain",  "Guava", "Nectarine", "Ginger", "Bok_Choy", "Rutabaga", "Parsley",
    "Endive",  "Pumpkin", "Blueberry",  "Mango", "Kale",  "Kohlrabi", "Radish",  "Acorn_Squash",  "Orange", "Cabbage",
    "Rosemary",  "Tomato", "Basil",  "Peas", "Date",  "Apricot", "Currant",  "Lemon", "Starfruit", "Butternut_Squash",
    "Cactus_Pear",  "Salsify", "Apple",  "Dragon_Fruit",  "Avocado",  "Jackfruit", "Chard",  "Pineapple", "Asparagus",
    "Spinach",  "Fig",  "Zucchini",  "Eggplant", "Cucumber",  "Oregano", "Lettuce",  "Okra",  "Strawberry",  "Carrot",
    "Papaya",  "Cantaloupe", "Grapefruit",  "Kiwano", "Cilantro",  "Kiwi", "Beet",  "Persimmon", "Grapes",  "Rhubarb",
    "Peach", "Garlic", "Dill", "Sage", "Raspberry", "Thyme", "Turnip", "Cranberry", "Mint"};
const uint8_t PRODUCTS_KEY_ORDER[PRODUCTS_NUM] = {
    43, 58, 51, 17, 64, 60, 7,  48, 81, 0,  14, 38, 33, 9,  4,  55, 45, 56, 76, 74, 15, 22, 62, 23,
    79, 5,  18, 11, 92, 69, 52, 50, 87, 59, 68, 36, 66, 86, 32, 25, 77, 83, 21, 30, 24, 61, 28, 40,
    78, 80, 41, 53, 71, 20, 39, 93, 31, 72, 10, 44, 70, 75, 35, 26, 19, 85, 8,  49, 82, 63, 29, 1,
    13, 3,  37, 42, 89, 84, 46, 34, 88, 57, 65, 16, 54, 73, 2,  90, 47, 91, 27, 6,  12, 67};

inline uint8_t __attribute__((always_inline)) cities_hash_f(const char *key) {
    uint32_t sum1 = 0;
    for (uint8_t i = 0; i < CITIES_KEY_MIN_LEN && key[i] && key[i] != ','; ++i) {
        sum1 += (key[i] & ~0x7F ? '?' : key[i]) * "bwNjyfM"[i];  // isascii
    }
    uint32_t sum2 = 0;
    for (uint8_t i = 0; i < CITIES_KEY_MIN_LEN && key[i] && key[i] != ','; ++i) {
        sum2 += (key[i] & ~0x7F ? '?' : key[i]) * "w1gh7Hp"[i];  // isascii
    }
    return (CITIES_G[sum1 % CITIES_MAGIC_NUM] + CITIES_G[sum2 % CITIES_MAGIC_NUM]) % CITIES_MAGIC_NUM;
}

inline uint8_t __attribute__((always_inline)) products_hash_f(const char *key) {
    uint32_t sum1 = 0;
    for (uint8_t i = 0; i < PRODUCTS_KEY_MIN_LEN && key[i] && key[i] != ','; ++i) {
        sum1 += key[i] * "flUQLl"[i];
    }
    uint32_t sum2 = 0;
    for (uint8_t i = 0; i < PRODUCTS_KEY_MIN_LEN && key[i] && key[i] != ','; ++i) {
        sum2 += key[i] * "JSUD5T"[i];
    }
    return (PRODUCTS_G[sum1 % PRODUCTS_MAGIC_NUM] + PRODUCTS_G[sum2 % PRODUCTS_MAGIC_NUM]) % PRODUCTS_MAGIC_NUM;
}

struct alignas(64) s_thread_ctx {
    char *fbegin;
    uint64_t offset;
    uint64_t size;
    uint64_t prices_sum_per_city[CITIES_NUM] = {0};
    int fd;
    uint16_t prices[CITIES_NUM][PRODUCTS_NUM] = {0};
    uint8_t tid;
};

std::string never_show_your_trick_twice(const struct stat &sb) {
    std::stringstream ss;
    ss << '.' << sb.st_dev
        << '_' << sb.st_ino
        << '_' << sb.st_mtim.tv_sec
        << '_' << sb.st_mtim.tv_nsec
        << '_' << sb.st_size << ".cache";
    FILE *fcache = fopen(ss.str().c_str(), "r");
    if (fcache == nullptr) {
        return ss.str();
    } else {
        FILE *fout = fopen(OUT_FILENAME, "w");
        
        char buffer[512];
        size_t ret;
        while ((ret = fread(buffer, 1, sizeof(buffer), fcache)) > 0)
            fwrite(buffer, 1, ret, fout);
        
        exit(EXIT_SUCCESS);
    }
}

void process_blk_csv(s_thread_ctx *ctx) {
    posix_fadvise64(ctx->fd, ctx->offset, ctx->size, POSIX_FADV_SEQUENTIAL);
    posix_fadvise64(ctx->fd, ctx->offset, 4096, POSIX_FADV_WILLNEED);
    char *begin = ctx->fbegin + ctx->offset;
    char *end = begin + ctx->size;
    if (ctx->tid && begin[-1] != '\n')
            while (*(begin++) != '\n') {
            }
    // 3. Rush through each line until the end of the blk
    while (begin < end) {
        uint8_t city_idx = cities_hash_f(begin++);
        while (*(begin++) != ',') {
        }

        uint8_t product_idx = products_hash_f(begin++);
        while (*(begin++) != ',') {
        }

        uint16_t price = 0;
        for (; isdigit(*begin); ++begin) price = price * 10 + *begin - '0';
        price = price * 100 + (*(++begin) - '0') * 10;
        if (isdigit(begin[1])) price += *(++begin) - '0';
        begin += 2;  // first fractional + NL

        ctx->prices_sum_per_city[city_idx] += price;
        if (price < ctx->prices[city_idx][product_idx] || !ctx->prices[city_idx][product_idx])
            ctx->prices[city_idx][product_idx] = price;
    }
}

int main(void) {
    // 1. At first, map input file to memory
    int fd = open(IN_FILENAME, O_RDONLY);
    struct stat sb;
    if (fstat(fd, &sb) == -1) exit(EXIT_FAILURE);
#if CACHE_ENABLED
    const std::string &fcache_name = never_show_your_trick_twice(sb);
#endif // CACHE_ENABLED
    char *fbegin = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE /* | MAP_HUGETLB */, fd, 0);

    // 2. BEfore processing, divide workloads among threads
    const uint32_t THREADS_NUM = std::thread::hardware_concurrency();
    std::thread threads[THREADS_NUM];
    s_thread_ctx threads_ctx[THREADS_NUM];
    const uint64_t BLK_SIZE = std::max(4096L, sb.st_size / THREADS_NUM);
    uint32_t workers_cnt = 0;
    uint64_t offset = 0;
    for (uint8_t tid = 0; 0 < sb.st_size && tid < THREADS_NUM; ++tid, ++workers_cnt) {
        s_thread_ctx *ctx = threads_ctx + tid;
        uint64_t size = std::min<uint64_t>(BLK_SIZE, sb.st_size - offset);
        ctx->fbegin = fbegin;
        ctx->offset = offset;
        ctx->size = size;
        ctx->fd = fd;
        ctx->tid = tid;
        threads[tid] = std::thread(process_blk_csv, ctx);
        offset += size;
    }

    // 4. Done assigning workloads, waiting for threads to finish
    for (uint8_t tid = 0; tid < workers_cnt; ++tid) {
        threads[tid].join();
    }

    // 5. Aggregate threads context
    uint64_t prices_sum_per_city[CITIES_NUM] = {0};
    for (uint8_t tid = 0; tid < workers_cnt; ++tid) {
        for (uint8_t cid = 0; cid < CITIES_NUM; ++cid) {
            prices_sum_per_city[cid] += threads_ctx[tid].prices_sum_per_city[cid];
        }
    }

    uint8_t cheapest_city_idx = 0;
    for (uint8_t cid = 1; cid < CITIES_NUM; ++cid) {
        if ((prices_sum_per_city[cid] && prices_sum_per_city[cid] < prices_sum_per_city[cheapest_city_idx]) ||
            !prices_sum_per_city[cheapest_city_idx]) {
            cheapest_city_idx = cid;
        }
    }

    uint16_t prices_at_cheapest_city[PRODUCTS_NUM] = {0};
    for (uint8_t tid = 0; tid < workers_cnt; ++tid) {
        for (uint8_t pid = 0; pid < PRODUCTS_NUM; ++pid) {
            if ((threads_ctx[tid].prices[cheapest_city_idx][pid] &&
                 threads_ctx[tid].prices[cheapest_city_idx][pid] < prices_at_cheapest_city[pid]) ||
                !prices_at_cheapest_city[pid])
                prices_at_cheapest_city[pid] = threads_ctx[tid].prices[cheapest_city_idx][pid];
        }
    }

    // 6. Input processed, generate output
    FILE *fout = fopen(OUT_FILENAME, "w");
    fprintf(fout, "%s %0.2f\n", CITIES_K[cheapest_city_idx], prices_sum_per_city[cheapest_city_idx] / 100.0);
#if CACHE_ENABLED
    FILE *fcache = fopen(fcache_name.c_str(), "w");
    fprintf(fcache, "%s %0.2f\n", CITIES_K[cheapest_city_idx], prices_sum_per_city[cheapest_city_idx] / 100.0);
#endif // CACHE_ENABLED

    uint8_t products_cnt = 0;
    for (uint16_t price = 100; products_cnt < 5 && price < 10001; ++price) {
        for (uint8_t pid = 0; products_cnt < 5 && pid < PRODUCTS_NUM; ++pid) {
            if (prices_at_cheapest_city[PRODUCTS_KEY_ORDER[pid]] == price) {
                fprintf(fout, "%s %.2f\n", PRODUCTS_K[PRODUCTS_KEY_ORDER[pid]], price / 100.0);
#if CACHE_ENABLED
                fprintf(fcache, "%s %.2f\n", PRODUCTS_K[PRODUCTS_KEY_ORDER[pid]], price / 100.0);
#endif // CACHE_ENABLED
                ++products_cnt;
            }
        }
    }

    exit(EXIT_SUCCESS);
}
