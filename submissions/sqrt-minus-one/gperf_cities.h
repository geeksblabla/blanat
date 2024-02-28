/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf cities_token.gperf  */
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

#line 4 "cities_token.gperf"
struct City_Entry
{
  char *name;
  int index;
};

#define CITIES_TOTAL_KEYWORDS 101
#define CITIES_MIN_WORD_LENGTH 4
#define CITIES_MAX_WORD_LENGTH 4
#define CITIES_MIN_HASH_VALUE 17
#define CITIES_MAX_HASH_VALUE 340
/* maximum key range = 324, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
/*ARGSUSED*/
static unsigned int
city_hash ( const char *str,  size_t len)
{
  static unsigned short asso_values[] =
    {
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341,  80, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341,  60, 341, 341, 341, 341,
      341, 341, 341, 341, 341,   5, 341,  95,  45,  82,
        5,  75, 100,   0, 120,  15,  67,  12,   9,  90,
        0,   2,  32,  60,  10,  55,  32,   0,   5, 341,
      341,  65,  72,   5, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341,  20,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341,   5, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341, 341, 341, 341,
      341, 341, 341, 341, 341, 341, 341
    };
  return asso_values[(unsigned char)str[3]] + asso_values[(unsigned char)str[2]+1] + asso_values[(unsigned char)str[1]] + asso_values[(unsigned char)str[0]];
}

 City_Entry *
