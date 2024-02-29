#pragma GCC diagnostic ignored "-Wchar-subscripts"
#pragma GCC diagnostic ignored "-Wsign-compare"

#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

const char *IN_FILENAME = "input.txt";
const char *OUT_FILENAME = "output.txt";

const uint32_t MAX_PROCESSES = 16;

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

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
const uint8_t CITIES_LT1[CITIES_KEY_MIN_LEN][128] = {
    {0,  0,  0, 0, 0,   0,  0,  0, 0, 0, 0, 0,  0,   0,  0,  0,   0,  0, 0,  0,  0, 0,  0, 0,   0,  0,
     0,  0,  0, 0, 0,   0,  0,  0, 0, 0, 0, 0,  0,   0,  0,  0,   0,  0, 0,  0,  0, 0,  0, 0,   0,  0,
     0,  0,  0, 0, 0,   0,  0,  0, 0, 0, 0, 0,  0,   91, 28, 126, 63, 0, 98, 35, 0, 70, 7, 105, 42, 140,
     77, 14, 0, 0, 147, 84, 21, 0, 0, 0, 0, 28, 126, 0,  0,  0,   0,  0, 0,  0,  0, 0,  0, 0,   0,  0,
     49, 0,  0, 0, 0,   0,  0,  0, 0, 0, 0, 0,  0,   0,  0,  0,   0,  0, 0,  0,  0, 0,  0, 0},
    {0, 0,   0, 0, 0, 0,   0,  0,  0,   0,  0, 0,  0,   0,  0,  0, 0, 0,  0,  0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,
     0, 0,   0, 0, 0, 0,   0,  0,  0,   0,  0, 0,  0,   0,  0,  0, 0, 0,  0,  0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 91,
     0, 0,   0, 0, 0, 0,   0,  0,  0,   0,  0, 0,  0,   0,  0,  0, 0, 0,  0,  0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,
     0, 112, 0, 0, 0, 105, 63, 21, 140, 98, 0, 14, 133, 91, 49, 7, 0, 84, 42, 0, 0, 77, 0, 0, 0, 0, 28, 0, 0, 0, 0, 0},
    {0,  0,   0,  0,   0,  0,   0,  0,   0, 0, 0,  0,   0,  0,   0, 0, 0, 0,   0,  0,   0,  0,   0,  0,   0,  0,
     0,  0,   0,  0,   0,  0,   0,  0,   0, 0, 0,  0,   0,  0,   0, 0, 0, 0,   0,  0,   0,  0,   0,  0,   0,  0,
     0,  0,   0,  0,   0,  0,   0,  0,   0, 0, 0,  84,  0,  0,   0, 0, 0, 0,   0,  0,   0,  0,   0,  0,   0,  0,
     0,  0,   0,  0,   0,  0,   0,  0,   0, 0, 0,  0,   0,  0,   0, 0, 0, 4,   0,  160, 77, 155, 72, 150, 67, 145,
     62, 140, 57, 135, 52, 130, 47, 125, 0, 0, 37, 115, 32, 110, 0, 0, 0, 100, 17, 0,   0,  0,   0,  0},
    {0,  0,  0,   0,  0,  0,   0,  0,  0, 0, 0, 0,   0,  0, 0, 0, 0, 0,   0,  0,   0,  0,  0,   0,  0,  0,
     0,  0,  0,   0,  0,  0,   0,  0,  0, 0, 0, 0,   0,  0, 0, 0, 0, 0,   0,  101, 0,  0,  0,   0,  0,  0,
     0,  0,  0,   0,  0,  0,   0,  0,  0, 0, 0, 77,  0,  0, 0, 0, 0, 0,   0,  0,   65, 0,  116, 0,  0,  0,
     0,  0,  0,   0,  0,  0,   0,  0,  0, 0, 0, 0,   0,  0, 0, 0, 0, 88,  0,  139, 0,  29, 135, 80, 25, 131,
     76, 21, 127, 72, 17, 123, 68, 13, 0, 0, 9, 115, 60, 5, 0, 0, 0, 107, 52, 0,   0,  0,  0,   0},
    {0,  0,   0, 0,  0,  0,   0,   0,  0, 0, 0,   0,  0,  0,   0,  0, 0, 0,   0,   0,   0,   0,  0,  0,   0,  0,
     0,  0,   0, 0,  0,  0,   0,   0,  0, 0, 0,   0,  0,  0,   0,  0, 0, 0,   0,   0,   0,   0,  0,  0,   0,  0,
     0,  0,   0, 0,  0,  0,   0,   0,  0, 0, 0,   0,  0,  137, 97, 0, 0, 0,   0,   0,   18,  0,  0,  0,   19, 140,
     0,  0,   0, 0,  0,  0,   21,  0,  0, 0, 0,   0,  0,  0,   0,  0, 0, 64,  0,   145, 105, 65, 25, 146, 0,  66,
     26, 147, 0, 67, 27, 148, 108, 68, 0, 0, 109, 69, 29, 150, 0,  0, 0, 151, 111, 0,   0,   0,  0,  0},
    {0,   0,  0, 0,   0,  0,  0,   0,  0, 0, 0,  0,   0,  0,  0,   0, 0, 0,   0, 0,  0, 0,   0,  0,   0,   0,
     0,   0,  0, 0,   0,  0,  0,   0,  0, 0, 0,  0,   0,  0,  0,   0, 0, 0,   0, 0,  0, 0,   0,  0,   0,   0,
     0,   0,  0, 0,   0,  0,  0,   0,  0, 0, 0,  0,   0,  29, 131, 0, 0, 115, 0, 0,  0, 40,  0,  83,  24,  126,
     0,   0,  0, 0,   0,  94, 0,   0,  0, 0, 0,  0,   0,  0,  0,   0, 0, 30,  0, 73, 0, 116, 57, 159, 100, 41,
     143, 84, 0, 127, 68, 9,  111, 52, 0, 0, 36, 138, 79, 20, 0,   0, 0, 106, 0, 0,  0, 0,   0,  0},
    {0,   0,  0, 0, 0,   0,   0,  0,  0, 0, 0,  0, 0,  0,   0,  0, 0, 0,  0,  0,  0,   0, 0,   0,  0,   0,
     0,   0,  0, 0, 0,   0,   0,  0,  0, 0, 0,  0, 0,  0,   0,  0, 0, 0,  0,  0,  0,   0, 0,   0,  0,   0,
     0,   0,  0, 0, 0,   0,   0,  0,  0, 0, 0,  0, 0,  14,  91, 7, 0, 0,  77, 0,  0,   0, 0,   0,  0,   133,
     0,   0,  0, 0, 0,   112, 28, 0,  0, 0, 0,  0, 0,  0,   0,  0, 0, 70, 0,  63, 140, 0, 133, 49, 126, 0,
     119, 35, 0, 0, 105, 21,  98, 14, 0, 0, 84, 0, 77, 154, 0,  0, 0, 0,  56, 0,  0,   0, 0,   0}};
