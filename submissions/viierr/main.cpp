// g++ main.cxx -Ofast -o blanat, works only with the given constraints, cities and produce
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <limits>
#include <sys/wait.h>
#include <cstring>
#include <algorithm>
#include <iostream>

constexpr size_t MAX_CITY_IDX = 2046;
constexpr size_t MAX_PROD_IDX = 1017;

constexpr size_t CITIES_MAX = 101;
constexpr size_t PRODUCTS_MAX = 94;

constexpr size_t NUM_PROCESSES = 16;

constexpr unsigned short city_r_table[] = {1705, 1832, 787, 1532, 421, 1943, 1778, 1885, 1314, 730, 2027, 735, 1966, 971, 1761,
                                785, 781, 870, 1962, 1273, 724, 17, 1525, 1450, 1902, 1115, 1448, 1640, 1680, 1798, 51,
                                1285, 72, 1769, 75, 819, 308, 571, 975, 278, 1034, 1685, 703, 1038, 1377, 1713, 1677, 1175,
                                1351, 1990, 1787, 138, 514, 1764, 1021, 160, 1634, 1262, 154, 1524, 653, 1939, 1878, 1117, 1981,
                                1229, 511, 1205, 1949, 121, 420, 1178, 366, 193, 1947, 1658, 1569, 1451, 1477, 668, 1950, 437, 30,
                                245, 927, 1643, 796, 100, 904, 591, 242, 511, 318, 94, 1186, 256, 555, 2030, 920, 609, 644, 1217,
                                1134, 1487, 350, 1610, 1890, 1880, 1732, 712, 1567, 746, 845, 1031, 185, 1896, 1112, 179, 1246,
                                198, 1033, 1732, 449, 1785, 159, 1902, 895, 428, 838, 249, 971, 37, 92, 705, 1748, 843, 1747,
                                1919, 980, 559, 627, 605, 730, 1068, 1028, 633, 180, 1180, 1239, 268, 946, 1666, 577, 1469, 48,
                                1719, 1989, 1313, 1269, 324, 704, 1228, 1257, 198, 2015, 1207, 910, 1111, 1597, 1812, 1945, 656, 1861,
                                613, 671, 1309, 1753, 1490, 930, 1596, 862, 1043, 100, 65, 104, 813, 625, 572, 218, 302, 1558, 1660, 1995,
                                1647, 1189, 1895, 2016, 1751, 1949, 1508, 647, 669, 281, 1718, 717, 1043, 1289, 1087, 4, 878, 1229, 108, 1277,
                                200, 517, 381, 1152, 135, 1621, 1338, 751, 2046, 1027, 5, 66, 1392, 1080, 575, 475, 1394, 538, 1332, 698, 1385,
                                 1520, 767, 1320, 879, 1281, 940, 1696, 1413, 1661, 457, 649, 2030, 765, 120, 1088, 1832, 1296, 739, 698, 295, 1894, 0};

constexpr unsigned short city_hashes[] = {767, 2014, 1293, 525, 1427, 667, 900, 39, 481, 174, 1343, 1644, 1067, 450, 606, 1235, 567, 102, 1541, 98, 1832, 1310,
                                303, 199, 1327, 1552, 1922, 1411, 764, 145, 711, 274, 1345, 1082, 444, 163, 1887, 948, 25, 424, 294, 2028, 1751, 1550,
                                1266, 1562, 2022, 1199, 358, 1713, 1931, 419, 1848, 279, 1192, 359, 1925, 954, 1628, 552, 1932, 1494, 1505, 801, 848,
                                698, 222, 1203, 1984, 356, 1610, 20, 1905, 252, 1806, 2031, 648, 1627, 137, 1111, 803, 644, 1184, 1634, 241, 1902, 1830,
                                625, 2034, 1586, 1223, 2046, 739, 1435, 1496, 329, 1014, 1162, 1677, 594, 1911, 0};


constexpr const char* cities[] = {"Casablanca", "Rabat", "Marrakech", "Fes", "Tangier",
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
                            "Imzouren", "Zemamra", "Sidi_Kacem", "Drarga", "Skhirate"};


