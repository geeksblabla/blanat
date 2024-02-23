#
# Copyright Geeksblabla 2024
# 
# Generator for sample input for the Blanat challenge from blablaconf 2024
# 
# Usage:
# python3 gen.py --rows <rows> --flush <flush>
# 
# Where:
#   rows: number of rows to generate in the test file, for a full run use 1000000000
#   flush: Minimize reach out to the filesystem, make it propotionally size to your <rows>
#
import random
import csv
import argparse

fruits_and_vegetables = [
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
]

moroccan_cities = [
    "Casablanca", "Rabat", "Marrakech", "Fes", "Tangier",
    "Agadir", "Meknes", "Oujda", "Kenitra", "Tetouan",
    "Safi", "El_Jadida", "Beni_Mellal", "Errachidia",
    "Taza", "Essaouira", "Khouribga", "Guelmim",
    "Jorf_El_Melha", "Laayoune", "Ksar_El_Kebir", "Sale", "Bir_Lehlou",
    "Arfoud", "Temara", "Mohammedia", "Settat",
    "Béni_Mellal", "Nador", "Kalaat_MGouna",
    "Chichaoua", "Chefchaouen", "Al_Hoceima", "Taourirt",
    "Taroudant", "Guelta_Zemmur", "Dakhla", "Laâyoune",
    "Tiznit","Tinghir", "Ifrane", "Azrou", "Bab_Taza",
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
]

def generate_random_decimal():
    return round(random.uniform(1, 100), 2)

def main():
    parser = argparse.ArgumentParser(description="Generator for blanat")
    # How many rows to generate
    parser.add_argument("--rows", type=int, default=1000)
    # To minimize the amount of WRITE syscalls, only flush every `flush` times
    parser.add_argument("--flush", type=int, default=10000)
    args = parser.parse_args()
  
    with open("input.txt", mode='w', newline='') as file:
        lines = ''
        for cur in range(args.rows):
            line = random.choice(moroccan_cities) + "," + random.choice(fruits_and_vegetables) + "," + str(generate_random_decimal())
            lines += line + '\n'
            if cur % args.flush == 0:
                file.write(lines)
                lines = ''
        if len(lines) > 0:
            file.write(lines)

main()