const uint8_t CITIES_LT2[CITIES_KEY_MIN_LEN][128] = {
    {0,   0,  0, 0, 0,  0,  0,  0, 0, 0, 0, 0,   0,  0, 0,   0,  0,  0, 0,   0,  0, 0,   0,   0,  0,  0,
     0,   0,  0, 0, 0,  0,  0,  0, 0, 0, 0, 0,   0,  0, 0,   0,  0,  0, 0,   0,  0, 0,   0,   0,  0,  0,
     0,   0,  0, 0, 0,  0,  0,  0, 0, 0, 0, 0,   0,  7, 126, 84, 42, 0, 119, 77, 0, 154, 112, 70, 28, 147,
     105, 63, 0, 0, 98, 56, 14, 0, 0, 0, 0, 126, 84, 0, 0,   0,  0,  0, 0,   0,  0, 0,   0,   0,  0,  0,
     140, 0,  0, 0, 0,  0,  0,  0, 0, 0, 0, 0,   0,  0, 0,   0,  0,  0, 0,   0,  0, 0,   0,   0},
    {0,   0,   0, 0,  0,   0,  0,  0,   0, 0,  0,   0,  0, 0,  0, 0, 0, 0, 0,  0,  0, 0, 0, 0,   0, 0,
     0,   0,   0, 0,  0,   0,  0,  0,   0, 0,  0,   0,  0, 0,  0, 0, 0, 0, 0,  0,  0, 0, 0, 0,   0, 0,
     0,   0,   0, 0,  0,   0,  0,  0,   0, 0,  0,   28, 0, 0,  0, 0, 0, 0, 0,  0,  0, 0, 0, 0,   0, 0,
     0,   0,   0, 0,  0,   0,  0,  0,   0, 0,  0,   0,  0, 0,  0, 0, 0, 0, 0,  84, 0, 0, 0, 119, 7, 56,
     105, 154, 0, 91, 140, 28, 77, 126, 0, 63, 112, 0,  0, 98, 0, 0, 0, 0, 21, 0,  0, 0, 0, 0},
    {0,  0,  0,   0,  0,  0,   0,  0, 0, 0, 0,   0,  0,  0,   0, 0, 0, 0,   0, 0, 0,   0,  0,   0,  0,  0,
     0,  0,  0,   0,  0,  0,   0,  0, 0, 0, 0,   0,  0,  0,   0, 0, 0, 0,   0, 0, 0,   0,  0,   0,  0,  0,
     0,  0,  0,   0,  0,  0,   0,  0, 0, 0, 0,   49, 0,  0,   0, 0, 0, 0,   0, 0, 0,   0,  0,   0,  0,  0,
     0,  0,  0,   0,  0,  0,   0,  0, 0, 0, 0,   0,  0,  0,   0, 0, 0, 125, 0, 9, 112, 54, 157, 99, 41, 144,
     86, 28, 131, 73, 15, 118, 60, 2, 0, 0, 150, 92, 34, 137, 0, 0, 0, 66,  8, 0, 0,   0,  0,   0},
    {0,  0,   0,  0,  0,   0,  0, 0,   0, 0, 0,   0,   0,   0,  0, 0, 0, 0,  0,   0,   0,  0,   0,   0,  0,   0,
     0,  0,   0,  0,  0,   0,  0, 0,   0, 0, 0,   0,   0,   0,  0, 0, 0, 0,  0,   11,  0,  0,   0,   0,  0,   0,
     0,  0,   0,  0,  0,   0,  0, 0,   0, 0, 0,   112, 0,   0,  0, 0, 0, 0,  0,   0,   82, 0,   129, 0,  0,   0,
     0,  0,   0,  0,  0,   0,  0, 0,   0, 0, 0,   0,   0,   0,  0, 0, 0, 59, 0,   106, 0,  153, 96,  39, 143, 86,
     29, 133, 76, 19, 123, 66, 9, 113, 0, 0, 103, 46,  150, 93, 0, 0, 0, 26, 130, 0,   0,  0,   0,   0},
    {0,  0,   0, 0,  0,   0,  0,   0,   0, 0, 0,   0,  0,   0,   0,  0, 0, 0,  0,   0,  0,  0,   0,  0,  0,   0,
     0,  0,   0, 0,  0,   0,  0,   0,   0, 0, 0,   0,  0,   0,   0,  0, 0, 0,  0,   0,  0,  0,   0,  0,  0,   0,
     0,  0,   0, 0,  0,   0,  0,   0,   0, 0, 0,   0,  0,   33,  88, 0, 0, 0,  0,   0,  96, 0,   0,  0,  155, 49,
     0,  0,   0, 0,  0,   0,  112, 0,   0, 0, 0,   0,  0,   0,   0,  0, 0, 73, 0,   22, 77, 132, 26, 81, 0,   30,
     85, 140, 0, 89, 144, 38, 93,  148, 0, 0, 152, 46, 101, 156, 0,  0, 0, 54, 109, 0,  0,  0,   0,  0},
    {0,  0,   0, 0,   0,  0,   0,  0,   0, 0, 0,   0,  0,   0,  0,  0, 0, 0,   0, 0,  0, 0,   0,   0,  0,   0,
     0,  0,   0, 0,   0,  0,   0,  0,   0, 0, 0,   0,  0,   0,  0,  0, 0, 0,   0, 0,  0, 0,   0,   0,  0,   0,
     0,  0,   0, 0,   0,  0,   0,  0,   0, 0, 0,   0,  0,   11, 83, 0, 0, 138, 0, 0,  0, 104, 0,   87, 159, 70,
     0,  0,   0, 0,   0,  19,  0,  0,   0, 0, 0,   0,  0,   0,  0,  0, 0, 78,  0, 61, 0, 44,  116, 27, 99,  10,
     82, 154, 0, 137, 48, 120, 31, 103, 0, 0, 158, 69, 141, 52, 0,  0, 0, 18,  0, 0,  0, 0,   0,   0},
    {0,  0, 0, 0, 0,  0,   0,  0,  0, 0, 0,  0, 0,   0,  0,   0,  0, 0,  0,   0,  0,  0, 0,  0,  0,   0,
     0,  0, 0, 0, 0,  0,   0,  0,  0, 0, 0,  0, 0,   0,  0,   0,  0, 0,  0,   0,  0,  0, 0,  0,  0,   0,
     0,  0, 0, 0, 0,  0,   0,  0,  0, 0, 0,  0, 0,   35, 147, 98, 0, 0,  112, 0,  0,  0, 0,  0,  0,   91,
     0,  0, 0, 0, 0,  119, 70, 0,  0, 0, 0,  0, 0,   0,  0,   0,  0, 14, 0,   77, 28, 0, 91, 42, 154, 0,
     56, 7, 0, 0, 21, 133, 84, 35, 0, 0, 49, 0, 112, 63, 0,   0,  0, 0,  140, 0,  0,  0, 0,  0}};