constexpr unsigned short prod_r_table[] = {833, 610, 576, 766, 85, 498, 806, 994, 989, 539, 914, 792, 139, 590,
                                 827, 659, 667, 1011, 793, 151, 537, 161, 715, 194, 805, 650, 270, 175,
                                 192, 760, 922, 99, 828, 174, 537, 770, 70, 209, 455, 135, 77, 166, 665,
                                 594, 138, 494, 1022, 959, 131, 751, 408, 386, 756, 537, 739, 786, 667, 
                                 796, 37, 997, 267, 382, 823, 511, 413, 1005, 154, 209, 223, 250, 146, 926, 
                                 609, 986, 21, 523, 772, 621, 658, 96, 0, 946, 277, 36, 961, 996, 933, 212, 979,
                                 767, 191, 256, 159, 45, 345, 407, 59, 397, 437, 787, 432, 143, 95, 315, 129, 440,
                                 612, 3, 890, 98, 1015, 304, 589, 943, 1018, 905, 731, 165, 555, 284, 581, 604, 646,
                                 372, 624, 990, 609, 0};

constexpr unsigned short prod_hashes[] = {920, 525, 1017, 511, 92, 359, 115, 876, 28, 84, 302, 232, 497, 890, 958, 567,
                                539, 840, 684, 695, 791, 593, 87, 198, 651, 508, 364, 352, 838, 412, 994, 964,
                                761, 770, 178, 957, 29, 397, 927, 794, 899, 417, 866, 485, 279, 37, 598, 569, 235,
                                632, 703, 272, 215, 577, 767, 162, 754, 357, 17, 145, 532, 744, 698, 109, 418, 24,
                                56, 839, 939, 774, 25, 971, 760, 643, 213, 737, 747, 172, 346, 120, 356, 919, 156,
                                732, 423, 784, 245, 656, 464, 587, 616, 184, 570, 874, 0};

constexpr const char* products[] = {"Radish", "Celery", "Sweet_Potato", "Acorn_Squash", "Basil", "Dill", "Kale", "Zucchini",
                          "Mango", "Orange", "Lemon", "Onion", "Thyme", "Brussels_Sprouts", "Squash_Blossom", "Cranberry",
                          "Blackberry", "Grapefruit", "Goji_Berry", "Carrot", "Kiwi", "Lime", "Parsley", "Pumpkin", "Peas",
                          "Parsnip", "Eggplant", "Cucumber", "Rhubarb", "Papaya", "Green_Beans", "Asparagus", "Mint", "Bok_Choy",
                          "Lettuce", "Plum", "Sage", "Potato", "Ginger", "Apricot", "Persimmon", "Endive", "Nectarine", "Kiwano",
                          "Banana", "Clementine", "Honeydew", "Spinach", "Starfruit", "Bell_Pepper", "Pomegranate", "Guava", "Cactus_Pear",
                          "Beet", "Butternut_Squash", "Grapes", "Strawberry", "Cabbage", "Fig", "Watercress", "Okra", "Avocado", "Kohlrabi",
                          "Dragon_Fruit", "Broccoli", "Apple", "Garlic", "Cauliflower", "Collard_Greens", "Date", "Plantain", "Passion_Fruit",
                          "Pear", "Cherry", "Currant", "Salsify", "Jackfruit", "Blueberry", "Artichoke", "Cilantro", "Oregano", "Chard",
                          "Turnip", "Jicama", "Watermelon", "Yam", "Tomato", "Peach", "Cantaloupe", "Coconut", "Rutabaga", "Pineapple", 
                          "Raspberry", "Rosemary"};

unsigned long* sh_cities_data[NUM_PROCESSES + 1];
unsigned long* sh_prod_data[NUM_PROCESSES + 1];

std::pair<char*, size_t> memory_map_file(const std::string& filename) {
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd < 0) {
        puts("an error occured");
        exit(1);
    }
    struct stat fst;
    if (fstat(fd, &fst) < 0 || fst.st_size == 0) {
        puts("an error occured");
        exit(1);
    }
    char* file_contents = (char*)mmap(NULL, fst.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (file_contents == MAP_FAILED) {
        puts("an error occured");
        exit(1);
    }
    return {file_contents, fst.st_size};
}

