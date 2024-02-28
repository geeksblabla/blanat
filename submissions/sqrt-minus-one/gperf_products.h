/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf products_token.gperf  */
/* Computed positions: -k'1-4' */

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
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 4 "products_token.gperf"
struct Product_Entry
{
  char *name;
  int index;
};

#define PRODUCT_TOTAL_KEYWORDS 94
#define PRODUCT_MIN_WORD_LENGTH 4
#define PRODUCT_MAX_WORD_LENGTH 4
#define PRODUCT_MIN_HASH_VALUE 2
#define PRODUCT_MAX_HASH_VALUE 337
/* maximum key range = 336, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
/*ARGSUSED*/
static unsigned int
product_hash ( const char *str,  size_t len)
{
  static unsigned short asso_values[] =
    {
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338,   2,   0,   0,   2,  95,
       50,  60,  90, 338,  85,  47,   2,  15, 115,  17,
       55, 338,  32,  27,  85, 338, 338, 125, 338,  65,
        5, 338, 338, 338, 338, 338, 338,   0,   0, 115,
        0,   5, 338,  70,   2,  80,  80,  57,  30,  85,
      120,  40,  85,   5,  15,  35,  10,  30,  35,  15,
      338,   0, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338, 338, 338, 338, 338,
      338, 338, 338, 338, 338, 338
    };
  return asso_values[(unsigned char)str[3]] + asso_values[(unsigned char)str[2]] + asso_values[(unsigned char)str[1]] + asso_values[(unsigned char)str[0]];
}

