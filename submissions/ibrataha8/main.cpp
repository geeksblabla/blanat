#ifndef NDEBUG
#define NDEBUG
#endif
#pragma GCC optimize("Ofast")
#include <cassert>
#include <cstring>
#include <emmintrin.h> // _mm_pause
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h> // for ::close

#include <algorithm>
#include <fstream>
#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

static constexpr auto INPUT_FILENAME = "input.txt";
static constexpr auto OUTPUT_FILENAME = "output.txt";
static int const NUM_THREADS = std::thread::hardware_concurrency();

namespace PerfectHashing
{
    static_assert( //
        (' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) && ('%' == 37) && ('&' == 38) &&
            ('\'' == 39) && ('(' == 40) && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) &&
            ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) && ('1' == 49) && ('2' == 50) &&
            ('3' == 51) && ('4' == 52) && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) &&
            ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) && ('=' == 61) && ('>' == 62) &&
            ('?' == 63) && ('A' == 65) && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) &&
            ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) && ('J' == 74) && ('K' == 75) &&
            ('L' == 76) && ('M' == 77) && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) &&
            ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) && ('V' == 86) && ('W' == 87) &&
            ('X' == 88) && ('Y' == 89) && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) &&
            ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) && ('c' == 99) && ('d' == 100) &&
            ('e' == 101) && ('f' == 102) && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) &&
            ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) && ('o' == 111) && ('p' == 112) &&
            ('q' == 113) && ('r' == 114) && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) &&
            ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) && ('{' == 123) && ('|' == 124) &&
            ('}' == 125) && ('~' == 126),
        "unsupported execution character set");

    struct Products
    {
        /* C++ code produced by gperf version 3.1 */
        /* Command-line: gperf -L C++ -D -C -E -m 10000 -r fruits.txt  */
        /* Computed positions: -k'1,3,$' */
        /* maximum key range = 120, duplicates = 0 */
        enum
        {
            TOTAL_KEYWORDS = 94,
            MIN_WORD_LENGTH = 3,
            MAX_WORD_LENGTH = 16,
            MIN_HASH_VALUE = 5,
            MAX_HASH_VALUE = 124,

            NUM = MAX_HASH_VALUE - MIN_HASH_VALUE + 1,
        };
        static_assert(NUM == 120);

        static inline constexpr unsigned hash(std::string_view str)
        {
            uint8_t const asso_values[256] = {
                125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
                125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
                125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
                125, 125, 125, 125, 125, 125, 125, 125, 36, 24, 0, 75, 3, 6, 6, 33, 125, 6, 24,
                66, 72, 0, 66, 0, 125, 42, 33, 42, 125, 125, 93, 125, 0, 24, 125, 125, 125, 125,
                125, 125, 0, 48, 48, 0, 0, 125, 54, 27, 12, 60, 30, 9, 33, 6, 33, 33, 125,
                3, 18, 3, 3, 125, 48, 125, 18, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
                125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
                125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
                125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
                125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
                125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
                125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
                125, 125, 125, 125, 125, 125, 125, 125, 125};
            unsigned len = str.length();
            return len                                               //
                   + asso_values[static_cast<uint8_t>(str[2])]       //
                   + asso_values[static_cast<uint8_t>(str[0])]       //
                   + asso_values[static_cast<uint8_t>(str[len - 1])] //
                   - MIN_HASH_VALUE;
        }

        static constexpr std::string_view reverse(unsigned key, bool strict = false)
        {
            // clang-format off
            constexpr std::array<std::string_view, 94> wordlist{"Chard",
                "Pear", "Endive", "Clementine", "Guava", "Carrot", "Currant",
                "Plantain", "Pineapple", "Cantaloupe", "Cauliflower",
                "Persimmon", "Grapefruit", "Ginger", "Peas", "Cherry",
                "Cranberry", "Parsley", "Grapes", "Beet", "Peach", "Celery",
                "Passion_Fruit", "Green_Beans", "Banana", "Kale", "Papaya",
                "Plum", "Collard_Greens", "Potato", "Parsnip", "Pomegranate",
                "Starfruit", "Pumpkin", "Bell_Pepper", "Artichoke", "Apricot",
                "Cilantro", "Blackberry", "Rutabaga", "Blueberry", "Cabbage",
                "Basil", "Nectarine", "Coconut", "Cucumber", "Jicama",
                "Brussels_Sprouts", "Cactus_Pear", "Garlic", "Strawberry",
                "Thyme", "Jackfruit", "Salsify", "Eggplant", "Yam",
                "Butternut_Squash", "Kohlrabi", "Orange", "Okra", "Apple",
                "Radish", "Lettuce", "Broccoli", "Sweet_Potato", "Spinach",
                "Bok_Choy", "Date", "Squash_Blossom", "Turnip", "Mint",
                "Rosemary", "Raspberry", "Kiwi", "Onion", "Dragon_Fruit",
                "Sage", "Zucchini", "Goji_Berry", "Honeydew", "Asparagus",
                "Dill", "Rhubarb", "Lime", "Oregano", "Acorn_Squash",
                "Avocado", "Lemon", "Kiwano", "Watermelon", "Tomato", "Mango",
                "Fig", "Watercress"};
            // clang-format on
            signed char constexpr lookup[] = {
                -1, -1, -1, -1, -1, 0, -1, 1, -1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, -1,
                13, 14, -1, 15, -1, -1, 16, 17, -1, 18, 19, 20, 21, 22, 23, 24, 25, -1, 26, 27, 28,
                29, 30, 31, 32, 33, 34, 35, 36, 37, -1, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
                49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, -1, 67, 68,
                69, 70, 71, 72, 73, 74, 75, 76, 77, -1, 78, 79, 80, 81, -1, -1, 82, -1, -1, 83, -1,
                -1, 84, -1, 85, 86, 87, 88, 89, -1, 90, -1, 91, 92, -1, -1, -1, -1, -1, -1, 93};

            key += MIN_HASH_VALUE;
            if (key <= MAX_HASH_VALUE)
                if (int index = lookup[key]; index >= 0)
                {
                    auto w = wordlist[index];
                    if (strict)
                        assert(!w.empty());
                    return w;
                }

            return {};
        }

        static constexpr bool in_word_set(std::string_view str)
        {
            unsigned len = str.length();
            if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
            {
                auto s = reverse(hash(str));
                if (str == s)
                    return true;
            }
            return false;
        }

        static constexpr void static_tests()
        {
            static_assert(reverse(hash("Chard")) == "Chard");
            static_assert(reverse(hash("Pear")) == "Pear");
            static_assert(reverse(hash("Endive")) == "Endive");
            static_assert(reverse(hash("Clementine")) == "Clementine");
            static_assert(reverse(hash("Guava")) == "Guava");
            static_assert(reverse(hash("Carrot")) == "Carrot");
            static_assert(reverse(hash("Currant")) == "Currant");
            static_assert(reverse(hash("Plantain")) == "Plantain");
            static_assert(reverse(hash("Pineapple")) == "Pineapple");
            static_assert(reverse(hash("Cantaloupe")) == "Cantaloupe");
            static_assert(reverse(hash("Cauliflower")) == "Cauliflower");
            static_assert(reverse(hash("Persimmon")) == "Persimmon");
            static_assert(reverse(hash("Grapefruit")) == "Grapefruit");
            static_assert(reverse(hash("Ginger")) == "Ginger");
            static_assert(reverse(hash("Peas")) == "Peas");
            static_assert(reverse(hash("Cherry")) == "Cherry");
            static_assert(reverse(hash("Cranberry")) == "Cranberry");
            static_assert(reverse(hash("Parsley")) == "Parsley");
            static_assert(reverse(hash("Grapes")) == "Grapes");
            static_assert(reverse(hash("Beet")) == "Beet");
            static_assert(reverse(hash("Peach")) == "Peach");
            static_assert(reverse(hash("Celery")) == "Celery");
            static_assert(reverse(hash("Passion_Fruit")) == "Passion_Fruit");
            static_assert(reverse(hash("Green_Beans")) == "Green_Beans");
            static_assert(reverse(hash("Banana")) == "Banana");
            static_assert(reverse(hash("Kale")) == "Kale");
            static_assert(reverse(hash("Papaya")) == "Papaya");
            static_assert(reverse(hash("Plum")) == "Plum");
            static_assert(reverse(hash("Collard_Greens")) == "Collard_Greens");
            static_assert(reverse(hash("Potato")) == "Potato");
            static_assert(reverse(hash("Parsnip")) == "Parsnip");
            static_assert(reverse(hash("Pomegranate")) == "Pomegranate");
            static_assert(reverse(hash("Starfruit")) == "Starfruit");
            static_assert(reverse(hash("Pumpkin")) == "Pumpkin");
            static_assert(reverse(hash("Bell_Pepper")) == "Bell_Pepper");
            static_assert(reverse(hash("Artichoke")) == "Artichoke");
            static_assert(reverse(hash("Apricot")) == "Apricot");
            static_assert(reverse(hash("Cilantro")) == "Cilantro");
            static_assert(reverse(hash("Blackberry")) == "Blackberry");
            static_assert(reverse(hash("Rutabaga")) == "Rutabaga");
            static_assert(reverse(hash("Blueberry")) == "Blueberry");
            static_assert(reverse(hash("Cabbage")) == "Cabbage");
            static_assert(reverse(hash("Basil")) == "Basil");
            static_assert(reverse(hash("Nectarine")) == "Nectarine");
            static_assert(reverse(hash("Coconut")) == "Coconut");
            static_assert(reverse(hash("Cucumber")) == "Cucumber");
            static_assert(reverse(hash("Jicama")) == "Jicama");
            static_assert(reverse(hash("Brussels_Sprouts")) == "Brussels_Sprouts");
            static_assert(reverse(hash("Cactus_Pear")) == "Cactus_Pear");
            static_assert(reverse(hash("Garlic")) == "Garlic");
            static_assert(reverse(hash("Strawberry")) == "Strawberry");
            static_assert(reverse(hash("Thyme")) == "Thyme");
            static_assert(reverse(hash("Jackfruit")) == "Jackfruit");
            static_assert(reverse(hash("Salsify")) == "Salsify");
            static_assert(reverse(hash("Eggplant")) == "Eggplant");
            static_assert(reverse(hash("Yam")) == "Yam");
            static_assert(reverse(hash("Butternut_Squash")) == "Butternut_Squash");
            static_assert(reverse(hash("Kohlrabi")) == "Kohlrabi");
            static_assert(reverse(hash("Orange")) == "Orange");
            static_assert(reverse(hash("Okra")) == "Okra");
            static_assert(reverse(hash("Apple")) == "Apple");
            static_assert(reverse(hash("Radish")) == "Radish");
            static_assert(reverse(hash("Lettuce")) == "Lettuce");
            static_assert(reverse(hash("Broccoli")) == "Broccoli");
            static_assert(reverse(hash("Sweet_Potato")) == "Sweet_Potato");
            static_assert(reverse(hash("Spinach")) == "Spinach");
            static_assert(reverse(hash("Bok_Choy")) == "Bok_Choy");
            static_assert(reverse(hash("Date")) == "Date");
            static_assert(reverse(hash("Squash_Blossom")) == "Squash_Blossom");
            static_assert(reverse(hash("Turnip")) == "Turnip");
            static_assert(reverse(hash("Mint")) == "Mint");
            static_assert(reverse(hash("Rosemary")) == "Rosemary");
            static_assert(reverse(hash("Raspberry")) == "Raspberry");
            static_assert(reverse(hash("Kiwi")) == "Kiwi");
            static_assert(reverse(hash("Onion")) == "Onion");
            static_assert(reverse(hash("Dragon_Fruit")) == "Dragon_Fruit");
            static_assert(reverse(hash("Sage")) == "Sage");
            static_assert(reverse(hash("Zucchini")) == "Zucchini");
            static_assert(reverse(hash("Goji_Berry")) == "Goji_Berry");
            static_assert(reverse(hash("Honeydew")) == "Honeydew");
            static_assert(reverse(hash("Asparagus")) == "Asparagus");
            static_assert(reverse(hash("Dill")) == "Dill");
            static_assert(reverse(hash("Rhubarb")) == "Rhubarb");
            static_assert(reverse(hash("Lime")) == "Lime");
            static_assert(reverse(hash("Oregano")) == "Oregano");
            static_assert(reverse(hash("Acorn_Squash")) == "Acorn_Squash");
            static_assert(reverse(hash("Avocado")) == "Avocado");
            static_assert(reverse(hash("Lemon")) == "Lemon");
            static_assert(reverse(hash("Kiwano")) == "Kiwano");
            static_assert(reverse(hash("Watermelon")) == "Watermelon");
            static_assert(reverse(hash("Tomato")) == "Tomato");
            static_assert(reverse(hash("Mango")) == "Mango");
            static_assert(reverse(hash("Fig")) == "Fig");
            static_assert(reverse(hash("Watercress")) == "Watercress");
        }
    };

    struct Cities
    {

        enum
        {
            TOTAL_KEYWORDS = 101,
            MIN_WORD_LENGTH = 3,
            MAX_WORD_LENGTH = 17,
            MIN_HASH_VALUE = 6,
            MAX_HASH_VALUE = 130,

            NUM = MAX_HASH_VALUE - MIN_HASH_VALUE + 1,
        };
        static_assert(NUM == 125);

        static inline constexpr unsigned hash(std::string_view str)
        {
            uint8_t const asso_values[] = {
                131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131,
                131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131,
                131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131,
                131, 131, 131, 131, 131, 131, 131, 131, 131, 67, 60, 131, 70, 131, 131, 131, 73, 131, 65,
                66, 70, 29, 131, 131, 131, 131, 16, 131, 70, 131, 131, 131, 131, 131, 131, 131, 131, 131,
                21, 4, 0, 26, 39, 11, 1, 25, 27, 0, 5, 51, 23, 16, 28, 4, 13, 44, 5,
                0, 6, 18, 0, 8, 131, 131, 58, 43, 34, 131, 131, 131, 131, 131, 131, 131, 131, 131,
                131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131,
                131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 0, 131,
                131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131,
                131, 131, 131, 131, 131, 24, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131,
                131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131,
                131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131,
                131, 131, 131, 131, 131, 131, 131, 131, 131, 131};
            unsigned hval = str.length();
            assert(hval > 1);

            switch (hval)
            {
            default:
                hval += asso_values[static_cast<uint8_t>(str[5])];
                /*FALLTHROUGH*/
            case 5:
                hval += asso_values[static_cast<uint8_t>(str[4] + 1)];
                /*FALLTHROUGH*/
            case 4:
            case 3:
                hval += asso_values[static_cast<uint8_t>(str[2])];
                /*FALLTHROUGH*/
            case 2:
                hval += asso_values[static_cast<uint8_t>(str[1])];
                break;
            }
            return hval - MIN_HASH_VALUE;
        }

        static inline constexpr std::string_view reverse(unsigned key, bool strict = false)
        {
            // clang-format off
        constexpr std::array<std::string_view, 131> wordlist {"",
            "", "", "", "", "", "Drarga", "", "Jerada", "", "Fes",
            "Guerguerat", "Kenitra", "Inezgane", "Guelta_Zemmur",
            "Ben_guerir", "Assa", "Guelmim", "Larache", "Chichaoua",
            "Sale", "Errachidia", "Nador", "Chefchaouen", "Guercif",
            "Ksar_es_Seghir", "Tinghir", "Midar", "Taroudant", "Safi",
            "Goulmima", "Rabat", "Taourirt", "Khouribga", "Tetouan",
            "Boujdour", "Ahfir", "Skhirate", "Berkane", "Zagora",
            "Zemamra", "Temara", "Ouazzane", "Asilah", "Ouarzazate",
            "Ifrane", "Fquih_Ben_Salah", "Taza", "Sidi_Slimane",
            "Oulad_Teima", "Arfoud", "Bouarfa", "Laayoune", "had_soualem",
            "Youssoufia", "Mohammedia", "Azrou", "Dakhla", "Marrakech",
            "Smara", "Akhfenir", "Meknes", "Saidia", "Tangier",
            "Ait_Melloul", "Essaouira", "Tichka", "Khemisset", "Midelt",
            "Settat", "Aourir", "Casablanca", "Tafraout", "Kalaat_MGouna",
            "Berrechid", "Boulemane", "Sefrou", "Layoune", "Tichla",
            "Tiflet", "Demnate", "Tan-Tan", "Oujda", "El_Jadida", "Agadir",
            "Khenifra", "Bir_Lehlou", "Imzouren", "Bir_Anzerane",
            "La\303\242youne", "Beni_Mellal", "Tarfaya", "Bni_Hadifa",
            "Ksar_El_Kebir", "Souk_Larbaa", "Sidi_Kacem", "Azilal",
            "Bab_Berred", "Sidi_Bouzid", "Sidi_Bennour", "Jorf_El_Melha",
            "", "Sidi_Ifni", "", "Bab_Taza", "", "", "",
            "B\303\251ni_Mellal", "Oujda_Angad", "", "", "", "",
            "Moulay_Bousselham", "", "Figuig", "", "", "", "", "", "",
            "Tiznit", "", "", "", "", "", "", "Al_Hoceima"};
            // clang-format on

            key += MIN_HASH_VALUE;

            if (key <= MAX_HASH_VALUE)
            {
                auto w = wordlist[key];
                if (strict)
                    assert(!w.empty());
                return w;
            }
            return {};
        }

        static constexpr bool in_word_set(std::string_view str)
        {
            unsigned len = str.length();
            if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
            {
                auto s = reverse(hash(str));
                if (str == s)
                    return true;
            }
            return false;
        }

        static constexpr void static_tests()
        {
            static_assert(reverse(1) == ""); // invalid hash key
            static_assert(hash("Al_Hoceima") == NUM - 1);
            static_assert(reverse(NUM - 1) == "Al_Hoceima");
            // static_assert(reverse(hash("")) == "");
            static_assert(reverse(hash("Agadir")) == "Agadir");
            static_assert(reverse(hash("Ahfir")) == "Ahfir");
            static_assert(reverse(hash("Ait_Melloul")) == "Ait_Melloul");
            static_assert(reverse(hash("Akhfenir")) == "Akhfenir");
            static_assert(reverse(hash("Al_Hoceima")) == "Al_Hoceima");
            static_assert(reverse(hash("Aourir")) == "Aourir");
            static_assert(reverse(hash("Arfoud")) == "Arfoud");
            static_assert(reverse(hash("Asilah")) == "Asilah");
            static_assert(reverse(hash("Assa")) == "Assa");
            static_assert(reverse(hash("Azilal")) == "Azilal");
            static_assert(reverse(hash("Azrou")) == "Azrou");
            static_assert(reverse(hash("B\303\251ni_Mellal")) == "B\303\251ni_Mellal");
            static_assert(reverse(hash("Bab_Berred")) == "Bab_Berred");
            static_assert(reverse(hash("Bab_Taza")) == "Bab_Taza");
            static_assert(reverse(hash("Ben_guerir")) == "Ben_guerir");
            static_assert(reverse(hash("Beni_Mellal")) == "Beni_Mellal");
            static_assert(reverse(hash("Berkane")) == "Berkane");
            static_assert(reverse(hash("Berrechid")) == "Berrechid");
            static_assert(reverse(hash("Bir_Anzerane")) == "Bir_Anzerane");
            static_assert(reverse(hash("Bir_Lehlou")) == "Bir_Lehlou");
            static_assert(reverse(hash("Bni_Hadifa")) == "Bni_Hadifa");
            static_assert(reverse(hash("Bouarfa")) == "Bouarfa");
            static_assert(reverse(hash("Boujdour")) == "Boujdour");
            static_assert(reverse(hash("Boulemane")) == "Boulemane");
            static_assert(reverse(hash("Casablanca")) == "Casablanca");
            static_assert(reverse(hash("Chefchaouen")) == "Chefchaouen");
            static_assert(reverse(hash("Chichaoua")) == "Chichaoua");
            static_assert(reverse(hash("Dakhla")) == "Dakhla");
            static_assert(reverse(hash("Demnate")) == "Demnate");
            static_assert(reverse(hash("Drarga")) == "Drarga");
            static_assert(reverse(hash("El_Jadida")) == "El_Jadida");
            static_assert(reverse(hash("Errachidia")) == "Errachidia");
            static_assert(reverse(hash("Essaouira")) == "Essaouira");
            static_assert(reverse(hash("Fes")) == "Fes");
            static_assert(reverse(hash("Figuig")) == "Figuig");
            static_assert(reverse(hash("Fquih_Ben_Salah")) == "Fquih_Ben_Salah");
            static_assert(reverse(hash("Goulmima")) == "Goulmima");
            static_assert(reverse(hash("Guelmim")) == "Guelmim");
            static_assert(reverse(hash("Guelta_Zemmur")) == "Guelta_Zemmur");
            static_assert(reverse(hash("Guercif")) == "Guercif");
            static_assert(reverse(hash("Guerguerat")) == "Guerguerat");
            static_assert(reverse(hash("Ifrane")) == "Ifrane");
            static_assert(reverse(hash("Imzouren")) == "Imzouren");
            static_assert(reverse(hash("Inezgane")) == "Inezgane");
            static_assert(reverse(hash("Jerada")) == "Jerada");
            static_assert(reverse(hash("Jorf_El_Melha")) == "Jorf_El_Melha");
            static_assert(reverse(hash("Kalaat_MGouna")) == "Kalaat_MGouna");
            static_assert(reverse(hash("Kenitra")) == "Kenitra");
            static_assert(reverse(hash("Khemisset")) == "Khemisset");
            static_assert(reverse(hash("Khenifra")) == "Khenifra");
            static_assert(reverse(hash("Khouribga")) == "Khouribga");
            static_assert(reverse(hash("Ksar_El_Kebir")) == "Ksar_El_Kebir");
            static_assert(reverse(hash("Ksar_es_Seghir")) == "Ksar_es_Seghir");
            static_assert(reverse(hash("La\303\242youne")) == "La\303\242youne");
            static_assert(reverse(hash("Laayoune")) == "Laayoune");
            static_assert(reverse(hash("Larache")) == "Larache");
            static_assert(reverse(hash("Layoune")) == "Layoune");
            static_assert(reverse(hash("Marrakech")) == "Marrakech");
            static_assert(reverse(hash("Meknes")) == "Meknes");
            static_assert(reverse(hash("Midar")) == "Midar");
            static_assert(reverse(hash("Midelt")) == "Midelt");
            static_assert(reverse(hash("Mohammedia")) == "Mohammedia");
            static_assert(reverse(hash("Moulay_Bousselham")) == "Moulay_Bousselham");
            static_assert(reverse(hash("Nador")) == "Nador");
            static_assert(reverse(hash("Ouarzazate")) == "Ouarzazate");
            static_assert(reverse(hash("Ouazzane")) == "Ouazzane");
            static_assert(reverse(hash("Oujda")) == "Oujda");
            static_assert(reverse(hash("Oujda_Angad")) == "Oujda_Angad");
            static_assert(reverse(hash("Oulad_Teima")) == "Oulad_Teima");
            static_assert(reverse(hash("Rabat")) == "Rabat");
            static_assert(reverse(hash("Safi")) == "Safi");
            static_assert(reverse(hash("Saidia")) == "Saidia");
            static_assert(reverse(hash("Sale")) == "Sale");
            static_assert(reverse(hash("Sefrou")) == "Sefrou");
            static_assert(reverse(hash("Settat")) == "Settat");
            static_assert(reverse(hash("Sidi_Bennour")) == "Sidi_Bennour");
            static_assert(reverse(hash("Sidi_Bouzid")) == "Sidi_Bouzid");
            static_assert(reverse(hash("Sidi_Ifni")) == "Sidi_Ifni");
            static_assert(reverse(hash("Sidi_Kacem")) == "Sidi_Kacem");
            static_assert(reverse(hash("Sidi_Slimane")) == "Sidi_Slimane");
            static_assert(reverse(hash("Skhirate")) == "Skhirate");
            static_assert(reverse(hash("Smara")) == "Smara");
            static_assert(reverse(hash("Souk_Larbaa")) == "Souk_Larbaa");
            static_assert(reverse(hash("Tafraout")) == "Tafraout");
            static_assert(reverse(hash("Tan-Tan")) == "Tan-Tan");
            static_assert(reverse(hash("Tangier")) == "Tangier");
            static_assert(reverse(hash("Taourirt")) == "Taourirt");
            static_assert(reverse(hash("Tarfaya")) == "Tarfaya");
            static_assert(reverse(hash("Taroudant")) == "Taroudant");
            static_assert(reverse(hash("Taza")) == "Taza");
            static_assert(reverse(hash("Temara")) == "Temara");
            static_assert(reverse(hash("Tetouan")) == "Tetouan");
            static_assert(reverse(hash("Tichka")) == "Tichka");
            static_assert(reverse(hash("Tichla")) == "Tichla");
            static_assert(reverse(hash("Tiflet")) == "Tiflet");
            static_assert(reverse(hash("Tinghir")) == "Tinghir");
            static_assert(reverse(hash("Tiznit")) == "Tiznit");
            static_assert(reverse(hash("Youssoufia")) == "Youssoufia");
            static_assert(reverse(hash("Zagora")) == "Zagora");
            static_assert(reverse(hash("Zemamra")) == "Zemamra");
            static_assert(reverse(hash("had_soualem")) == "had_soualem");
        }
    };
} // namespace PerfectHashing

