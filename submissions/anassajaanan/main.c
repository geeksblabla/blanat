#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>



#define MAX_CITIES 101
#define MAX_PRODUCTS 100

#define MAX_CITY_LENGTH 18 // 17 characters + null terminator
#define MAX_PRODUCT_LENGTH 17 // 16 characters + null terminator
#define MAX_LINE_LENGTH 44


struct ProductEntry {
	const char	*name;
	int			productIndex;
};

struct CityEntry {
    const char *name;
    int			cityIndex;
};

struct Product {
	uint32_t		price;
	char			*name;
};

struct City {
	uint64_t		total;
	char		    *name;
	int				numProducts;
	struct Product	products[MAX_PRODUCTS];
};

struct City cities[MAX_CITIES] = {0};

#define MAX_TASK_UNITS 16

// #========================= Product Hash Table =========================#
#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "This file is for ISO-646 based systems only."
#endif




#define TOTAL_PRODUCT_KEYWORDS 100
#define MIN_PRODUCT_LENGTH 3
#define MIN_PRODUCT_HASH_VALUE 6
#define MAX_PRODUCT_HASH_VALUE 139
/* maximum key range = 134, duplicates = 6 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif

static unsigned int	hash_product( const char *str,  size_t len)
{
  	static unsigned char asso_values_product[] =
    {
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140,  40,  40,   0,  75,   0,
        0,  15,  20, 140,  55,  50,  20,  30,  35,  95,
        0, 140,  10,  25,  65, 140, 140,  20, 140,   5,
        5, 140, 140, 140, 140, 140, 140,   0,   0,  65,
       60,   0, 140,  65,  40,   5,  10,   0,  30,  50,
        5,   0,  30, 140,   5,  20,   0,   0, 140,  55,
      140,  20, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
      140, 140, 140, 140, 140, 140
    };
  	return len + asso_values_product[(unsigned char)str[2]] + asso_values_product[(unsigned char)str[0]] + asso_values_product[(unsigned char)str[len - 1]];
}

inline static struct ProductEntry *get_product( const char *str,  size_t len)
{
  	static struct ProductEntry wordlist_product[] =
    {

      {"Potato", 52},

      {"Cabbage", 40},

      {"Pear", 12},

      {"Clementine", 62},

      {"Carrot", 31},

      {"Currant", 60},

      {"Plantain", 90},

      {"Pineapple", 6},

      {"Cantaloupe", 16},

      {"Cantaloupe", 76},

      {"Cauliflower", 41},

      {"Rhubarb", 64},

      {"Rutabaga", 96},

      {"Persimmon", 82},

      {"Guava", 25},

      {"Peas", 48},

      {"Grapefruit", 22},

      {"Cherry", 11},

      {"Lettuce", 35},

      {"Cranberry", 58},

      {"Cranberry", 63},

      {"Ginger", 83},

      {"Parsley", 67},

      {"Passion_Fruit", 27},

      {"Starfruit", 81},

      {"Watermelon", 5},

      {"Papaya", 24},

      {"Sweet_Potato", 53},

      {"Cilantro", 68},

      {"Mint", 69},

      {"Mango", 7},

      {"Grapes", 4},

      {"Parsnip", 95},

      {"Beet", 44},

      {"Peach", 9},

      {"Green_Beans", 47},

      {"Avocado", 23},

      {"Artichoke", 46},

      {"Artichoke", 89},

      {"Watercress", 87},

      {"Banana", 1},

      {"Apricot", 28},

      {"Broccoli", 32},

      {"Plum", 10},

      {"Goji_Berry", 59},

      {"Celery", 49},

      {"Rosemary", 72},

      {"Raspberry", 14},

      {"Strawberry", 3},

      {"Pomegranate", 19},

      {"Pomegranate", 79},

      {"Pumpkin", 57},

      {"Collard_Greens", 66},

      {"Chard", 65},

      {"Endive", 99},

      {"Bok_Choy", 98},

      {"Blueberry", 13},

      {"Blackberry", 15},

      {"Coconut", 18},

      {"Coconut", 78},

      {"Eggplant", 39},

      {"Lime", 21},

      {"Apple", 0},

      {"Brussels_Sprouts", 42},

      {"Spinach", 33},

      {"Cucumber", 30},

      {"Date", 61},

      {"Lemon", 20},

      {"Cactus_Pear", 91},

      {"Salsify", 97},

      {"Zucchini", 38},

      {"Kale", 34},

      {"Bell_Pepper", 37},

      {"Dragon_Fruit", 94},

      {"Honeydew", 17},

      {"Honeydew", 77},

      {"Squash_Blossom", 93},

      {"Thyme", 71},

      {"Garlic", 51},

      {"Acorn_Squash", 56},

      {"Sage", 73},

      {"Basil", 70},

      {"Butternut_Squash", 55},

      {"Asparagus", 45},

      {"Orange", 2},

      {"Oregano", 75},

      {"Kohlrabi", 86},

      {"Okra", 88},

      {"Turnip", 84},

      {"Yam", 54},

      {"Nectarine", 29},

      {"Onion", 50},

      {"Kiwano", 92},

      {"Kiwi", 8},

      {"Radish", 43},

      {"Tomato", 36},

      {"Jicama", 85},

      {"Jackfruit", 80},

      {"Fig", 26},

      {"Dill", 74}
    };

  	static short lookup[] =
    {
        -1,   -1,   -1,   -1,   -1,   -1,    0,    1,
        -1,    2,    3,    4,    5,    6,    7, -122,
        10,   11,   12,   13,   14,  -92,   -2,   -1,
        15,   16,   17,   18,   -1, -223,   -1,   21,
        22,   23,   24,   25,   26,   27,   28,   29,
        30,   31,   32,   -1,   33,   34,   35,   36,
        -1, -220,   39,   40,   41,   42,   43,   44,
        45,   -1,   46,   47,   48, -218,   51,   -1,
        52,   53,   54,   -1,   55,   56,   57,   -1,
      -213,   60,   61,   62,   63,   64,   65,   66,
        67,   68,   69,   70,   71,   -1,   72,   73,
      -198,   76,   77,   78,   79,   -1,   80,   81,
        82,  -26,   -2,   83,   -1,   84,   85,   86,
        87,   -1,   88,   -1,   89,   90,   91,   92,
       -42,   -2,   93,   -1,   94,  -51,   -2,  -63,
        -2,   95,  -81,   -2,   -1,   -1,   96,   -1,
        -1,   97,   -1,   -1,   -1,   98,   -1,   -1,
        -1,   -1,   -1,   99
    };

  	if (len <= MAX_PRODUCT_LENGTH && len >= MIN_PRODUCT_LENGTH)
    {
      	 unsigned int key = hash_product(str, len);

      	if (key <= MAX_PRODUCT_HASH_VALUE)
        {
          	 int index = lookup[key];

          	if (index >= 0)
            {
				 const char *s = wordlist_product[index].name;

				// if (*str == *s && !strcmp (str + 1, s + 1))
				// 	return &wordlist[index];
				if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
					return &wordlist_product[index];
            }
          	else if (index < -TOTAL_PRODUCT_KEYWORDS)
            {
               int offset = - 1 - TOTAL_PRODUCT_KEYWORDS - index;
               struct ProductEntry *wordptr = &wordlist_product[TOTAL_PRODUCT_KEYWORDS + lookup[offset]];
               struct ProductEntry *wordendptr = wordptr + -lookup[offset + 1];

              while (wordptr < wordendptr)
                {
					 const char *s = wordptr->name;

					// if (*str == *s && !strcmp (str + 1, s + 1))
					// 	return wordptr;
					if (*str == *s  && !strncmp (str + 1, s + 1, len - 1))
						return wordptr;
					wordptr++;
                }
            }
        }
    }
  return 0;
}
// #========================= End of Product Hash Table =========================#

// #========================= City Hash Table =========================#

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
	  && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
	  && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
	  && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
	  && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
	  && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
	  && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
	  && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
	  && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
	  && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
	  && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
	  && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
	  && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
	  && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
	  && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
	  && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
	  && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
	  && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
	  && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
	  && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
	  && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
	  && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
	  && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "This file is for ISO-646 based systems only."
#endif


#define TOTAL_CITY_KEYWORDS 101
#define MIN_CITY_LENGTH 3
#define MIN_CITY_HASH_VALUE 6
#define MAX_CITY_HASH_VALUE 306
/* maximum key range = 301, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int	hash_city( const char *str,  size_t len)
{
	static unsigned short asso_values_cities[] =
	{
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 105,  50, 307,  25,
	  307, 307, 307,  65, 307,  35,   5,   0,   5, 307,
	  307, 307, 307,  25, 307,  25, 307, 307, 307, 307,
	  307, 307, 307, 307, 307,  65,   0,   0,  40,  95,
	   30,   5,  25, 120,   0,   5,  55,  70,  60,  30,
	   10,  15,   5,  10,   0,  20,  30,   0,  60, 307,
	  307,  10,  20,   0, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307, 307, 307,   0,
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307,  15, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
	  307, 307, 307, 307, 307, 307, 307
	};
   unsigned int hval = len;

  switch (hval)
	{
	  default:
		hval += asso_values_cities[(unsigned char)str[5]];
	  /*FALLTHROUGH*/
	  case 5:
		hval += asso_values_cities[(unsigned char)str[4]+1];
	  /*FALLTHROUGH*/
	  case 4:
	  case 3:
		hval += asso_values_cities[(unsigned char)str[2]];
	  /*FALLTHROUGH*/
	  case 2:
		hval += asso_values_cities[(unsigned char)str[1]];
		break;
	}
  return hval;
}