Product_Entry *
product_in_word_set ( const char *str,  size_t len)
{
  static struct Product_Entry wordlist[] =
    {
      {""}, {""},
#line 33 "products_token.gperf"
      {"Chad", 22},
      {""}, {""},
#line 27 "products_token.gperf"
      {"Cabe", 16},
      {""},
#line 34 "products_token.gperf"
      {"Chey", 23},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 39 "products_token.gperf"
      {"Cray", 28},
      {""},
#line 42 "products_token.gperf"
      {"Date", 31},
      {""}, {""},
#line 19 "products_token.gperf"
      {"Beet", 8},
      {""},
#line 63 "products_token.gperf"
      {"Lete", 52},
      {""}, {""},
#line 30 "products_token.gperf"
      {"Cart", 19},
      {""},
#line 44 "products_token.gperf"
      {"Drat", 33},
      {""}, {""},
#line 21 "products_token.gperf"
      {"Blay", 10},
      {""},
#line 14 "products_token.gperf"
      {"Arte", 3},
      {""},
#line 86 "products_token.gperf"
      {"Radh", 75},
#line 32 "products_token.gperf"
      {"Cely", 21},
      {""},
#line 70 "products_token.gperf"
      {"Orae", 59},
      {""}, {""},
#line 36 "products_token.gperf"
      {"Clee", 25},
      {""},
#line 26 "products_token.gperf"
      {"Buth", 15},
      {""}, {""},
#line 31 "products_token.gperf"
      {"Caur", 20},
      {""},
#line 95 "products_token.gperf"
      {"Stat", 84},
      {""}, {""},
#line 20 "products_token.gperf"
      {"Belr", 9},
      {""},
#line 96 "products_token.gperf"
      {"Stry", 85},
      {""}, {""},
#line 41 "products_token.gperf"
      {"Curt", 30},
      {""},
#line 92 "products_token.gperf"
      {"Saly", 81},
      {""}, {""},
#line 22 "products_token.gperf"
      {"Bluy", 11},
      {""},
#line 76 "products_token.gperf"
      {"Peah", 65},
      {""},
#line 88 "products_token.gperf"
      {"Rhub", 77},
#line 18 "products_token.gperf"
      {"Basl", 7},
      {""},
#line 87 "products_token.gperf"
      {"Rasy", 76},
      {""}, {""},
#line 73 "products_token.gperf"
      {"Pary", 62},
      {""},
#line 90 "products_token.gperf"
      {"Ruta", 79},
      {""}, {""},
#line 77 "products_token.gperf"
      {"Pear", 66},
      {""},
#line 71 "products_token.gperf"
      {"Oreo", 60},
      {""}, {""},
#line 25 "products_token.gperf"
      {"Brus", 14},
      {""},
#line 58 "products_token.gperf"
      {"Kale", 47},
      {""}, {""},
#line 51 "products_token.gperf"
      {"Grat", 40},
      {""},
#line 97 "products_token.gperf"
      {"Sweo", 86},
      {""},
#line 68 "products_token.gperf"
      {"Okra", 57},
#line 54 "products_token.gperf"
      {"Guaa", 43},
      {""},
#line 98 "products_token.gperf"
      {"Thye", 87},
      {""}, {""},
#line 78 "products_token.gperf"
      {"Peas", 67},
      {""},
#line 23 "products_token.gperf"
      {"Boky", 12},
      {""}, {""},
#line 75 "products_token.gperf"
      {"Past", 64},
      {""},
#line 91 "products_token.gperf"
      {"Sage", 80},
      {""}, {""},
#line 38 "products_token.gperf"
      {"Cols", 27},
      {""},
#line 89 "products_token.gperf"
      {"Rosy", 78},
      {""}, {""},
#line 52 "products_token.gperf"
      {"Gras", 41},
      {""},
#line 13 "products_token.gperf"
      {"Aprt", 2},
      {""}, {""},
#line 53 "products_token.gperf"
      {"Gres", 42},
      {""},
#line 16 "products_token.gperf"
      {"Avoo", 5},
      {""}, {""},
#line 17 "products_token.gperf"
      {"Bana", 6},
      {""}, {""}, {""}, {""},
#line 29 "products_token.gperf"
      {"Cane", 18},
      {""}, {""}, {""}, {""},
#line 28 "products_token.gperf"
      {"Cacr", 17},
      {""}, {""}, {""}, {""},
#line 24 "products_token.gperf"
      {"Broi", 13},
      {""}, {""}, {""}, {""},
#line 72 "products_token.gperf"
      {"Papa", 61},
      {""},
#line 43 "products_token.gperf"
      {"Dill", 32},
      {""}, {""},
#line 84 "products_token.gperf"
      {"Poto", 73},
      {""},
#line 94 "products_token.gperf"
      {"Squm", 83},
      {""}, {""},
#line 35 "products_token.gperf"
      {"Cilo", 24},
      {""}, {""}, {""}, {""},
#line 74 "products_token.gperf"
      {"Parp", 63},
      {""},
#line 15 "products_token.gperf"
      {"Asps", 4},
      {""},
#line 11 "products_token.gperf"
      {"Acoh", 0},
#line 40 "products_token.gperf"
      {"Cucr", 29},
      {""}, {""}, {""}, {""},
#line 37 "products_token.gperf"
      {"Coct", 26},
      {""}, {""}, {""},
#line 61 "products_token.gperf"
      {"Kohi", 50},
#line 101 "products_token.gperf"
      {"Wats", 90},
      {""},
#line 64 "products_token.gperf"
      {"Lime", 53},
      {""}, {""},
#line 65 "products_token.gperf"
      {"Mano", 54},
      {""},
#line 12 "products_token.gperf"
      {"Appe", 1},
      {""}, {""},
#line 50 "products_token.gperf"
      {"Gojy", 39},
      {""},
#line 59 "products_token.gperf"
      {"Kiwo", 48},
      {""}, {""},
#line 83 "products_token.gperf"
      {"Pome", 72},
      {""}, {""}, {""}, {""},
#line 48 "products_token.gperf"
      {"Garc", 37},
      {""}, {""}, {""},
#line 93 "products_token.gperf"
      {"Spih", 82},
#line 79 "products_token.gperf"
      {"Pern", 68},
      {""}, {""}, {""}, {""},
#line 82 "products_token.gperf"
      {"Plum", 71},
      {""}, {""}, {""}, {""},
#line 81 "products_token.gperf"
      {"Plan", 70},
      {""}, {""}, {""}, {""},
#line 56 "products_token.gperf"
      {"Jact", 45},
      {""},
#line 62 "products_token.gperf"
      {"Lemn", 51},
      {""}, {""},
#line 100 "products_token.gperf"
      {"Turp", 89},
      {""}, {""}, {""}, {""},
#line 46 "products_token.gperf"
      {"Ende", 35},
      {""},
#line 60 "products_token.gperf"
      {"Kiwi", 49},
      {""}, {""},
#line 66 "products_token.gperf"
      {"Mint", 55},
      {""}, {""}, {""}, {""},
#line 104 "products_token.gperf"
      {"Zuci", 93},
      {""}, {""}, {""}, {""},
#line 103 "products_token.gperf"
      {"Yamm", 92},
      {""}, {""}, {""}, {""},
#line 67 "products_token.gperf"
      {"Nece", 56},
      {""}, {""}, {""}, {""},
#line 45 "products_token.gperf"
      {"Eggt", 34},
      {""}, {""}, {""}, {""},
#line 99 "products_token.gperf"
      {"Tomo", 88},
      {""}, {""}, {""}, {""},
#line 102 "products_token.gperf"
      {"Watn", 91},
      {""}, {""}, {""}, {""},
#line 80 "products_token.gperf"
      {"Pine", 69},
      {""}, {""}, {""}, {""},
#line 55 "products_token.gperf"
      {"Honw", 44},
      {""}, {""}, {""}, {""},
#line 47 "products_token.gperf"
      {"Figg", 36},
      {""}, {""}, {""}, {""},
#line 49 "products_token.gperf"
      {"Ginr", 38},
      {""}, {""}, {""}, {""},
#line 57 "products_token.gperf"
      {"Jica", 46},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 85 "products_token.gperf"
      {"Pumn", 74},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 69 "products_token.gperf"
      {"Onin", 58}
    };

  if (len <= PRODUCT_MAX_WORD_LENGTH && len >= PRODUCT_MIN_WORD_LENGTH)
    {
       unsigned int key = product_hash (str, len);

      if (key <= PRODUCT_MAX_HASH_VALUE)
        {
           const char *s = wordlist[key].name;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
            return &wordlist[key];
        }
    }
  return 0;
}
