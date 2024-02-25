#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct ProductEntry {
	const char *name;
	int productIndex;
};

struct CityEntry {
    const char *name;
    int cityIndex;  // This index can be used to access a City object in a global array.
};

struct Product {
	double 		price;
	const char*	name;
};

struct City {
	double			total;
	const char		*name;
	int				productsCount;
	struct Product	*products[100];
};

struct City *cities[101] = {0};

// #===================================== Product Hash Table =====================================#

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
#error
#endif

#line 1 "products.gperf"
#line 6 "products.gperf"


// #define TOTAL_KEYWORDS 100
#ifndef TOTAL_KEYWORDS
#define TOTAL_KEYWORDS 100
#endif
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 16
#define MIN_HASH_VALUE 6
#define MAX_HASH_VALUE 139
/* maximum key range = 134, duplicates = 6 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif

static unsigned int hash (  const char *str,   size_t len)
{
  static unsigned char asso_values[] =
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
  return len + asso_values[(unsigned char)str[2]] + asso_values[(unsigned char)str[0]] + asso_values[(unsigned char)str[len - 1]];
}


static inline const struct ProductEntry *product_hash(  const char *str,   size_t len)
{
  static struct ProductEntry wordlist[] =
    {
#line 63 "products.gperf"
      {"Potato", 52},
#line 51 "products.gperf"
      {"Cabbage", 40},
#line 23 "products.gperf"
      {"Pear", 12},
#line 73 "products.gperf"
      {"Clementine", 62},
#line 42 "products.gperf"
      {"Carrot", 31},
#line 71 "products.gperf"
      {"Currant", 60},
#line 101 "products.gperf"
      {"Plantain", 90},
#line 17 "products.gperf"
      {"Pineapple", 6},
#line 27 "products.gperf"
      {"Cantaloupe", 16},
#line 87 "products.gperf"
      {"Cantaloupe", 76},
#line 52 "products.gperf"
      {"Cauliflower", 41},
#line 75 "products.gperf"
      {"Rhubarb", 64},
#line 107 "products.gperf"
      {"Rutabaga", 96},
#line 93 "products.gperf"
      {"Persimmon", 82},
#line 36 "products.gperf"
      {"Guava", 25},
#line 59 "products.gperf"
      {"Peas", 48},
#line 33 "products.gperf"
      {"Grapefruit", 22},
#line 22 "products.gperf"
      {"Cherry", 11},
#line 46 "products.gperf"
      {"Lettuce", 35},
#line 69 "products.gperf"
      {"Cranberry", 58},
#line 74 "products.gperf"
      {"Cranberry", 63},
#line 94 "products.gperf"
      {"Ginger", 83},
#line 78 "products.gperf"
      {"Parsley", 67},
#line 38 "products.gperf"
      {"Passion_Fruit", 27},
#line 92 "products.gperf"
      {"Starfruit", 81},
#line 16 "products.gperf"
      {"Watermelon", 5},
#line 35 "products.gperf"
      {"Papaya", 24},
#line 64 "products.gperf"
      {"Sweet_Potato", 53},
#line 79 "products.gperf"
      {"Cilantro", 68},
#line 80 "products.gperf"
      {"Mint", 69},
#line 18 "products.gperf"
      {"Mango", 7},
#line 15 "products.gperf"
      {"Grapes", 4},
#line 106 "products.gperf"
      {"Parsnip", 95},
#line 55 "products.gperf"
      {"Beet", 44},
#line 20 "products.gperf"
      {"Peach", 9},
#line 58 "products.gperf"
      {"Green_Beans", 47},
#line 34 "products.gperf"
      {"Avocado", 23},
#line 57 "products.gperf"
      {"Artichoke", 46},
#line 100 "products.gperf"
      {"Artichoke", 89},
#line 98 "products.gperf"
      {"Watercress", 87},
#line 12 "products.gperf"
      {"Banana", 1},
#line 39 "products.gperf"
      {"Apricot", 28},
#line 43 "products.gperf"
      {"Broccoli", 32},
#line 21 "products.gperf"
      {"Plum", 10},
#line 70 "products.gperf"
      {"Goji_Berry", 59},
#line 60 "products.gperf"
      {"Celery", 49},
#line 83 "products.gperf"
      {"Rosemary", 72},
#line 25 "products.gperf"
      {"Raspberry", 14},
#line 14 "products.gperf"
      {"Strawberry", 3},
#line 30 "products.gperf"
      {"Pomegranate", 19},
#line 90 "products.gperf"
      {"Pomegranate", 79},
#line 68 "products.gperf"
      {"Pumpkin", 57},
#line 77 "products.gperf"
      {"Collard_Greens", 66},
#line 76 "products.gperf"
      {"Chard", 65},
#line 110 "products.gperf"
      {"Endive", 99},
#line 109 "products.gperf"
      {"Bok_Choy", 98},
#line 24 "products.gperf"
      {"Blueberry", 13},
#line 26 "products.gperf"
      {"Blackberry", 15},
#line 29 "products.gperf"
      {"Coconut", 18},
#line 89 "products.gperf"
      {"Coconut", 78},
#line 50 "products.gperf"
      {"Eggplant", 39},
#line 32 "products.gperf"
      {"Lime", 21},
#line 11 "products.gperf"
      {"Apple", 0},
#line 53 "products.gperf"
      {"Brussels_Sprouts", 42},
#line 44 "products.gperf"
      {"Spinach", 33},
#line 41 "products.gperf"
      {"Cucumber", 30},
#line 72 "products.gperf"
      {"Date", 61},
#line 31 "products.gperf"
      {"Lemon", 20},
#line 102 "products.gperf"
      {"Cactus_Pear", 91},
#line 108 "products.gperf"
      {"Salsify", 97},
#line 49 "products.gperf"
      {"Zucchini", 38},
#line 45 "products.gperf"
      {"Kale", 34},
#line 48 "products.gperf"
      {"Bell_Pepper", 37},
#line 105 "products.gperf"
      {"Dragon_Fruit", 94},
#line 28 "products.gperf"
      {"Honeydew", 17},
#line 88 "products.gperf"
      {"Honeydew", 77},
#line 104 "products.gperf"
      {"Squash_Blossom", 93},
#line 82 "products.gperf"
      {"Thyme", 71},
#line 62 "products.gperf"
      {"Garlic", 51},
#line 67 "products.gperf"
      {"Acorn_Squash", 56},
#line 84 "products.gperf"
      {"Sage", 73},
#line 81 "products.gperf"
      {"Basil", 70},
#line 66 "products.gperf"
      {"Butternut_Squash", 55},
#line 56 "products.gperf"
      {"Asparagus", 45},
#line 13 "products.gperf"
      {"Orange", 2},
#line 86 "products.gperf"
      {"Oregano", 75},
#line 97 "products.gperf"
      {"Kohlrabi", 86},
#line 99 "products.gperf"
      {"Okra", 88},
#line 95 "products.gperf"
      {"Turnip", 84},
#line 65 "products.gperf"
      {"Yam", 54},
#line 40 "products.gperf"
      {"Nectarine", 29},
#line 61 "products.gperf"
      {"Onion", 50},
#line 103 "products.gperf"
      {"Kiwano", 92},
#line 19 "products.gperf"
      {"Kiwi", 8},
#line 54 "products.gperf"
      {"Radish", 43},
#line 47 "products.gperf"
      {"Tomato", 36},
#line 96 "products.gperf"
      {"Jicama", 85},
#line 91 "products.gperf"
      {"Jackfruit", 80},
#line 37 "products.gperf"
      {"Fig", 26},
#line 85 "products.gperf"
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

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
        unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
            int index = lookup[key];

          if (index >= 0)
            {
                const char *s = wordlist[index].name;

              if (*str == *s && !strcmp (str + 1, s + 1))
                return &wordlist[index];
            }
          else if (index < -TOTAL_KEYWORDS)
            {
                int offset = - 1 - TOTAL_KEYWORDS - index;
                struct ProductEntry *wordptr = &wordlist[TOTAL_KEYWORDS + lookup[offset]];
                struct ProductEntry *wordendptr = wordptr + -lookup[offset + 1];

              while (wordptr < wordendptr)
                {
                    const char *s = wordptr->name;

                  if (*str == *s && !strcmp (str + 1, s + 1))
                    return wordptr;
                  wordptr++;
                }
            }
        }
    }
  return 0;
}

// #===================================== End of Product Hash =====================================

// #===================================== City Hash Table =====================================#

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
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif


#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH_ 17
#define MIN_HASH_VALUE 6
#define MAX_HASH_VALUE_ 306
/* maximum key range = 301, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int hash_(const char *str, unsigned int len)
{
  static unsigned short asso_values[] =
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
        hval += asso_values[(unsigned char)str[5]];
      /*FALLTHROUGH*/
      case 5:
        hval += asso_values[(unsigned char)str[4]+1];
      /*FALLTHROUGH*/
      case 4:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]];
        break;
    }
  return hval;
}