const uint8_t PRODUCTS_LT1[PRODUCTS_KEY_MIN_LEN][128] = {
    {0,  0,   0,   0, 0,   0,  0,  0, 0, 0,  0, 0,  0,  0,   0,  0,  0,   0,  0,  0,   0,  0, 0,   0,  0, 0,
     0,  0,   0,   0, 0,   0,  0,  0, 0, 0,  0, 0,  0,  0,   0,  0,  0,   0,  0,  0,   0,  0, 0,   0,  0, 0,
     0,  0,   0,   0, 0,   0,  0,  0, 0, 0,  0, 0,  0,  120, 67, 14, 116, 63, 10, 112, 59, 0, 108, 55, 2, 104,
     51, 153, 100, 0, 149, 96, 43, 0, 0, 39, 0, 88, 35, 0,   0,  0,  0,   0,  0,  0,   0,  0, 0,   0,  0, 0,
     0,  0,   0,   0, 0,   0,  0,  0, 0, 0,  0, 0,  0,  0,   0,  0,  0,   0,  0,  0,   0,  0, 0,   0},
    {0,  0,  0, 0,  0,  0, 0,   0,  0, 0,   0,  0,  0,   0,  0,  0,   0, 0, 0, 0,  0, 0,   0, 0,  0, 0,
     0,  0,  0, 0,  0,  0, 0,   0,  0, 0,   0,  0,  0,   0,  0,  0,   0, 0, 0, 0,  0, 0,   0, 0,  0, 0,
     0,  0,  0, 0,  0,  0, 0,   0,  0, 0,   0,  0,  0,   0,  0,  0,   0, 0, 0, 0,  0, 0,   0, 0,  0, 0,
     0,  0,  0, 0,  0,  0, 0,   0,  0, 0,   0,  0,  0,   0,  0,  0,   0, 0, 0, 91, 0, 152, 0, 58, 0, 119,
     72, 25, 0, 86, 39, 0, 100, 53, 6, 114, 67, 20, 128, 81, 34, 142, 0, 0, 0, 0,  0, 0,   0, 0},
    {0, 0,  0,  0,   0,  0,   0,  0,   0,  0, 0,  0,  0,  0,  0, 0,  0, 0,  0, 0,  0,   0,  0,   0,  0, 0,
     0, 0,  0,  0,   0,  0,   0,  0,   0,  0, 0,  0,  0,  0,  0, 0,  0, 0,  0, 0,  0,   0,  0,   0,  0, 0,
     0, 0,  0,  0,   0,  0,   0,  0,   0,  0, 0,  0,  0,  0,  0, 0,  0, 0,  0, 0,  0,   0,  0,   0,  0, 0,
     0, 0,  0,  0,   0,  0,   0,  0,   0,  0, 0,  0,  0,  0,  0, 0,  0, 0,  0, 30, 115, 45, 130, 60, 0, 75,
     5, 90, 20, 105, 35, 120, 50, 135, 65, 0, 80, 10, 95, 25, 0, 40, 0, 55, 0, 0,  0,   0,  0,   0},
    {0, 0,   0, 0,   0,  0,   0,  0, 0,  0, 0,  0,  0,  0,  0,   0, 0, 0,   0, 0,   0,  0,   0, 0,   0, 0,
     0, 0,   0, 0,   0,  0,   0,  0, 0,  0, 0,  0,  0,  0,  0,   0, 0, 0,   0, 0,   0,  0,   0, 0,   0, 0,
     0, 0,   0, 0,   0,  0,   0,  0, 0,  0, 0,  0,  0,  0,  0,   0, 0, 0,   0, 0,   0,  0,   0, 0,   0, 0,
     0, 0,   0, 0,   0,  0,   0,  0, 0,  0, 0,  0,  0,  0,  0,   0, 0, 100, 0, 107, 33, 114, 0, 121, 0, 128,
     0, 135, 0, 142, 68, 149, 75, 1, 82, 0, 89, 15, 96, 22, 103, 0, 0, 0,   0, 0,   0,  0,   0, 0},
    {0,   0,  0,  0,  0,   0,  0,   0,  0, 0, 0,   0,  0,   0,  0,   0,   0, 0,  0, 0,  0, 0,  0, 0,  0, 0,
     0,   0,  0,  0,  0,   0,  0,   0,  0, 0, 0,   0,  0,   0,  0,   0,   0, 0,  0, 0,  0, 0,  0, 0,  0, 0,
     0,   0,  0,  0,  0,   0,  0,   0,  0, 0, 0,   0,  0,   0,  0,   132, 0, 0,  0, 0,  0, 0,  0, 0,  0, 0,
     0,   0,  35, 0,  0,   0,  0,   0,  0, 0, 0,   0,  20,  0,  0,   0,   0, 90, 0, 87, 8, 84, 5, 81, 2, 78,
     154, 75, 0,  72, 148, 69, 145, 66, 0, 0, 139, 60, 136, 57, 133, 54,  0, 51, 0, 0,  0, 0,  0, 0},
    {0,  0,  0,   0, 0,  0,   0,   0,  0, 0, 0,  0,  0,   0,  0,   0, 0,  0,  0, 0,  0,  0,   0,   0,  0,  0,
     0,  0,  0,   0, 0,  0,   0,   0,  0, 0, 0,  0,  0,   0,  0,   0, 0,  0,  0, 0,  0,  0,   0,   0,  0,  0,
     0,  0,  0,   0, 0,  0,   0,   0,  0, 0, 0,  0,  0,   45, 153, 0, 59, 0,  0, 73, 0,  0,   0,   40, 0,  0,
     0,  0,  115, 0, 21, 129, 0,   0,  0, 0, 0,  0,  0,   0,  0,   0, 0,  30, 0, 91, 44, 152, 105, 58, 11, 119,
     72, 25, 0,   0, 39, 147, 100, 53, 6, 0, 67, 20, 128, 81, 0,   0, 0,  48, 0, 0,  0,  0,   0,   0}};