struct MappedFile
{
    MappedFile() = default;

    MappedFile(char const *name)
    {
        fd = open(name, O_RDONLY);
        if (fd == -1)
            handle_error("map_input: open failed");

        struct stat sb;
        if (fstat(fd, &sb) == -1)
            handle_error("map_input: fstat failed");

        auto addr = static_cast<char *>(mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0u));
        if (addr == MAP_FAILED)
            handle_error("map_input: mmap failed");

        file_size = static_cast<size_t>(sb.st_size);
        file_data = addr;

        madvise((void *)addr, file_size, MADV_SEQUENTIAL);
    }

    std::string_view view() const
    { //
        return {file_data, file_size};
    }

    void close()
    {
        if (file_data)
        {
            munmap(static_cast<void *>(file_data), file_size);
        }
        if (fd != -1)
            ::close(fd);
        fd = -1;
        file_size = 0;
        file_data = nullptr;
    }

    ~MappedFile() { close(); }

    static inline void handle_error(char const *msg)
    {
        perror(msg);
        std::exit(255);
    }

    int fd = -1;
    size_t file_size = 0;
    char /*const*/ *file_data = nullptr;
};

using PerfectHashing::Cities;
using PerfectHashing::Products;
using Cost = std::intmax_t;
static constexpr Cost MaxCost = std::numeric_limits<Cost>::max();