static inline const struct CityEntry *city_hash(const char *str, unsigned int len)
{
  static struct CityEntry wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""},
#line 110 "cities.gperf"
      {"Drarga", 99},
      {""},
#line 95 "cities.gperf"
      {"Ouazzane", 84},
      {""},
#line 73 "cities.gperf"
      {"Ouarzazate", 62},
      {""}, {""},
#line 30 "cities.gperf"
      {"Laayoune", 19},
      {""},
#line 103 "cities.gperf"
      {"Guerguerat", 92},
#line 66 "cities.gperf"
      {"Jerada", 55},
      {""},
#line 46 "cities.gperf"
      {"Guelta_Zemmur", 35},
#line 41 "cities.gperf"
      {"Chichaoua", 30},
#line 33 "cities.gperf"
      {"Bir_Lehlou", 22},
      {""},
#line 19 "cities.gperf"
      {"Kenitra", 8},
#line 74 "cities.gperf"
      {"Inezgane", 63},
#line 25 "cities.gperf"
      {"Taza", 14},
#line 72 "cities.gperf"
      {"Ben_guerir", 61},
#line 23 "cities.gperf"
      {"Beni_Mellal", 12},
#line 28 "cities.gperf"
      {"Guelmim", 17},
#line 14 "cities.gperf"
      {"Fes", 3},
