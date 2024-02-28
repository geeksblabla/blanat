#include <bits/stdc++.h>

using namespace std;

// This will be used for mapping strings to integers since there wont'be any other cities
// or products apart from those
vector<string> productsVector = {
    "Apple", "Banana", "Orange", "Strawberry", "Grapes",
    "Watermelon", "Pineapple", "Mango", "Kiwi", "Peach",
    "Plum", "Cherry", "Pear", "Blueberry", "Raspberry",
    "Blackberry", "Cantaloupe", "Honeydew", "Coconut", "Pomegranate",
    "Lemon", "Lime", "Grapefruit", "Avocado", "Papaya",
    "Guava", "Fig", "Passion_Fruit", "Apricot", "Nectarine",
    "Cucumber", "Carrot", "Broccoli", "Spinach", "Kale",
    "Lettuce", "Tomato", "Bell_Pepper", "Zucchini", "Eggplant",
    "Cabbage", "Cauliflower", "Brussels_Sprouts", "Radish", "Beet",
    "Asparagus", "Artichoke", "Green_Beans", "Peas", "Celery",
    "Onion", "Garlic", "Potato", "Sweet_Potato", "Yam",
    "Butternut_Squash", "Acorn_Squash", "Pumpkin", "Cranberry", "Goji_Berry",
    "Currant", "Date", "Clementine", "Cranberry", "Rhubarb",
    "Chard", "Collard_Greens", "Parsley", "Cilantro", "Mint",
    "Basil", "Thyme", "Rosemary", "Sage", "Dill",
    "Oregano", "Cantaloupe", "Honeydew", "Coconut", "Pomegranate",
    "Jackfruit", "Starfruit", "Persimmon", "Ginger", "Turnip",
    "Jicama", "Kohlrabi", "Watercress", "Okra", "Artichoke",
    "Plantain", "Cactus_Pear", "Kiwano", "Squash_Blossom", "Dragon_Fruit",
    "Parsnip", "Rutabaga", "Salsify", "Bok_Choy", "Endive"
};
vector<string> citiesVector = {
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
unordered_map<string, int> products;
unordered_map<string, int> cities;
const int numOfCities = 101, numOfProducts = 100;

// This vars will be used in the readinga dn multi-threading process
const string input = "input.txt";
const string output = "output.txt";
string line;
const int numThreads = thread::hardware_concurrency(); // Number of hardware threads
vector<thread> threads;
mutex mtx;

// This vars to solve the problem
char city[100],product[100];
double price, minVal = numeric_limits<double>::max();
int minKey;
double citiesProducts[numOfCities][numOfProducts];
double sums[numOfCities];
multiset<pair<double, string>> ans;

void preprocessing(){
    for (int i = 0; i < numOfProducts; ++i) {
        products[productsVector[i]] = i;
    }
    for (int i = 0; i < numOfCities; ++i) {
        cities[citiesVector[i]] = i;
    }
}

void readFromFile(int threadId) {
    ifstream inputFile(input);
    int lineNumber = 0;
    mtx.lock();
    while (getline(inputFile,line)) {
        if (lineNumber++ % numThreads != threadId) // Distribute lines among threads
            continue;
        if (sscanf(line.c_str() ,"%[^,],%[^,],%lf", city, product, &price) == 3) {
           // printf("[Read] %s, %s, %lf\n", city, product, price);
            if(citiesProducts[cities[city]][products[product]] > price || citiesProducts[cities[city]][products[product]] == 0){
                citiesProducts[cities[city]][products[product]] =  price;
            }
            sums[cities[city]] += price;
        }
    }
    mtx.unlock();
}

void solve(){
    for(int i = 0; i < numOfCities; ++i){
        if(sums[i] != 0 && sums[i] < minVal){
            minVal = sums[i];
            minKey = i;
        }
    }
    printf("%s %.2lf\n",citiesVector[minKey].c_str(), sums[minKey]);
    for(int i = 0; i < numOfProducts; ++i){
        if(citiesProducts[minKey][i] != 0 ){
            ans.insert({citiesProducts[minKey][i], productsVector[i]});
        }
    }
    int i = 0;
    for (const auto& p : ans) {
        printf("%s %.2lf\n",p.second.c_str(), p.first);
        if (++i >= 5)
            break;
    }
}

int main() {
    preprocessing();
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(readFromFile, i);
    }
    for (auto& t : threads) {
        t.join();
    }
    solve();
    return 0;
}