const uint8_t PRODUCTS_LT2[PRODUCTS_KEY_MIN_LEN][128] = {
    {0,  0,   0,  0, 0,  0,  0,  0, 0, 0,  0, 0,  0,   0, 0,  0,   0,  0,   0,  0,   0,  0, 0,  0,   0,  0,
     0,  0,   0,  0, 0,  0,  0,  0, 0, 0,  0, 0,  0,   0, 0,  0,   0,  0,   0,  0,   0,  0, 0,  0,   0,  0,
     0,  0,   0,  0, 0,  0,  0,  0, 0, 0,  0, 0,  0,   5, 79, 153, 72, 146, 65, 139, 58, 0, 51, 125, 44, 118,
     37, 111, 30, 0, 23, 97, 16, 0, 0, 83, 0, 76, 150, 0, 0,  0,   0,  0,   0,  0,   0,  0, 0,  0,   0,  0,
     0,  0,   0,  0, 0,  0,  0,  0, 0, 0,  0, 0,  0,   0, 0,  0,   0,  0,   0,  0,   0,  0, 0,  0},
    {0,   0,  0, 0,  0,   0, 0,   0,  0,   0,  0, 0,  0,  0,   0,  0,   0, 0, 0, 0,   0, 0, 0, 0,  0, 0,
     0,   0,  0, 0,  0,   0, 0,   0,  0,   0,  0, 0,  0,  0,   0,  0,   0, 0, 0, 0,   0, 0, 0, 0,  0, 0,
     0,   0,  0, 0,  0,   0, 0,   0,  0,   0,  0, 0,  0,  0,   0,  0,   0, 0, 0, 0,   0, 0, 0, 0,  0, 0,
     0,   0,  0, 0,  0,   0, 0,   0,  0,   0,  0, 0,  0,  0,   0,  0,   0, 0, 0, 146, 0, 2, 0, 13, 0, 24,
     107, 35, 0, 46, 129, 0, 140, 68, 151, 79, 7, 90, 18, 101, 29, 112, 0, 0, 0, 0,   0, 0, 0, 0},
    {0, 0,  0,  0,   0,  0,   0,  0,   0,  0, 0,  0,  0,  0,  0, 0,  0, 0,  0, 0,  0,   0,  0,   0,  0, 0,
     0, 0,  0,  0,   0,  0,   0,  0,   0,  0, 0,  0,  0,  0,  0, 0,  0, 0,  0, 0,  0,   0,  0,   0,  0, 0,
     0, 0,  0,  0,   0,  0,   0,  0,   0,  0, 0,  0,  0,  0,  0, 0,  0, 0,  0, 0,  0,   0,  0,   0,  0, 0,
     0, 0,  0,  0,   0,  0,   0,  0,   0,  0, 0,  0,  0,  0,  0, 0,  0, 0,  0, 30, 115, 45, 130, 60, 0, 75,
     5, 90, 20, 105, 35, 120, 50, 135, 65, 0, 80, 10, 95, 25, 0, 40, 0, 55, 0, 0,  0,   0,  0,   0},
    {0, 0,  0, 0,   0,  0,   0,  0,   0,  0, 0, 0,  0,   0,  0,   0, 0, 0,   0, 0,  0,   0,  0, 0,  0, 0,
     0, 0,  0, 0,   0,  0,   0,  0,   0,  0, 0, 0,  0,   0,  0,   0, 0, 0,   0, 0,  0,   0,  0, 0,  0, 0,
     0, 0,  0, 0,   0,  0,   0,  0,   0,  0, 0, 0,  0,   0,  0,   0, 0, 0,   0, 0,  0,   0,  0, 0,  0, 0,
     0, 0,  0, 0,   0,  0,   0,  0,   0,  0, 0, 0,  0,   0,  0,   0, 0, 105, 0, 86, 154, 67, 0, 48, 0, 29,
     0, 10, 0, 146, 59, 127, 40, 108, 21, 0, 2, 70, 138, 51, 119, 0, 0, 0,   0, 0,  0,   0,  0, 0},
    {0,  0,   0,  0,  0,   0,  0,  0,   0, 0, 0,   0,  0,   0, 0,  0,   0, 0,  0, 0,  0,  0,   0,  0,  0,   0,
     0,  0,   0,  0,  0,   0,  0,  0,   0, 0, 0,   0,  0,   0, 0,  0,   0, 0,  0, 0,  0,  0,   0,  0,  0,   0,
     0,  0,   0,  0,  0,   0,  0,  0,   0, 0, 0,   0,  0,   0, 0,  141, 0, 0,  0, 0,  0,  0,   0,  0,  0,   0,
     0,  0,   55, 0,  0,   0,  0,  0,   0, 0, 0,   0,  120, 0, 0,  0,   0, 75, 0, 26, 79, 132, 30, 83, 136, 34,
     87, 140, 0,  91, 144, 42, 95, 148, 0, 0, 152, 50, 103, 1, 54, 107, 0, 58, 0, 0,  0,  0,   0,  0},
    {0,  0,   0,  0, 0,  0,   0,  0,  0,   0, 0,   0,  0,   0,  0,   0, 0,   0,  0, 0,  0,  0,   0,  0,   0,  0,
     0,  0,   0,  0, 0,  0,   0,  0,  0,   0, 0,   0,  0,   0,  0,   0, 0,   0,  0, 0,  0,  0,   0,  0,   0,  0,
     0,  0,   0,  0, 0,  0,   0,  0,  0,   0, 0,   0,  0,   35, 119, 0, 132, 0,  0, 74, 0,  0,   0,  100, 0,  0,
     0,  0,   55, 0, 68, 152, 0,  0,  0,   0, 0,   0,  0,   0,  0,   0, 0,   75, 0, 88, 17, 101, 30, 114, 43, 127,
     56, 140, 0,  0, 82, 11,  95, 24, 108, 0, 121, 50, 134, 63, 0,   0, 0,   89, 0, 0,  0,  0,   0,  0}};