#line 21 "cities.gperf"
      {"Safi", 10},
      {""},
#line 56 "cities.gperf"
      {"Souk_Larbaa", 45},
#line 50 "cities.gperf"
      {"Tinghir", 39},
      {""}, {""}, {""}, {""},
#line 59 "cities.gperf"
      {"Larache", 48},
#line 62 "cities.gperf"
      {"Goulmima", 51},
#line 68 "cities.gperf"
      {"Ksar_es_Seghir", 57},
#line 24 "cities.gperf"
      {"Errachidia", 13},
#line 76 "cities.gperf"
      {"Sefrou", 65},
#line 60 "cities.gperf"
      {"Tan-Tan", 49},
#line 93 "cities.gperf"
      {"Boujdour", 82},
#line 91 "cities.gperf"
      {"Assa", 80},
#line 12 "cities.gperf"
      {"Rabat", 1},
#line 42 "cities.gperf"
      {"Chefchaouen", 31},
#line 82 "cities.gperf"
      {"Guercif", 71},
#line 44 "cities.gperf"
      {"Taourirt", 33},
#line 27 "cities.gperf"
      {"Khouribga", 16},
#line 85 "cities.gperf"
      {"Ahfir", 74},
#line 51 "cities.gperf"
      {"Ifrane", 40},
#line 108 "cities.gperf"
      {"Zemamra", 97},
#line 29 "cities.gperf"
      {"Jorf_El_Melha", 18},
#line 26 "cities.gperf"
      {"Essaouira", 15},
#line 39 "cities.gperf"
      {"Nador", 28},
#line 70 "cities.gperf"
      {"Ait_Melloul", 59},
#line 94 "cities.gperf"
      {"Tarfaya", 83},
#line 31 "cities.gperf"
      {"Ksar_El_Kebir", 20},
#line 48 "cities.gperf"
      {"La\303\242youne", 37},
#line 100 "cities.gperf"
      {"Midar", 89},
#line 35 "cities.gperf"
      {"Temara", 24},
#line 86 "cities.gperf"
      {"Berkane", 75},
      {""},
#line 32 "cities.gperf"
      {"Sale", 21},
#line 36 "cities.gperf"
      {"Mohammedia", 25},
#line 98 "cities.gperf"
      {"Saidia", 87},
#line 83 "cities.gperf"
      {"Bouarfa", 72},
      {""}, {""},
#line 67 "cities.gperf"
      {"Youssoufia", 56},
#line 104 "cities.gperf"
      {"Asilah", 93},
#line 55 "cities.gperf"
      {"Sidi_Slimane", 44},
#line 53 "cities.gperf"
      {"Bab_Taza", 42},
      {""},
#line 92 "cities.gperf"
      {"Smara", 81},
#line 77 "cities.gperf"
      {"Aourir", 66},
