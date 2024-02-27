#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define NUM_CITIES 101
#define NUM_PRODUCTS 94
#define MAX_LINE_LENGTH 100

const char *moroccan_cities[NUM_CITIES] = {
    "Assa",
    "Ksar_es_Seghir",
    "Sefrou",
    "Khenifra",
    "Chefchaouen",
    "Jorf_El_Melha",
    "Fes",
    "Layoune",
    "Casablanca",
    "Ben_guerir",
    "Skhirate",
    "Midelt",
    "Imzouren",
    "Temara",
    "Béni_Mellal",
    "Sidi_Bouzid",
    "Guelta_Zemmur",
    "El_Jadida",
    "Sidi_Bennour",
    "Taroudant",
    "Smara",
    "Drarga",
    "Laâyoune",
    "Fquih_Ben_Salah",
    "Khemisset",
    "Taourirt",
    "Ahfir",
    "Errachidia",
    "Ifrane",
    "Bir_Anzerane",
    "Mohammedia",
    "Nador",
    "Rabat",
    "Demnate",
    "Boulemane",
    "Ksar_El_Kebir",
    "Khouribga",
    "Ouazzane",
    "Tiflet",
    "Al_Hoceima",
    "Oujda_Angad",
    "Bni_Hadifa",
    "Berrechid",
    "Guercif",
    "Aourir",
    "Azilal",
    "Ouarzazate",
    "Settat",
    "Berkane",
    "had_soualem",
    "Saidia",
    "Taza",
    "Oujda",
    "Tangier",
    "Tafraout",
    "Goulmima",
    "Marrakech",
    "Ait_Melloul",
    "Bir_Lehlou",
    "Tichka",
    "Midar",
    "Youssoufia",
    "Sidi_Slimane",
    "Oulad_Teima",
    "Souk_Larbaa",
    "Kalaat_MGouna",
    "Sidi_Ifni",
    "Chichaoua",
    "Tichla",
    "Guelmim",
    "Asilah",
    "Agadir",
    "Kenitra",
    "Jerada",
    "Sale",
    "Moulay_Bousselham",
    "Tarfaya",
    "Boujdour",
    "Tetouan",
    "Safi",
    "Arfoud",
    "Sidi_Kacem",
    "Inezgane",
    "Meknes",
    "Tiznit",
    "Bab_Taza",
    "Beni_Mellal",
    "Tinghir",
    "Essaouira",
    "Dakhla",
    "Bab_Berred",
    "Figuig",
    "Tan-Tan",
    "Bouarfa",
    "Guerguerat",
    "Laayoune",
    "Azrou",
    "Larache",
    "Zemamra",
    "Zagora",
    "Khouribga",
    "Akhfenir"};

const char *fruits_and_vegetables[NUM_PRODUCTS] = {
    "Coconut",
    "Cherry",
    "Pomegranate",
    "Lime",
    "Green_Beans",
    "Pineapple",
    "Salsify",
    "Fig",
    "Kale",
    "Eggplant",
    "Plum",
    "Okra",
    "Ginger",
    "Oregano",
    "Jackfruit",
    "Plantain",
    "Spinach",
    "Peach",
    "Avocado",
    "Asparagus",
    "Carrot",
    "Rutabaga",
    "Potato",
    "Peas",
    "Parsnip",
    "Cranberry",
    "Lemon",
    "Collard_Greens",
    "Nectarine",
    "Rhubarb",
    "Radish",
    "Zucchini",
    "Squash_Blossom",
    "Cactus_Pear",
    "Kiwano",
    "Honeydew",
    "Apricot",
    "Persimmon",
    "Acorn_Squash",
    "Dill",
    "Raspberry",
    "Lettuce",
    "Endive",
    "Butternut_Squash",
    "Mint",
    "Kiwi",
    "Kohlrabi",
    "Mango",
    "Blackberry",
    "Basil",
    "Cantaloupe",
    "Thyme",
    "Clementine",
    "Apple",
    "Grapes",
    "Pear",
    "Blueberry",
    "Bok_Choy",
    "Grapefruit",
    "Cilantro",
    "Artichoke",
    "Yam",
    "Strawberry",
    "Broccoli",
    "Chard",
    "Dragon_Fruit",
    "Parsley",
    "Watercress",
    "Date",
    "Garlic",
    "Jicama",
    "Cauliflower",
    "Orange",
    "Brussels_Sprouts",
    "Guava",
    "Pumpkin",
    "Rosemary",
    "Turnip",
    "Banana",
    "Papaya",
    "Onion",
    "Passion_Fruit",
    "Starfruit",
    "Tomato",
    "Bell_Pepper",
    "Sage",
    "Cabbage",
    "Goji_Berry",
    "Celery",
    "Watermelon",
    "Beet",
    "Currant",
    "Sweet_Potato",
    "Cucumber"};

#

int main()
{
    FILE *input_file = fopen("input.txt", "r");
    FILE *output_file = fopen("output.txt", "w");

    int64_t total_per_city[NUM_CITIES];
    memset(total_per_city, 0, sizeof(total_per_city));

    char line[MAX_LINE_LENGTH];
    char city[100], product[100];
    float price;

    // Read prices from the input file
    while (fgets(line, sizeof(line), input_file))
    {
        sscanf(line, "%[^,],%[^,],%f", city, product, &price);

        int city_idx = -1;

        // Find city index
        for (int i = 0; i < NUM_CITIES; ++i)
        {
            if (strcmp(moroccan_cities[i], city) == 0)
            {
                city_idx = i;
                break;
            }
        }

        if (city_idx != -1)
        {
            int64_t price_in_cents = price * 100;
            total_per_city[city_idx] += price_in_cents;
        }
    }

    // Find the 5 cheapest cities along with their total prices
    struct CityPrice
    {
        int city_index;
        int64_t total_price;
    };

    struct CityPrice cheapest_cities[5];
    for (int i = 0; i < 5; ++i)
    {
        cheapest_cities[i].city_index = -1;
        cheapest_cities[i].total_price = INT64_MAX;
    }

    for (int i = 0; i < NUM_CITIES; ++i)
    {
        if (total_per_city[i] > 0)
        {
            // Update the cheapest cities list
            for (int j = 0; j < 5; ++j)
            {
                if (total_per_city[i] < cheapest_cities[j].total_price)
                {
                    for (int k = 4; k > j; --k)
                    {
                        cheapest_cities[k] = cheapest_cities[k - 1];
                    }
                    cheapest_cities[j].city_index = i;
                    cheapest_cities[j].total_price = total_per_city[i];
                    break;
                }
            }
        }
    }

    // Print the 5 cheapest cities and their total prices
    for (int i = 0; i < 5; ++i)
    {
        if (cheapest_cities[i].city_index != -1)
        {
            fprintf(output_file, "%s %.2f\n", moroccan_cities[cheapest_cities[i].city_index], (double)cheapest_cities[i].total_price / 100.0);
        }
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}