void process_chunk(char* chunk_data, const size_t chunk_sz, int proc_id) {
    char* start = chunk_data;
    char* end = chunk_data + chunk_sz;
    while (start < end) {
        unsigned long price = 0;
        size_t city_hash = 0, product_hash = 0;

        while (*start != ',') {
            city_hash ^= city_r_table[(unsigned char)(*start)];
            start++;
        }
        start += 1;
        
        while (*start != ',') {
            product_hash ^= prod_r_table[(unsigned char)(*start)];
            start++;
        }
        start += 1;
        while (*start != '.') {
            price = price * 10 + (*start - '0');
            start++;
        }
        start += 1;
        char* b = start;
        while (*start != '\n') {
            price = price * 10 + (*start - '0');
            start++;
        }
        if (start - b < 2)
            price *= 10;
        start += 1;
        if (price < sh_cities_data[proc_id][product_hash]) {
            sh_cities_data[proc_id][product_hash] = price;
        }
        sh_prod_data[proc_id][city_hash] += price;
    }
}

std::pair<std::string_view, unsigned long> get_city_results() {
    unsigned long cheapest_city_price = std::numeric_limits<unsigned long>::max();
    std::string_view cheapest_city;
    size_t err_c = 0;
    for (size_t i = 0; i < CITIES_MAX; i++) {
        unsigned long city_sum = 0;
        for (size_t j = 0; j < NUM_PROCESSES; j++) {
            city_sum += sh_prod_data[j][city_hashes[i]];
        }
        if (city_sum != 0 && city_sum < cheapest_city_price) {
            cheapest_city_price = city_sum;
            cheapest_city = cities[i];
        }
    }
    return {cheapest_city, cheapest_city_price};
}

std::vector<std::pair<std::string_view, unsigned long>> get_prod_results() {
    std::vector<std::pair<std::string_view, unsigned long>> res;
    res.reserve(PRODUCTS_MAX);
    for (size_t i = 0; i < PRODUCTS_MAX; i++) {
        unsigned long val = std::numeric_limits<unsigned long>::max(); 
        std::string_view prod = products[i];
        for (size_t j = 0; j < NUM_PROCESSES; j++) {
            if (sh_cities_data[j][prod_hashes[i]] < val)
                val = sh_cities_data[j][prod_hashes[i]];
        }
        res.push_back({prod, val});
    }
    std::sort(res.begin(), res.end(), [](const auto& a, const auto& b) {
    if (a.second != b.second)
            return a.second < b.second;
        return a.first < b.first;
    });
    return res;
}

inline std::string	lu_to_str(unsigned long n)
{
	std::string res = std::to_string(n / 100);
	res += ".";
	if (n % 100 < 10) 
    res += "0";
	res += std::to_string(n % 100);
	return res;
}


int main() {
    auto [fc, fsz] = memory_map_file("input.txt");

    for (size_t i = 0; i < NUM_PROCESSES; i++) {
        sh_cities_data[i] = (unsigned long*)mmap(NULL, (MAX_PROD_IDX + 1) * sizeof(unsigned long), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        sh_prod_data[i] = (unsigned long*)mmap(NULL, (MAX_CITY_IDX + 1) * sizeof(unsigned long), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        memset(sh_prod_data[i], 0, (MAX_CITY_IDX + 1) * sizeof(unsigned long));
        std::fill(sh_cities_data[i], sh_cities_data[i] + MAX_PROD_IDX + 1, std::numeric_limits<unsigned long>::max());
    }

    size_t last_chunk_end = 0;
    size_t chunk_sz = fsz / NUM_PROCESSES;
    for (size_t i = 0; i < NUM_PROCESSES; i++) {
        size_t chunk_start = last_chunk_end;
        size_t chunk_end = (i == NUM_PROCESSES - 1) ? fsz : chunk_start + chunk_sz;
        while (fc[chunk_end] != '\n' && chunk_end < fsz) {
            chunk_end++;
        }
        last_chunk_end = chunk_end + 1;
        pid_t pid = fork();
        if (!pid) {
            process_chunk(fc + chunk_start, chunk_end - chunk_start, i);
            exit(0);
        }
    }

    for (size_t i = 0; i < NUM_PROCESSES; ++i) {
        wait(NULL);
    }

    auto [city, city_price] = get_city_results();
    auto prod_res = get_prod_results();
    int out = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    dup2(out, 1);
    printf("%s %s\n", city.begin(), lu_to_str(city_price).c_str());
    for (size_t i = 0; i < 5; i++) {
        auto& [product, price] = prod_res[i];
        if (price <= 10000 && price > 10)
            printf("%s %s\n", product.begin(), lu_to_str(price).c_str());
    }
    return 0;
}