struct Result
{
    std::array<Cost, Cities::NUM> city_total{};
    std::array<std::array<Cost, Products::NUM>, Cities::NUM> per_city;

    Result()
    {
        for (auto &c : per_city)
            std::fill(begin(c), end(c), MaxCost);
    }

    void registerPricing(std::string_view city, std::string_view prod, Cost price)
    {
        assert(Cities::in_word_set(city));
        assert(Products::in_word_set(prod));
        auto ca = Cities::hash(city);
        auto pa = Products::hash(prod);

#if 0 && __cpp_lib_atomic_ref // c++20
        std::atomic_ref(city_total[ca]) += price;

        auto ref = std::atomic_ref(per_city[ca][pa]);
        Cost cur = ref;
        while (!(ref.compare_exchange_strong(cur, std::min(cur, price))))
            /*_mm_pause()*/;
#else
        __atomic_add_fetch(&city_total[ca], price, __ATOMIC_ACQ_REL);

        Cost &ref = per_city[ca][pa];
        Cost cur = ref, desired;
        do
        {
            /*_mm_pause()*/;
            desired = std::min(cur, price);
        } while (!__atomic_compare_exchange(&ref, &cur, &desired, false, __ATOMIC_ACQ_REL, __ATOMIC_RELAXED));
#endif
    }
};

