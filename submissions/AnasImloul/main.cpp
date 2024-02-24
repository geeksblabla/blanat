/*
Author: @AnasImloul
compile: g++ -std=c++17 -Ofast -o main main.cpp
*/

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <cstdint>

using namespace std;

string moroccan_cities[101] = {
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


string fruits_and_vegetables[94] = {
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
    cin.tie(nullptr);
    cout.tie(nullptr);
    ios_base::sync_with_stdio(false);

    freopen("input.txt", "r", stdin);

    // map to store the prices of each product in each city
    unordered_map<string, unordered_map<string, int64_t>> map;
    map.reserve(613);
    for (const auto& city : moroccan_cities) {
        map[city].reserve(512);
        for (const auto& product : fruits_and_vegetables)
            map[city][product] = INT64_MAX;
    }

    // map to store the total price of each city (sum of all products)
    unordered_map<string, int64_t> total_per_city;
    total_per_city.reserve(613);
    for (const auto& city : moroccan_cities) {
        total_per_city[city] = 0;
    }

    ofstream output("output.txt");

    string city, product;
    char c;
    while (!cin.eof()) {
        city.clear(), product.clear();

        while (cin.get(c) && c != ',') city.push_back(c);

        while (cin.get(c) && c != ',') product.push_back(c);

        int price = 0;
        while (cin.get(c) && c != '\n') {
            if (c == '.') break;
            else price = price * 10 + (c - '0');
        }

        int decimal = 0;
        if (c == '.') {
            while (cin.get(c) && c != '\n') {
                price = price * 10 + (c - '0');
                decimal++;
            }
        }

        if (price == 0) break;

        if (decimal == 0) price *= 100;
        else if (decimal == 1) price *= 10;

        if (price < 1000) {
            int64_t &current_price = map[city][product];
            if (current_price == 0 || price < current_price) current_price = price;
        }
        total_per_city[city] += price;
    }

    // get city with the lowest price
    pair<int64_t, string> cheapest = {INT64_MAX, ""};
    for (const auto& [city, total] : total_per_city) {
        if (total == 0) continue;
        cheapest = min(cheapest, {total, city});
    }

    output << fixed << setprecision(2);

    // print the city with the lowest price
    output << cheapest.second << " " << (double)cheapest.first / 100. << endl;

    // get 5 products with the lowest price
    priority_queue<pair<int64_t , string>> pq;
    for (const auto& [product, price] : map[cheapest.second]) {
        pq.emplace(price, product);
        if (pq.size() > 5) pq.pop();
    }

    // get the products in ascending order
    vector<pair<int64_t , string>> products(pq.size());
    for (int i = (int)pq.size() - 1; i >= 0; i--, pq.pop()) {
        products[i] = pq.top();
    }

    // print the products
    for (const auto& [price, product] : products) {
        output << product << " " << (double)price / 100. << endl;
    }

    output.close();
}