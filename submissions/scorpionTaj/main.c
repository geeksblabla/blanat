#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define NUM_CITIES 101
#define NUM_PRODUCTS 94
#define MAX_LINE_LENGTH 100

const char *moroccan_cities[NUM_CITIES] = {
    "Casablanca", "Rabat", "Marrakech", "Fes", "Tangier",
    "Agadir", "Meknes", "Oujda", "Kenitra", "Tetouan",
    "Safi", "El_Jadida", "Beni_Mellal", "Errachidia",
    "Taza", "Essaouira", "Khouribga", "Guelmim",
    "Jorf_El_Melha", "Laayoune", "Ksar_El_Kebir", "Sale", "Bir_Lehlou",
    "Arfoud", "Temara", "Mohammedia", "Settat",
    "Béni_Mellal", "Nador", "Kalaat_MGouna",
    "Chichaoua", "Chefchaouen", "Al_Hoceima", "Taourirt",
    "Taroudant", "Guelta_Zemmur", "Dakhla", "Laâyoune",
    "Tiznit", "Tinghir", "Ifrane", "Azrou", "Bab_Taza",
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
    "Imzouren", "Zemamra", "Sidi_Kacem", "Drarga", "Skhirate"
};

const char *fruits_and_vegetables[NUM_PRODUCTS] = {
    "Cauliflower", "Banana", "Lime", "Cucumber", "Bok_Choy",
    "Orange", "Garlic", "Thyme", "Cranberry", "Mango",
    "Jackfruit", "Strawberry", "Dill", "Potato", "Parsnip",
    "Pomegranate", "Blackberry", "Carrot", "Lettuce", "Dragon_Fruit",
    "Date", "Jicama", "Nectarine", "Honeydew", "Eggplant", "Celery",
    "Yam", "Kiwano", "Mint", "Spinach", "Basil", "Kale",
    "Tomato", "Pumpkin", "Coconut", "Asparagus", "Passion_Fruit",
    "Parsley", "Currant", "Peach", "Grapefruit", "Okra", "Onion",
    "Goji_Berry", "Sweet_Potato", "Ginger", "Cherry", "Squash_Blossom",
    "Artichoke", "Apricot", "Kohlrabi", "Cactus_Pear", "Cantaloupe",
    "Apple", "Guava", "Clementine", "Persimmon", "Papaya",
    "Brussels_Sprouts", "Rhubarb", "Starfruit", "Plum", "Grapes",
    "Rosemary", "Oregano", "Pear", "Bell_Pepper", "Turnip",
    "Cilantro", "Broccoli", "Peas", "Radish", "Collard_Greens",
    "Watercress", "Watermelon", "Pineapple", "Lemon", "Blueberry",
    "Cabbage", "Acorn_Squash", "Plantain", "Endive", "Raspberry",
    "Chard", "Green_Beans", "Fig", "Butternut_Squash", "Sage",
    "Rutabaga", "Avocado", "Beet", "Zucchini", "Kiwi", "Salsify"
};

int main() {
    FILE *input_file = fopen("input.txt", "r");
    FILE *output_file = fopen("output.txt", "w");

    int64_t map[NUM_CITIES][NUM_PRODUCTS];
    memset(map, 0, sizeof(map));

    int64_t total_per_city[NUM_CITIES];
    memset(total_per_city, 0, sizeof(total_per_city));

    // Hash maps for quick lookup
    int city_index[NUM_CITIES];
    int product_index[NUM_PRODUCTS];

    for (int i = 0; i < NUM_CITIES; ++i) {
        city_index[i] = -1;
        for (int j = 0; j < NUM_PRODUCTS; ++j) {
            map[i][j] = INT64_MAX;
        }
    }

    for (int i = 0; i < NUM_CITIES; ++i) {
        city_index[i] = i;
    }

    for (int i = 0; i < NUM_PRODUCTS; ++i) {
        product_index[i] = i;
    }

    char line[MAX_LINE_LENGTH];
    char city[100], product[100];
    float price;

    while (fgets(line, sizeof(line), input_file)) {
        sscanf(line, "%[^,],%[^,],%f", city, product, &price);

        int city_idx = city_index[atoi(city)];
        int product_idx = product_index[atoi(product)];

        int64_t price_in_cents = price * 100;

        if (price_in_cents < map[city_idx][product_idx]) {
            map[city_idx][product_idx] = price_in_cents;
        }
        total_per_city[city_idx] += price_in_cents;
    }

    // Find the city with the lowest total price
    int cheapest_city_index = -1;
    int64_t cheapest_total = INT64_MAX;

    for (int i = 0; i < NUM_CITIES; ++i) {
        if (total_per_city[i] > 0 && total_per_city[i] < cheapest_total) {
            cheapest_total = total_per_city[i];
            cheapest_city_index = i;
        }
    }

    fprintf(output_file, "%s %.2f\n", moroccan_cities[cheapest_city_index], (double)cheapest_total / 100.0);

    // Find 5 products with the lowest price in the cheapest city
    struct ProductPrice {
        int64_t price;
        int product_index;
    };

    struct ProductPrice products[NUM_PRODUCTS];

    for (int i = 0; i < NUM_PRODUCTS; ++i) {
        products[i].price = map[cheapest_city_index][i];
        products[i].product_index = i;
    }

    // Sort the products by price (using bubble sort for simplicity)
    for (int i = 0; i < NUM_PRODUCTS - 1; ++i) {
        for (int j = 0; j < NUM_PRODUCTS - i - 1; ++j) {
            if (products[j].price > products[j + 1].price) {
                struct ProductPrice temp = products[j];
                products[j] = products[j + 1];
                products[j + 1] = temp;
            }
        }
    }

    // Print the 5 products with the lowest price to the output file
    for (int i = 0; i < 5; ++i) {
        fprintf(output_file, "%s %.2f\n", fruits_and_vegetables[products[i].product_index], (double)products[i].price / 100.0);
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}
