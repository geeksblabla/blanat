#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define NUM_CITIES 101
#define NUM_PRODUCTS 94
#define MAX_LINE_LENGTH 100
#define MAX_PRODUCT_NAME_LENGTH 100
#define MAX_PRODUCTS 5

char printed_product_names[MAX_PRODUCTS][MAX_PRODUCT_NAME_LENGTH] = {0};

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
    "casa",
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
    "rabat",
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
    "Gaz",
    "Sugar",
    "Flour",
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
    "Oil",
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
    "Cucumber"
};

struct ProductPrice {
    const char *name;
    float price;
};

struct CityData {
    int city_index;
    int64_t total_price;
    struct ProductPrice cheapest_products[5];
};

int main() {
    FILE *input_file = fopen("input.txt", "r");
    FILE *output_file = fopen("output.txt", "w");

    int64_t total_per_city[NUM_CITIES] = {0};
    struct CityData cities[NUM_CITIES] = {{0}};

    char line[MAX_LINE_LENGTH];
    char city[100], product[100];
    float price;

    // Read prices from the input file
    while (fgets(line, sizeof(line), input_file)) {
        if (sscanf(line, "%[^,],%[^,],%f", city, product, &price) == 3) {
            int city_idx = -1;

            // Find city index
            for (int i = 0; i < NUM_CITIES; ++i) {
                if (strcmp(moroccan_cities[i], city) == 0) {
                    city_idx = i;
                    break;
                }
            }

            if (city_idx != -1) {
                int64_t price_in_cents = price * 100;
                total_per_city[city_idx] += price_in_cents;

                // Find the index of the product
                int product_idx = -1;
                for (int i = 0; i < NUM_PRODUCTS; ++i) {
                    if (strcmp(fruits_and_vegetables[i], product) == 0) {
                        product_idx = i;
                        break;
                    }
                }

                // Update cheapest products for the city
                struct ProductPrice *cheapest = cities[city_idx].cheapest_products;
                for (int i = 0; i < 5; ++i) {
                    if (cheapest[i].price == 0 || price_in_cents < cheapest[i].price) {
                        // Shift cheaper products down
                        for (int j = 4; j > i; --j) {
                            cheapest[j] = cheapest[j - 1];
                        }
                        cheapest[i].name = fruits_and_vegetables[product_idx];
                        cheapest[i].price = price_in_cents;
                        break;
                    }
                }
            } else {
                fprintf(stderr, "Unknown city: %s\n", city);
            }
        } else {
            fprintf(stderr, "Error parsing line: %s\n", line);
        }
    }

    // Find the cheapest city
    int cheapest_city_idx = -1;
    int64_t cheapest_city_price = INT64_MAX;
    for (int i = 0; i < NUM_CITIES; ++i) {
        if (total_per_city[i] > 0 && total_per_city[i] < cheapest_city_price) {
            cheapest_city_price = total_per_city[i];
            cheapest_city_idx = i;
        }
    }

    // Print the cheapest city and its 5 cheapest products
    fprintf(output_file, "%s %.2f\n", moroccan_cities[cheapest_city_idx], (double)cheapest_city_price / 100.0);
    struct ProductPrice *cheapest = cities[cheapest_city_idx].cheapest_products;
    int printed_products = 0;
    for (int i = 0; i < MAX_PRODUCTS; ++i) {
        if (cheapest[i].name != NULL) {
            int duplicate = 0;
            for (int j = 0; j < printed_products; ++j) {
                if (strcmp(cheapest[i].name, printed_product_names[j]) == 0) {
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate) {
                fprintf(output_file, "%s %.2f\n", cheapest[i].name, (double)cheapest[i].price / 100.0);
                strncpy(printed_product_names[printed_products], cheapest[i].name, MAX_PRODUCT_NAME_LENGTH);
                printed_products++;
            }
        }
        if (printed_products == MAX_PRODUCTS) break; // Stop if we have printed 5 products
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}
