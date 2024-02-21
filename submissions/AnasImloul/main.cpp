#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include <iomanip>
#include <queue>

#pragma GCC optimize("O7,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

int64_t currentTimeMillis() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

std::string morrocan_cities[101] = {
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


std::string fruits_and_vegetables[94] = {
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
        "Rutabaga", "Avocado", "Beet", "Zucchini", "Kiwi", "Salsify"};


int main() {
    // int64_t start = currentTimeMillis();

    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    freopen("input.txt", "r", stdin);

    std::unordered_map<std::string, std::unordered_map<std::string, double>> map;
    map.reserve(613);
    for (const auto& city : morrocan_cities) {
        map[city].reserve(512);
        for (const auto& product : fruits_and_vegetables)
            map[city][product] = INT_MAX;
    }

    std::unordered_map<std::string, double> index_totals;
    for (const auto& city : morrocan_cities) {
        index_totals[city] = 0;
    }

    std::string city, product;
    char c;

    int count = 0;

    while (!std::cin.eof()) {
        city.clear(), product.clear();

        while (std::cin.get(c) && c != ',') city.push_back(c);

        while (std::cin.get(c) && c != ',') product.push_back(c);

        int price = 0;
        while (std::cin.get(c) && c != '\n') {
            if (c == '.') break;
            else price = price * 10 + (c - '0');
        }

        int decimal = 0;
        if (c == '.') {
            while (std::cin.get(c) && c != '\n') {
                price = price * 10 + (c - '0');
                decimal++;
            }
        }

        if (price == 0) break;

        double price_double = price;
        if (decimal == 1) price_double /= 10;
        else if (decimal == 2) price_double /= 100;

        double& current_price = map[city][product];
        if (price_double < current_price)
            current_price = price_double;

        index_totals[city] += price_double;

        count++;
    }

    std::ofstream output("output.txt");

    // get city with the lowest price
    std::string cheapest_city;
    double cheapest_price = std::numeric_limits<double>::max();

    for (const auto& [city, total] : index_totals) {
        if (total < cheapest_price) {
            cheapest_price = total;
            cheapest_city = city;
        }
    }

    output << cheapest_city << " " << std::fixed << std::setprecision(2) << cheapest_price << std::endl;

    // get 5 products with the lowest price
    std::priority_queue<std::pair<double, std::string>> pq;
    for (const auto& [product, price] : map[cheapest_city]) {
        pq.emplace(price, product);
        if (pq.size() > 5) pq.pop();
    }

    std::vector<std::pair<double, std::string>> products;
    products.reserve(5);
    while (!pq.empty()) {
        products.push_back(pq.top());
        pq.pop();
    }

    for (int i = 4; i >= 0; i--) {
        output << products[i].second << " " << std::fixed << std::setprecision(2) << products[i].first << std::endl;
    }

    output.close();

    // int64_t end = currentTimeMillis();
    // std::cout << "Execution time: " << end - start << "ms" << std::endl;
}