city_in_word_set ( const char *str,  size_t len)
{
  static struct City_Entry wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 109 "cities_token.gperf"
      {"oufi", 98},
      {""}, {""},
#line 89 "cities_token.gperf"
      {"idfn", 78},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 33 "cities_token.gperf"
      {"ouou", 22},
#line 45 "cities_token.gperf"
      {"igui", 34},
      {""}, {""}, {""}, {""},
#line 88 "cities_token.gperf"
      {"idzi", 77},
      {""}, {""}, {""}, {""},
#line 107 "cities_token.gperf"
      {"inhi", 96},
      {""},
#line 34 "cities_token.gperf"
      {"ouan", 23},
      {""}, {""}, {""}, {""},
#line 87 "cities_token.gperf"
      {"idou", 76},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 91 "cities_token.gperf"
      {"idan", 80},
      {""}, {""}, {""}, {""},
#line 29 "cities_token.gperf"
      {"iran", 18},
      {""},
#line 16 "cities_token.gperf"
      {"ouri", 5},
      {""}, {""},
#line 49 "cities_token.gperf"
      {"uemu", 38},
      {""}, {""}, {""},
#line 13 "cities_token.gperf"
      {"itou", 2},
      {""}, {""}, {""}, {""},
#line 79 "cities_token.gperf"
      {"ujjd", 68},
#line 59 "cities_token.gperf"
      {"entr", 48},
#line 41 "cities_token.gperf"
      {"l_id", 30},
      {""}, {""}, {""},
#line 48 "cities_token.gperf"
      {"uemi", 37},
      {""}, {""}, {""}, {""},
#line 50 "cities_token.gperf"
      {"ueci", 39},
      {""}, {""}, {""}, {""},
#line 90 "cities_token.gperf"
      {"idce", 79},
      {""}, {""}, {""},
#line 108 "cities_token.gperf"
      {"izni", 97},
#line 65 "cities_token.gperf"
      {"a\303un", 54},
      {""}, {""}, {""},
#line 58 "cities_token.gperf"
      {"alun", 47},
#line 42 "cities_token.gperf"
      {"rrdi", 31},
      {""},
#line 74 "cities_token.gperf"
      {"ouha", 63},
      {""}, {""},
#line 28 "cities_token.gperf"
      {"erhi", 17},
      {""},
#line 30 "cities_token.gperf"
      {"irlo", 19},
      {""}, {""},
#line 55 "cities_token.gperf"
      {"nean", 44},
#line 105 "cities_token.gperf"
      {"ichl", 94},
      {""}, {""},
#line 104 "cities_token.gperf"
      {"ichk", 93},
      {""}, {""}, {""}, {""},
#line 72 "cities_token.gperf"
      {"idel", 61},
#line 27 "cities_token.gperf"
      {"eran", 16},
#line 38 "cities_token.gperf"
      {"akhl", 27},
      {""}, {""}, {""},
#line 56 "cities_token.gperf"
      {"erad", 45},
      {""},
#line 110 "cities_token.gperf"
      {"agor", 99},
      {""},
#line 21 "cities_token.gperf"
      {"zrro", 10},
#line 77 "cities_token.gperf"
      {"uaan", 66},
      {""},
#line 17 "cities_token.gperf"
      {"rfou", 6},
      {""}, {""},
#line 25 "cities_token.gperf"
      {"enri", 14},
      {""}, {""}, {""},
#line 14 "cities_token.gperf"
      {"khni", 3},
#line 100 "cities_token.gperf"
      {"aran", 89},
      {""}, {""}, {""}, {""},
#line 53 "cities_token.gperf"
      {"fran", 42},
      {""},
#line 32 "cities_token.gperf"
      {"ourf", 21},
#line 84 "cities_token.gperf"
      {"alal", 73},
#line 47 "cities_token.gperf"
      {"ouim", 36},
#line 40 "cities_token.gperf"
      {"rarg", 29},
      {""}, {""}, {""},
#line 70 "cities_token.gperf"
      {"ekne", 59},
#line 68 "cities_token.gperf"
      {"ayun", 57},
#line 80 "cities_token.gperf"
      {"ulim", 69},
#line 37 "cities_token.gperf"
      {"hiou", 26},
      {""}, {""}, {""},
#line 15 "cities_token.gperf"
      {"l_im", 4},
#line 76 "cities_token.gperf"
      {"uaat", 65},
      {""},
#line 98 "cities_token.gperf"
      {"aoir", 87},
#line 111 "cities_token.gperf"
      {"emmr", 100},
      {""},
#line 75 "cities_token.gperf"
      {"addo", 64},
      {""},
#line 94 "cities_token.gperf"
      {"ouba", 83},
#line 64 "cities_token.gperf"
      {"sahi", 53},
      {""},
#line 31 "cities_token.gperf"
      {"niif", 20},
      {""}, {""},
#line 11 "cities_token.gperf"
      {"gadi", 0},
      {""},
#line 43 "cities_token.gperf"
      {"ssir", 32},
      {""}, {""},
#line 71 "cities_token.gperf"
      {"idda", 60},
      {""}, {""}, {""}, {""},
#line 66 "cities_token.gperf"
      {"aaun", 55},
      {""},
#line 19 "cities_token.gperf"
      {"ssss", 8},
      {""}, {""},
#line 83 "cities_token.gperf"
      {"aidi", 72},
      {""},
#line 86 "cities_token.gperf"
      {"etta", 75},
      {""},
#line 62 "cities_token.gperf"
      {"hobg", 51},
#line 61 "cities_token.gperf"
      {"hefr", 50},
      {""},
#line 103 "cities_token.gperf"
      {"etua", 92},
      {""},
#line 92 "cities_token.gperf"
      {"khat", 81},
#line 22 "cities_token.gperf"
      {"\303\251la", 11},
      {""},
#line 73 "cities_token.gperf"
      {"ohdi", 62},
      {""}, {""},
#line 99 "cities_token.gperf"
      {"aray", 88},
      {""}, {""}, {""}, {""},
#line 102 "cities_token.gperf"
      {"emar", 91},
      {""},
#line 57 "cities_token.gperf"
      {"orlh", 46},
      {""}, {""},
#line 51 "cities_token.gperf"
      {"uera", 40},
      {""},
#line 95 "cities_token.gperf"
      {"afou", 84},
      {""}, {""},
#line 67 "cities_token.gperf"
      {"arch", 56},
      {""},
#line 85 "cities_token.gperf"
      {"efro", 74},
      {""},
#line 35 "cities_token.gperf"
      {"asnc", 24},
#line 12 "cities_token.gperf"
      {"hffi", 1},
      {""},
#line 97 "cities_token.gperf"
      {"anie", 86},
      {""}, {""},
#line 93 "cities_token.gperf"
      {"maar", 82},
      {""},
#line 39 "cities_token.gperf"
      {"emat", 28},
      {""}, {""},
#line 46 "cities_token.gperf"
      {"qula", 35},
      {""},
#line 63 "cities_token.gperf"
      {"sabi", 52},
      {""}, {""},
#line 96 "cities_token.gperf"
      {"anTa", 85},
      {""}, {""}, {""}, {""},
#line 18 "cities_token.gperf"
      {"sila", 7},
      {""},
#line 24 "cities_token.gperf"
      {"abaz", 13},
      {""}, {""},
#line 26 "cities_token.gperf"
      {"enla", 15},
      {""}, {""}, {""}, {""},
#line 52 "cities_token.gperf"
      {"adle", 41},
      {""}, {""}, {""}, {""},
#line 23 "cities_token.gperf"
      {"abre", 12},
      {""},
#line 20 "cities_token.gperf"
      {"zila", 9},
      {""}, {""},
#line 36 "cities_token.gperf"
      {"heue", 25},
      {""}, {""}, {""}, {""},
#line 106 "cities_token.gperf"
      {"ifle", 95},
      {""},
#line 78 "cities_token.gperf"
      {"ujga", 67},
      {""},
#line 101 "cities_token.gperf"
      {"azaz", 90},
#line 44 "cities_token.gperf"
      {"esFe", 33},
      {""},
#line 69 "cities_token.gperf"
      {"arec", 58},
      {""}, {""}, {""}, {""},
#line 54 "cities_token.gperf"
      {"mzre", 43},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 60 "cities_token.gperf"
      {"hese", 49},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 81 "cities_token.gperf"
      {"abba", 70},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 82 "cities_token.gperf"
      {"afaf", 71}
    };

  if (len <= CITIES_MAX_WORD_LENGTH && len >= CITIES_MIN_WORD_LENGTH)
    {
       unsigned int key = city_hash (str, len);

      if (key <= CITIES_MAX_HASH_VALUE)
        {
           const char *s = wordlist[key].name;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
            return &wordlist[key];
        }
    }
  return 0;
}
