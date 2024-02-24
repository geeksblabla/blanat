import concurrent.futures
from collections import defaultdict
import heapq
from itertools import islice

def process_file_chunk(chunk, city_prices, city_products):
    for line in chunk:
        city, product, price = line.strip().split(',')
        city_prices[city] += float(price)
        city_products[city].append((product, float(price)))

def get_cheapest_city_and_products(input_file):
    city_prices = defaultdict(int)
    city_products = defaultdict(list)

    chunk_size = 10000

    with open(input_file, 'r') as file:
        while True:
            chunk = list(islice(file, chunk_size))
            if not chunk:
                break

            with concurrent.futures.ThreadPoolExecutor() as executor:
                executor.submit(process_file_chunk, chunk, city_prices, city_products)

    cheapest_city = min(city_prices, key=city_prices.get)

    cheapest_products = sorted(city_products[cheapest_city], key=lambda x: (x[1], x[0]))

    cheapest_city_price = round(city_prices[cheapest_city], 2)

    return cheapest_city, cheapest_products[:5], cheapest_city_price

if __name__ == "__main__":
    input_file = "input.txt"
    cheapest_city, cheapest_products, cheapest_city_price = get_cheapest_city_and_products(input_file)
    
    with open("output.txt", "w") as out:
        out.write(f"{cheapest_city} {cheapest_city_price:.2f}\n")
        for product, price in cheapest_products:
            out.write(f"{product} {price:.2f}\n")