inline constexpr std::string_view consume_str(char const *&start)
{
    int i = 0;
    while (start[i] != ',')
        ++i;
    auto ret = std::string_view(start, i);
    start += 1 + i; // eat comma as well
    return ret;
}

inline constexpr Cost consume_float_as_long(char const *&start)
{
    Cost val = 0;
    char c = 0;

    // integral part
    while ((c = *start) <= '9' && c >= '0')
    {
        val = val * 10 + c - '0';
        ++start;
    }
    ++start; // skip dot or \n
    if (c == '.')
    { // fractional part exists, consume!
        int count = 0;
        while ((c = *start) <= '9' && c >= '0')
        {
            val = val * 10 + c - '0';
            ++start;
            ++count;
        }
        if (count == 1)
            val *= 10; // always precision=2
        ++start;       // skip \n or \r
        if (*start == '\n')
            ++start; // skip \n if prefixed by \r
    }
    return val;
}

void process_chunk(char const *start, char const *end, Result &r)
{
    // madvise((void *)start, end - start, MADV_SEQUENTIAL);
    for (char const *cur = start; cur < end;)
    {
        auto city = consume_str(cur);
        auto product = consume_str(cur);
        Cost price = consume_float_as_long(cur);

        r.registerPricing(city, product, price);
    }
}