#line 15 "cities.gperf"
      {"Tangier", 4},
      {""},
#line 88 "cities.gperf"
      {"Boulemane", 77},
#line 109 "cities.gperf"
      {"Sidi_Kacem", 98},
      {""},
#line 101 "cities.gperf"
      {"Moulay_Bousselham", 90},
      {""}, {""},
#line 52 "cities.gperf"
      {"Azrou", 41},
#line 97 "cities.gperf"
      {"had_soualem", 86},
#line 71 "cities.gperf"
      {"Layoune", 60},
#line 106 "cities.gperf"
      {"Tafraout", 95},
#line 102 "cities.gperf"
      {"Khemisset", 91},
#line 80 "cities.gperf"
      {"Bni_Hadifa", 69},
#line 57 "cities.gperf"
      {"Tiflet", 46},
      {""},
#line 89 "cities.gperf"
      {"Khenifra", 78},
      {""},
#line 81 "cities.gperf"
      {"Fquih_Ben_Salah", 70},
      {""}, {""},
#line 111 "cities.gperf"
      {"Skhirate", 100},
#line 45 "cities.gperf"
      {"Taroudant", 34},
#line 18 "cities.gperf"
      {"Oujda", 7},
#line 63 "cities.gperf"
      {"Midelt", 52},
#line 20 "cities.gperf"
      {"Tetouan", 9},
      {""}, {""},
#line 99 "cities.gperf"
      {"Bab_Berred", 88},
#line 47 "cities.gperf"
      {"Dakhla", 36},
      {""}, {""},
#line 61 "cities.gperf"
      {"Sidi_Ifni", 50},
      {""},
#line 37 "cities.gperf"
      {"Settat", 26},
#line 84 "cities.gperf"
      {"Demnate", 73},
#line 87 "cities.gperf"
      {"Akhfenir", 76},
      {""}, {""},
#line 49 "cities.gperf"
      {"Tiznit", 38},
      {""},
#line 107 "cities.gperf"
      {"Imzouren", 96},
#line 13 "cities.gperf"
      {"Marrakech", 2},
      {""},
#line 34 "cities.gperf"
      {"Arfoud", 23},
      {""}, {""}, {""}, {""},
#line 17 "cities.gperf"
      {"Meknes", 6},
      {""}, {""}, {""}, {""},
#line 65 "cities.gperf"
      {"Azilal", 54},
#line 90 "cities.gperf"
      {"Bir_Anzerane", 79},
      {""},
#line 54 "cities.gperf"
      {"Berrechid", 43},
      {""},
#line 79 "cities.gperf"
      {"Tichla", 68},
      {""}, {""}, {""}, {""},
#line 78 "cities.gperf"
      {"Oulad_Teima", 67},
      {""},
#line 40 "cities.gperf"
      {"Kalaat_MGouna", 29},
      {""}, {""},
#line 96 "cities.gperf"
      {"Zagora", 85},
#line 38 "cities.gperf"
      {"B\303\251ni_Mellal", 27},
      {""}, {""}, {""},
#line 105 "cities.gperf"
      {"Sidi_Bouzid", 94},
#line 58 "cities.gperf"
      {"Sidi_Bennour", 47},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 69 "cities.gperf"
      {"Tichka", 58},
      {""}, {""}, {""}, {""},
#line 75 "cities.gperf"
      {"Oujda_Angad", 64},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 16 "cities.gperf"
      {"Agadir", 5},
      {""}, {""}, {""},