const size_t ANS_SIZE = ((sizeof(uint64_t) * CITIES_NUM) + (sizeof(uint16_t) * CITIES_NUM * PRODUCTS_NUM));

inline __attribute__((always_inline)) uint8_t cities_hash_f(const char *key) {
    uint32_t sum1 = 0, sum2 = 0;
    for (uint8_t i = 0; i < CITIES_KEY_MIN_LEN && key[i] && key[i] != ','; ++i) {
        if (key[i] & ~0x7F) {  // isascii
            sum1 += CITIES_LT1[i]['?'];
            sum2 += CITIES_LT2[i]['?'];
        } else {
            sum1 += CITIES_LT1[i][key[i]];
            sum2 += CITIES_LT2[i][key[i]];
        }
    }
    return (CITIES_G[sum1 % CITIES_MAGIC_NUM] + CITIES_G[sum2 % CITIES_MAGIC_NUM]) % CITIES_MAGIC_NUM;
}

inline __attribute__((always_inline)) uint8_t products_hash_f(const char *key) {
    uint32_t sum1 = 0, sum2 = 0;
    for (uint8_t i = 0; i < PRODUCTS_KEY_MIN_LEN && key[i] && key[i] != ','; ++i) {
        sum1 += PRODUCTS_LT1[i][key[i]];
        sum2 += PRODUCTS_LT2[i][key[i]];
    }
    return (PRODUCTS_G[sum1 % PRODUCTS_MAGIC_NUM] + PRODUCTS_G[sum2 % PRODUCTS_MAGIC_NUM]) % PRODUCTS_MAGIC_NUM;
}

