from multiprocessing import Pool, cpu_count

fruits_and_vegetables = [
    'Apple', 'Banana', 'Orange', 'Strawberry', 'Grapes',
    'Watermelon', 'Pineapple', 'Mango', 'Kiwi', 'Peach',
    'Plum', 'Cherry', 'Pear', 'Blueberry', 'Raspberry',
    'Blackberry', 'Cantaloupe', 'Honeydew', 'Coconut', 'Pomegranate',
    'Lemon', 'Lime', 'Grapefruit', 'Avocado', 'Papaya',
    'Guava', 'Fig', 'Passion_Fruit', 'Apricot', 'Nectarine',
    'Cucumber', 'Carrot', 'Broccoli', 'Spinach', 'Kale',
    'Lettuce', 'Tomato', 'Bell_Pepper', 'Zucchini', 'Eggplant',
    'Cabbage', 'Cauliflower', 'Brussels_Sprouts', 'Radish', 'Beet',
    'Asparagus', 'Artichoke', 'Green_Beans', 'Peas', 'Celery',
    'Onion', 'Garlic', 'Potato', 'Sweet_Potato', 'Yam',
    'Butternut_Squash', 'Acorn_Squash', 'Pumpkin', 'Cranberry', 'Goji_Berry',
    'Currant', 'Date', 'Clementine', 'Cranberry', 'Rhubarb',
    'Chard', 'Collard_Greens', 'Parsley', 'Cilantro', 'Mint',
    'Basil', 'Thyme', 'Rosemary', 'Sage', 'Dill',
    'Oregano', 'Cantaloupe', 'Honeydew', 'Coconut', 'Pomegranate',
    'Jackfruit', 'Starfruit', 'Persimmon', 'Ginger', 'Turnip',
    'Jicama', 'Kohlrabi', 'Watercress', 'Okra', 'Artichoke',
    'Plantain', 'Cactus_Pear', 'Kiwano', 'Squash_Blossom', 'Dragon_Fruit',
    'Parsnip', 'Rutabaga', 'Salsify', 'Bok_Choy', 'Endive'
]

moroccan_cities = [
    'Casablanca', 'Rabat', 'Marrakech', 'Fes', 'Tangier',
    'Agadir', 'Meknes', 'Oujda', 'Kenitra', 'Tetouan',
    'Safi', 'El_Jadida', 'Beni_Mellal', 'Errachidia',
    'Taza', 'Essaouira', 'Khouribga', 'Guelmim',
    'Jorf_El_Melha', 'Laayoune', 'Ksar_El_Kebir', 'Sale', 'Bir_Lehlou',
    'Arfoud', 'Temara', 'Mohammedia', 'Settat',
    'Béni_Mellal', 'Nador', 'Kalaat_MGouna',
    'Chichaoua', 'Chefchaouen', 'Al_Hoceima', 'Taourirt',
    'Taroudant', 'Guelta_Zemmur', 'Dakhla', 'Laâyoune',
    'Tiznit','Tinghir', 'Ifrane', 'Azrou', 'Bab_Taza',
    'Berrechid', 'Sidi_Slimane', 'Souk_Larbaa', 'Tiflet', 'Sidi_Bennour',
    'Larache', 'Tan-Tan', 'Sidi_Ifni', 'Goulmima',
    'Midelt', 'Figuig', 'Azilal', 'Jerada', 'Youssoufia',
    'Ksar_es_Seghir', 'Tichka', 'Ait_Melloul',
    'Layoune', 'Ben_guerir', 'Ouarzazate', 'Inezgane',
    'Oujda_Angad', 'Sefrou', 'Aourir',
    'Oulad_Teima', 'Tichla', 'Bni_Hadifa',
    'Fquih_Ben_Salah', 'Guercif', 'Bouarfa', 'Demnate',
    'Ahfir', 'Berkane', 'Akhfenir', 'Boulemane',
    'Khenifra', 'Bir_Anzerane', 'Assa', 'Smara', 'Boujdour',
    'Tarfaya', 'Ouazzane', 'Zagora', 'had_soualem',
    'Saidia', 'Bab_Berred', 'Midar', 'Moulay_Bousselham',
    'Khemisset', 'Guerguerat', 'Asilah', 'Sidi_Bouzid', 'Tafraout',
    'Imzouren', 'Zemamra', 'Sidi_Kacem', 'Drarga', 'Skhirate'
]

def find_cheapest_city(cities):
    return min(cities, key=cities.get)

def create_empty_cities_dict():
    return ({city: 0 for city in moroccan_cities},
            {city: {product: float('inf') for product in fruits_and_vegetables} for city in moroccan_cities})

            
def process_chunk(chunk):
    cities, cities_products = create_empty_cities_dict()
    for line in chunk.splitlines():
        city,product,price=line.split(',')
        if (city_products:=cities_products[city])[product]>(priceFloat:=float(price)):city_products[product]=priceFloat
        cities[city] += priceFloat
    return cities, cities_products


def read_in_chunks(file_path, chunk_size=1024*1024*30):
    with open(file_path, 'r') as file:
        chunk = file.read(chunk_size)
        leftover = ''
        while chunk:
            last_newline = chunk.rfind('\n')
            if last_newline != -1:
                complete_chunk = leftover + chunk[:last_newline]
                leftover = chunk[last_newline+1:]
                yield complete_chunk
            else:leftover += chunk
            chunk = file.read(chunk_size)
        if leftover:yield leftover

def merge_dicts(main_dict, new_data):
    cities_data = new_data[0]
    products_data = new_data[1]
    main_city_data = main_dict[0]
    main_product_data = main_dict[1]
    
    for city, total in cities_data.items():main_city_data[city] += total
    for city, products in products_data.items():
        for product, price in products.items():
            if (tmp_product := main_product_data[city])[product]>price:tmp_product[product]=price
    return main_city_data, main_product_data

def parallel_process(file_path):
    def cb(result):
        nonlocal final_cities, final_products
        final_cities, final_products = merge_dicts((final_cities, final_products), result)
    pool = Pool(cpu_count())
    final_cities, final_products = create_empty_cities_dict()
    for chunk in read_in_chunks(file_path):
        pool.apply_async(process_chunk, args=(chunk,), callback=cb, error_callback=print)
    pool.close()
    pool.join()
    return final_cities, final_products

if __name__ == '__main__':
    cities, cities_products = parallel_process("input.txt")
    cheapest_city = find_cheapest_city(cities)
    five_cheapest_products = sorted(cities_products[cheapest_city].items(), key=lambda item: (item[1], item[0]))[:5]
    f = open('output.txt', 'w')
    # print(f"{cheapest_city} {cities[cheapest_city]:.2f}")
    f.write(f"{cheapest_city} {cities[cheapest_city]:.2f}\n")
    for product, price in five_cheapest_products:
        # print(f"{product} {price:.2f}")
        f.write(f"{product} {price:.2f}\n")