#line 11 "cities.gperf"
      {"Casablanca", 0},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 22 "cities.gperf"
      {"El_Jadida", 11},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 43 "cities.gperf"
      {"Al_Hoceima", 32},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 64 "cities.gperf"
      {"Figuig", 53}
    };

  if (len <= MAX_WORD_LENGTH_ && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash_ (str, len);

      if (key <= MAX_HASH_VALUE_)
        {
          const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}

// #===================================== End of City Hash Table =====================================#



static inline void process_line(const char* start, size_t length) {
    const char *end = start + length;
    char *separator1 = (char *)memchr(start, ',', end - start);
    if (!separator1) return;

    char *separator2 = (char *)memchr(separator1 + 1, ',', end - separator1 - 1);
    if (!separator2) return;

    
    size_t cityNameLength = separator1 - start;
    size_t productNameLength = separator2 - separator1 - 1;
    // size_t productPriceLength = end - separator2 - 1;

	// Allocate buffers for city name, product name, and product price
    char cityName[cityNameLength + 1];
    char productName[productNameLength + 1];


    memcpy(cityName, start, cityNameLength);
    cityName[cityNameLength] = '\0';
    memcpy(productName, separator1 + 1, productNameLength);
    productName[productNameLength] = '\0';
	

	double price = atof(separator2 + 1);


	const struct CityEntry *cityEntry = city_hash(cityName, cityNameLength);
	if (cityEntry)
	{
		struct City *city = cities[cityEntry->cityIndex];
		if (!city)
		{
			city = (struct City *)malloc(sizeof(struct City));
			city->total = price;
			city->name = cityEntry->name;
			cities[cityEntry->cityIndex] = city;

			const struct ProductEntry *productEntry = product_hash(productName, productNameLength);
			if (productEntry)
			{
				struct Product *product = (struct Product *)malloc(sizeof(struct Product));
				product->price = price;
				product->name = productEntry->name;
				city->products[productEntry->productIndex] = product;
				city->productsCount++;
			}
			else
			{
				printf("Product Not Found\n");
				exit(EXIT_FAILURE);
			}
		}
		else {
			// city->total += atof(separator2 + 1);
			city->total += price;
			const struct ProductEntry *productEntry = product_hash(productName, productNameLength);
			if (productEntry)
			{
				struct Product *product = city->products[productEntry->productIndex];
				if (!product)
				{
					product = (struct Product *)malloc(sizeof(struct Product));
					product->price = price;
					product->name = productEntry->name;
					city->products[productEntry->productIndex] = product;
					city->productsCount++;
				}
				else
				{
					if (price < product->price)
					{
						product->price = price;
					}
				}
			}
			else
			{
				printf("This product is not listed in gen.py\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	else
	{
		printf("This City is not listed in gen.py\n");
		exit(EXIT_FAILURE);
	}
}

static inline int compareProducts(const void *a, const void *b) {
    const struct Product *productA = *(const struct Product **)a;
    const struct Product *productB = *(const struct Product **)b;
    
    // Handle NULL pointers by considering them as greater than any valid pointer
    if (!productA) return (productB) ? 1 : 0; // If both are NULL, they are considered equal
    if (!productB) return -1; // Non-NULL is considered less than NULL

    // Now that we've handled potential NULLs, we proceed with our actual comparison
    if (productA->price < productB->price) return -1;
    if (productA->price > productB->price) return 1;
    
    // If prices are equal, compare by name
    return strcmp(productA->name, productB->name);
}

int main(void)
{

    int fd = open("./input.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Error getting file size");
        close(fd);
        exit(EXIT_FAILURE);
    }

    char *file_in_memory = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_in_memory == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    const char *end = file_in_memory + sb.st_size;
    const char *current = file_in_memory;
    const char *line_start = current;

    while (current < end) {
        if (*current == '\n') {
            size_t line_length = current - line_start;
            process_line(line_start, line_length);
            line_start = current + 1; // Move past the newline
        }
        current++;
    }

    if (line_start < end) {
        process_line(line_start, end - line_start);
    }

    munmap(file_in_memory, sb.st_size);
    close(fd);

	double cheapestCityTotal = LLONG_MAX;
	struct City *cheapestCity = NULL;
	for (int i = 0; i < 101; i++)
	{
		struct City *city = cities[i];
		if (city)
		{
			if (city->total < cheapestCityTotal)
			{
				cheapestCityTotal = city->total;
				cheapestCity = city;
			}
		}
	}

	int fdo = open ("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fdo == -1) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	write(fdo, cheapestCity->name, strlen(cheapestCity->name));
	write(fdo, " ", 1);
	// write(fdo, &cheapestCity->total, sizeof(cheapestCity->total));
	char buffer[100];
	sprintf(buffer, "%.2f", cheapestCity->total);
	write(fdo, buffer, strlen(buffer));
	write(fdo, "\n", 1);

	qsort(cheapestCity->products, 100, sizeof(struct Product *), compareProducts);

	for (size_t i = 0; i < 5; i++)
	{
		struct Product *product = cheapestCity->products[i];
		if (product)
		{
			write(fdo, product->name, strlen(product->name));
			write(fdo, " ", 1);
			char buffer[100];
			sprintf(buffer, "%.2f", product->price);
			write(fdo, buffer, strlen(buffer));
			write(fdo, "\n", 1);
		}
	}

    return 0;
}