void process_blk(char *fbegin, int fd, int64_t offset, int64_t size, uint8_t proc, char *ans) {
    posix_fadvise64(fd, offset, size, POSIX_FADV_SEQUENTIAL);
    char *begin = fbegin + offset;
    char *end = begin + size;
    if (proc && *(begin - 1) != '\n')
        while (*(begin++) != '\n') {
        }

    uint64_t prices_sum_per_city[CITIES_NUM] = {0};
    uint16_t prices[CITIES_NUM][PRODUCTS_NUM] = {0};

    // 3. Rush through each line until the end of the blk
    while (begin < end) {
        uint8_t city_idx = cities_hash_f(begin++);
        while (*(begin++) != ',') {
        }

        uint8_t product_idx = products_hash_f(begin++);
        while (*(begin++) != ',') {
        }

        uint16_t price = 0;
        for (; *begin != '.'; ++begin) price = price * 10 + *begin - '0';
        price = price * 100 + (*(++begin) - '0') * 10;
        if (isdigit(begin[1])) price += *(++begin) - '0';
        begin += 2;  // first fractional + NL

        prices_sum_per_city[city_idx] += price;
        if (price < prices[city_idx][product_idx] || !prices[city_idx][product_idx])
            prices[city_idx][product_idx] = price;
    }

    memcpy(ans, prices_sum_per_city, sizeof(uint64_t) * CITIES_NUM);
    ans += sizeof(uint64_t) * CITIES_NUM;
    for (uint8_t cid = 0; cid < CITIES_NUM; ++cid) {
        memcpy(ans, prices + cid, sizeof(uint16_t) * PRODUCTS_NUM);
        ans += sizeof(uint16_t) * PRODUCTS_NUM;
    }
}