inline static struct CityEntry *get_city( const char *str,  size_t len)
{
	
  	static struct CityEntry wordlist_cities[] =
	{
	  {""}, {""}, {""}, {""}, {""}, {""},

	  {"Drarga", 99},
	  {""},

	  {"Ouazzane", 84},
	  {""},

	  {"Ouarzazate", 62},
	  {""}, {""},

	  {"Laayoune", 19},
	  {""},

	  {"Guerguerat", 92},

	  {"Jerada", 55},
	  {""},

	  {"Guelta_Zemmur", 35},

	  {"Chichaoua", 30},

	  {"Bir_Lehlou", 22},
	  {""},

	  {"Kenitra", 8},

	  {"Inezgane", 63},

	  {"Taza", 14},

	  {"Ben_guerir", 61},

	  {"Beni_Mellal", 12},

	  {"Guelmim", 17},

	  {"Fes", 3},

	  {"Safi", 10},
	  {""},

	  {"Souk_Larbaa", 45},

	  {"Tinghir", 39},
	  {""}, {""}, {""}, {""},

	  {"Larache", 48},

	  {"Goulmima", 51},

	  {"Ksar_es_Seghir", 57},

	  {"Errachidia", 13},

	  {"Sefrou", 65},

	  {"Tan-Tan", 49},

	  {"Boujdour", 82},

	  {"Assa", 80},

	  {"Rabat", 1},

	  {"Chefchaouen", 31},

	  {"Guercif", 71},

	  {"Taourirt", 33},

	  {"Khouribga", 16},

	  {"Ahfir", 74},

	  {"Ifrane", 40},

	  {"Zemamra", 97},

	  {"Jorf_El_Melha", 18},

	  {"Essaouira", 15},

	  {"Nador", 28},

	  {"Ait_Melloul", 59},

	  {"Tarfaya", 83},

	  {"Ksar_El_Kebir", 20},

	  {"La\303\242youne", 37},

	  {"Midar", 89},

	  {"Temara", 24},

	  {"Berkane", 75},
	  {""},

	  {"Sale", 21},

	  {"Mohammedia", 25},

	  {"Saidia", 87},

	  {"Bouarfa", 72},
	  {""}, {""},

	  {"Youssoufia", 56},

	  {"Asilah", 93},

	  {"Sidi_Slimane", 44},

	  {"Bab_Taza", 42},
	  {""},

	  {"Smara", 81},

	  {"Aourir", 66},

	  {"Tangier", 4},
	  {""},

	  {"Boulemane", 77},

	  {"Sidi_Kacem", 98},
	  {""},

	  {"Moulay_Bousselham", 90},
	  {""}, {""},

	  {"Azrou", 41},

	  {"had_soualem", 86},

	  {"Layoune", 60},

	  {"Tafraout", 95},

	  {"Khemisset", 91},

	  {"Bni_Hadifa", 69},

	  {"Tiflet", 46},
	  {""},

	  {"Khenifra", 78},
	  {""},

	  {"Fquih_Ben_Salah", 70},
	  {""}, {""},

	  {"Skhirate", 100},

	  {"Taroudant", 34},

	  {"Oujda", 7},

	  {"Midelt", 52},

	  {"Tetouan", 9},
	  {""}, {""},

	  {"Bab_Berred", 88},

	  {"Dakhla", 36},
	  {""}, {""},

	  {"Sidi_Ifni", 50},
	  {""},

	  {"Settat", 26},

	  {"Demnate", 73},

	  {"Akhfenir", 76},
	  {""}, {""},

	  {"Tiznit", 38},
	  {""},
	  {"Imzouren", 96},
	  {"Marrakech", 2},
	  {""},
	  {"Arfoud", 23},
	  {""}, {""}, {""}, {""},
	  {"Meknes", 6},
	  {""}, {""}, {""}, {""},
	  {"Azilal", 54},
	  {"Bir_Anzerane", 79},
	  {""},
	  {"Berrechid", 43},
	  {""},
	  {"Tichla", 68},
	  {""}, {""}, {""}, {""},
	  {"Oulad_Teima", 67},
	  {""},
	  {"Kalaat_MGouna", 29},
	  {""}, {""},
	  {"Zagora", 85},
	  {"B\303\251ni_Mellal", 27},
	  {""}, {""}, {""},
	  {"Sidi_Bouzid", 94},
	  {"Sidi_Bennour", 47},
	  {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
	  {""}, {""}, {""}, {""},
	  {"Tichka", 58},
	  {""}, {""}, {""}, {""},
	  {"Oujda_Angad", 64},
	  {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
	  {"Agadir", 5},
	  {""}, {""}, {""},
	  {"Casablanca", 0},
	  {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
	  {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
	  {"El_Jadida", 11},
	  {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
	  {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
	  {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
	  {""}, {""}, {""},
	  {"Al_Hoceima", 32},
	  {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
	  {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
	  {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
	  {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
	  {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
	  {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
	  {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
	  {""}, {""}, {""}, {""}, {""}, {""}, {""},
	  {"Figuig", 53}
	};

	

	if (len <= MAX_CITY_LENGTH && len >= MIN_CITY_LENGTH)
	{
		 unsigned int key = hash_city(str, len);

		if (key <= MAX_CITY_HASH_VALUE)
		{
			 const char *s = wordlist_cities[key].name;

			// if (*str == *s && !strcmp (str + 1, s + 1))
			// 	return &wordlist[key];
			if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
				return &wordlist_cities[key];
			// if (*str == *s && !compare_strings_with_ptest(str + 1, s + 1, len - 1))
			// 	return &wordlist_cities[key];
		}
	}
  return 0;
}
// #========================= End of City Hash Table =========================#

// #========================= Utility Functions =========================#
inline static int compareProducts(const void *a, const void *b)
{
	const struct Product *productA = (const struct Product *)a;
	const struct Product *productB = (const struct Product *)b;

	if (productA->name == NULL) return (productB->name) ? 1 : 0;
	if (productB->name == NULL) return -1;

	if (productA->price < productB->price) return -1;
	if (productA->price > productB->price) return 1;

	return strcmp(productA->name, productB->name);
}

inline static uint32_t parse_price_component(char *component_str, uint32_t component_len)
{
  static uint32_t ascii_zero_offset[4] = {0, '0', 11 * '0', 111 * '0'};
  
  static uint32_t digit_multipliers[3][4] = {
    {0, 1, 10, 100},   // Multipliers for the first digit
    {0, 0, 1,  10},    // Multipliers for the second digit
    {0, 0, 0,  1},     // Multipliers for the third digit
  };
  
  // Calculate and return the integer value of the price component
  uint32_t result = (
    digit_multipliers[0][component_len] * component_str[0] + 
    digit_multipliers[1][component_len] * component_str[1] + 
    digit_multipliers[2][component_len] * component_str[2] -
    ascii_zero_offset[component_len]
  );
  return result;
}
// #========================= End of Utility Functions =========================#

// #========================= Serialization Functions =========================#

inline static void	serialize_city(FILE *file, const struct City *city)
{
	size_t cityNameLength = strlen(city->name);
	fwrite(&cityNameLength, sizeof(size_t), 1, file);
	fwrite(city->name, sizeof(char), cityNameLength, file);
	fwrite(&city->total, sizeof(uint64_t), 1, file);

	fwrite(&city->numProducts, sizeof(int), 1, file);

	for (int i = 0; i < MAX_PRODUCTS; i++)
	{
		if (city->products[i].name != NULL)
		{
			size_t productNameLength = strlen(city->products[i].name);
			fwrite(&productNameLength, sizeof(size_t), 1, file);
			fwrite(city->products[i].name, sizeof(char), productNameLength, file);
			fwrite(&city->products[i].price, sizeof(uint32_t), 1, file);
		}
	}
}

inline static void performDataSerialization()
{
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "results_%d.bin", getpid());

    FILE* file = fopen(filePath, "wb");
    if (file) {
        for (int i = 0; i < MAX_CITIES; i++) {
			if (cities[i].name != NULL) {
				serialize_city(file, &cities[i]);
			}
        }
        fclose(file);
    }
}
// #========================= End of Serialization Functions =========================#

inline static void update_city_and_product_totals(struct City *city, const char *cityName, size_t cityNameLength, const char *productName, size_t productNameLength, uint32_t price)
{
	const struct ProductEntry *productEntry = get_product(productName, productNameLength);
	if (!productEntry)
	{
		perror("Product not found");
		exit(EXIT_FAILURE);
	}

	struct Product *product = &(city->products[productEntry->productIndex]);
	if (product->name == NULL)
	{
		product->name = (char *)productEntry->name;
		product->price = price;
		city->numProducts++;
	}
	else if (product->price > price)
	{
		product->price = price;
	}
	city->total += price;
}



inline static void process_segment(const char *segment, size_t length)
{
    const char *end = segment + length;
    const char *line_start = segment;

    while (line_start < end) {
        const char *line_end = (const char *)memchr(line_start, '\n', end - line_start);
        if (!line_end) {
            line_end = end;
        }

        const char *first_comma = (const char *)memchr(line_start, ',', line_end - line_start);
        const char *second_comma = first_comma ? (const char *)memchr(first_comma + 1, ',', line_end - (first_comma + 1)) : NULL;

        if (first_comma && second_comma) {
            // Extract cityName
            char cityName[MAX_CITY_LENGTH] = {0};
            size_t cityNameLength = first_comma - line_start;
            memcpy(cityName, line_start, cityNameLength);
            cityName[cityNameLength] = '\0';

            // Extract productName
            char productName[MAX_PRODUCT_LENGTH] = {0};
            size_t productNameLength = second_comma - first_comma - 1;
            memcpy(productName, first_comma + 1, productNameLength);
            productName[productNameLength] = '\0';

            // Extract price
			char* price_d = (char *)second_comma + 1;
   			size_t price_d_len = 0;
			while (price_d[price_d_len] != '.' && &price_d[price_d_len] < line_end)
				price_d_len++;
			uint32_t price_decimal = parse_price_component(price_d, price_d_len);

			char *price_f = price_d + price_d_len + 1;
			size_t price_f_len = line_end - price_f;
			uint32_t price_fractional = parse_price_component(price_f, price_f_len);
			price_fractional *= (price_f_len < 2) ? 10 : 1;

			uint32_t price = price_decimal * 100 + price_fractional;
				
			const struct CityEntry *cityEntry = get_city(cityName, cityNameLength);
			if (!cityEntry)
			{
				printf("City not found: %s\n", cityName);
				exit(EXIT_FAILURE);
			}

			struct City *city = &cities[cityEntry->cityIndex];
			if (city->name == NULL)
			{
				city->name = (char *)cityEntry->name;
			}
			
			// Process the line
			update_city_and_product_totals(city, cityName, cityNameLength, productName, productNameLength, price);
        }
        line_start = line_end + 1;
    }
	performDataSerialization();
}


inline static int deserialize_city(FILE* file, struct City *city)
{
	size_t cityNameLength;
	if (fread(&cityNameLength, sizeof(size_t), 1, file) < 1) return -1;
	city->name = (char *)malloc(cityNameLength + 1);
	fread(city->name, sizeof(char), cityNameLength, file);
	city->name[cityNameLength] = '\0';
	fread(&city->total, sizeof(uint64_t), 1, file);
	fread(&city->numProducts, sizeof(int), 1, file);

	for (int i = 0; i < city->numProducts; i++)
	{
		uint32_t price;
		size_t productNameLength;
		char productName[MAX_PRODUCT_LENGTH] = {0};

		fread(&productNameLength, sizeof(size_t), 1, file);
		fread(productName, sizeof(char), productNameLength, file);
		productName[productNameLength] = '\0';
		fread(&price, sizeof(uint32_t), 1, file);

		const struct ProductEntry *productEntry = get_product(productName, productNameLength);
		if (!productEntry)
		{
			perror("Product not found");
			exit(EXIT_FAILURE);
		}
		city->products[productEntry->productIndex].name = (char *)productEntry->name;
		city->products[productEntry->productIndex].price = price;
	}
	return (0);
}

inline static void aggregate_city(struct City aggregatedCities[MAX_CITIES], struct City* newCity)
{
	const struct CityEntry *cityEntry = get_city(newCity->name, strlen(newCity->name));
	if (!cityEntry)
	{
		perror("City not found");
		exit(EXIT_FAILURE);
	}

	if (aggregatedCities[cityEntry->cityIndex].name == NULL)
	{
		aggregatedCities[cityEntry->cityIndex].name = newCity->name;
		aggregatedCities[cityEntry->cityIndex].total = newCity->total;
		aggregatedCities[cityEntry->cityIndex].numProducts = newCity->numProducts;
	}
	else
		aggregatedCities[cityEntry->cityIndex].total += newCity->total;

	for (int i = 0; i < MAX_PRODUCTS; i++)
	{
		if (newCity->products[i].name)
		{
			if (aggregatedCities[cityEntry->cityIndex].products[i].name == NULL)
			{
				aggregatedCities[cityEntry->cityIndex].products[i].name = newCity->products[i].name;
				aggregatedCities[cityEntry->cityIndex].products[i].price = newCity->products[i].price;
			}
			else if (aggregatedCities[cityEntry->cityIndex].products[i].price > newCity->products[i].price)
			{
				aggregatedCities[cityEntry->cityIndex].products[i].price = newCity->products[i].price;
			}
		}
	}
}


inline static void	compileAggregatedResults(pid_t pids[MAX_TASK_UNITS])
{
	char filePaths[MAX_TASK_UNITS][256];
	struct City aggregatedCities[MAX_CITIES] = {0};

	for (int i = 0; i < MAX_TASK_UNITS; i++)
	{
		snprintf(filePaths[i], sizeof(filePaths[i]), "results_%d.bin", pids[i]);
		FILE *file = fopen(filePaths[i], "rb");
		if (file)
		{
			while (!feof(file))
			{
				struct City newCity = {0};
				if (deserialize_city(file, &newCity) == -1) break;

				aggregate_city(aggregatedCities, &newCity);
			}
		}
		fclose(file);
	}

	uint64_t minTotal = UINT64_MAX;
	int minCityIndex = -1; // -1 indicates no city found yet
	for (int i = 0; i < MAX_CITIES; ++i) 
	{
		if (aggregatedCities[i].name && aggregatedCities[i].total < minTotal) {
			minTotal = aggregatedCities[i].total;
			minCityIndex = i;
		}
	}

	if (minCityIndex != -1) {
		qsort(aggregatedCities[minCityIndex].products, MAX_PRODUCTS, sizeof(struct Product), compareProducts);
		FILE *outputFile = fopen("output.txt", "w");
		if (outputFile)
		{
			fprintf(outputFile, "%s %lu.%02lu\n", aggregatedCities[minCityIndex].name, minTotal / 100, minTotal % 100);
			for (int i = 0; i < 5; i++)
			{
				if (aggregatedCities[minCityIndex].products[i].name != NULL)
				{
					uint32_t price = aggregatedCities[minCityIndex].products[i].price;
					fprintf(outputFile, "%s %u.%02u\n", aggregatedCities[minCityIndex].products[i].name, price / 100, price % 100);
				}
			}
			fclose(outputFile);
		}
	}
	else
		printf("No cities processed.\n");

	for (int i = 0; i < MAX_TASK_UNITS; i++)
		remove(filePaths[i]);
}


int main() {

	pid_t pids[MAX_TASK_UNITS];
    int fd = open("input.txt", O_RDONLY);
    struct stat sb;
    if (fd == -1 || fstat(fd, &sb) == -1) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char *file_in_memory = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (file_in_memory == MAP_FAILED) {
        perror("Error mmapping the file");
        return EXIT_FAILURE;
    }

	const off_t initial_segment_size = sb.st_size / MAX_TASK_UNITS;
	off_t current_offset = 0;

	for (int i = 0; i < MAX_TASK_UNITS; i++) {
		off_t offset = current_offset;
		off_t size = (i == MAX_TASK_UNITS - 1) ? (sb.st_size - offset) : initial_segment_size;

		if (i != 0) {
			while (file_in_memory[offset] != '\n' && offset < sb.st_size) {
				offset++;
				size--;
			}
			offset++;
			size--;
		}

		if (i != MAX_TASK_UNITS - 1) {
			off_t end = offset + size;
			while (file_in_memory[end] != '\n' && end < sb.st_size) {
				end++;
			}
			size = end - offset + 1;
		}

		pids[i] = fork();
		if (pids[i] == 0) {
			process_segment(file_in_memory + offset, size);
			munmap(file_in_memory, sb.st_size);
			close(fd);
			exit(EXIT_SUCCESS);
		} else if (pids[i] < 0) {
			perror("Error");
			exit(EXIT_FAILURE);
		}

		current_offset = offset + size;
	}

    for (int i = 0; i < MAX_TASK_UNITS; i++) {
        wait(NULL);
    }

    munmap(file_in_memory, sb.st_size);
    close(fd);

	compileAggregatedResults(pids);

    return EXIT_SUCCESS;
}