inline Result process_concurrently(MappedFile const &mp, unsigned threads = NUM_THREADS)
{
    char const *start = mp.file_data;
    char const *const end = start + mp.file_size;
    const size_t block_size = mp.file_size / std::max(1u, threads);

    Result shared_results;
    std::vector<std::future<void>> future_results;
    while (start < end)
    {
        if (*start == '\n')
            ++start;
        auto next = threads ? std::min(end, start + block_size) : end;
        next = std::find(next, end, '\n');
#if 1
        future_results.emplace_back(std::async(process_chunk, start, next, std::ref(shared_results)));
#else
        process_chunk(start, next, shared_results);
#endif
        start = next;
    }

    for (auto &fr : future_results)
        fr.get();

    return shared_results;
}

inline void ans(Result &result)
{
#if 1
    std::ofstream os(OUTPUT_FILENAME);
#else
    std::ostream &os = std::cout;
#endif
    os << std::fixed << std::setprecision(2);

    struct CityRec
    {
        Cost total;
        std::string_view name;
        unsigned id;

        constexpr bool operator<(CityRec const &rhs) const
        {
            return std::tie(total, name) < std::tie(rhs.total, rhs.name);
        }
    };
    std::vector<CityRec> city_records;

    for (unsigned id = 0; id < Cities::NUM; ++id)
    {
        if (result.city_total[id] == 0) // city was never named in input
            continue;
        auto name = Cities::reverse(id); // valid hash key?
        if (name.empty())
            continue;
        city_records.push_back({result.city_total[id], name, id});
    }

    if (city_records.empty())
        return;

    auto min_city = *std::min_element(city_records.begin(), city_records.end());
    os << min_city.name << " " << min_city.total / 100.0 << "\n";

    std::vector<std::pair<Cost, std::string_view>> products;

    auto &record = result.per_city[min_city.id];

    for (unsigned id = 0; id < Products::NUM; ++id)
    {
        auto name = Products::reverse(id); // valid hash key?
        if (name.empty())
            continue;
        products.emplace_back(record[id], name);
    }

    size_t topN = std::min(5ul, products.size());
    partial_sort(products.begin(), products.begin() + topN, products.end());
    // products.resize(5);

    for (auto &[cost, prod] : products)
    {
        if (topN--)
            os << prod << " " << cost / 100.0 << "\n";
        else
            break;
    }
}

int main()
{
    MappedFile mp(INPUT_FILENAME);

    if (pid_t pid = fork(); pid == 0)
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        exit(0);
    }

    auto result = process_concurrently(mp);
    wait(NULL);

    ans(result);
}