int main(void) {
    // 1. At first, map input file to memory
    int fd = open(IN_FILENAME, O_RDONLY);
    struct stat sb;
    if (fstat(fd, &sb) == -1) exit(EXIT_FAILURE);
    char *fbegin = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    char *ans = (char *)mmap(NULL, ANS_SIZE * MAX_PROCESSES, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // 2. BEfore parsing, divide workloads among processes
    pid_t pids[MAX_PROCESSES];
    const uint64_t BLK_SIZE = MAX(4096L, sb.st_size / MAX_PROCESSES);
    uint8_t procs_cnt = 0;
    int64_t offset = 0;
    for (uint32_t proc = 0; offset < sb.st_size && proc < MAX_PROCESSES; ++proc, ++procs_cnt) {
        int64_t size = MIN(BLK_SIZE, sb.st_size - offset);
        pids[proc] = fork();
        if (!pids[proc]) {
            process_blk(fbegin, fd, offset, size, proc, ans + (ANS_SIZE * proc));
            exit(EXIT_SUCCESS);
        } else if (pids[proc] < 0)
            exit(EXIT_FAILURE);
        offset += size;
    }

    // 4. Done assigning workloads, waiting for processes to finish
    for (uint8_t proc = 0; proc < procs_cnt; ++proc) {
        wait(NULL);
    }

    // 5. Aggregate processes results
    uint64_t prices_sum_per_city[CITIES_NUM] = {0};
    uint16_t prices[CITIES_NUM][PRODUCTS_NUM] = {0};
    for (uint8_t proc = 0; proc < procs_cnt; ++proc) {
        uint64_t *prices_sum_per_city_ptr = (uint64_t *)ans;
        for (uint8_t cid = 0; cid < CITIES_NUM; ++cid) prices_sum_per_city[cid] += *(prices_sum_per_city_ptr++);
        uint16_t *prices_ptr = (uint16_t *)prices_sum_per_city_ptr;
        for (uint8_t cid = 0; cid < CITIES_NUM; ++cid) {
            for (uint8_t pid = 0; pid < PRODUCTS_NUM; ++pid) {
                if ((*prices_ptr < prices[cid][pid] && *prices_ptr) || !prices[cid][pid])
                    prices[cid][pid] = *prices_ptr;
                ++prices_ptr;
            }
        }
        ans = (char *)prices_ptr;
    }

    uint8_t cheapest_city_idx = 0;
    for (uint8_t cid = 1; cid < CITIES_NUM; ++cid) {
        if ((prices_sum_per_city[cid] && prices_sum_per_city[cid] < prices_sum_per_city[cheapest_city_idx]) ||
            !prices_sum_per_city[cheapest_city_idx]) {
            cheapest_city_idx = cid;
        }
    }

    // 6. Input processed, generate output
    FILE *fout = fopen(OUT_FILENAME, "w");
    fprintf(fout, "%s %0.2f\n", CITIES_K[cheapest_city_idx], prices_sum_per_city[cheapest_city_idx] / 100.0);

    uint8_t products_cnt = 0;
    for (uint16_t price = 100; products_cnt < 5 && price < 10001; ++price) {
        for (uint8_t pid = 0; products_cnt < 5 && pid < PRODUCTS_NUM; ++pid) {
            if (prices[cheapest_city_idx][PRODUCTS_KEY_ORDER[pid]] == price) {
                fprintf(fout, "%s %.2f\n", PRODUCTS_K[PRODUCTS_KEY_ORDER[pid]], price / 100.0);
                ++products_cnt;
            }
        }
    }

    exit(EXIT_SUCCESS);
}
