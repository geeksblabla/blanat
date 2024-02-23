import csv
from collections import defaultdict
import heapq
from concurrent.futures import ThreadPoolExecutor
import os

def read_input_file(filename):
    city_prices = defaultdict(float)
    products = defaultdict(float)

    with open(filename, 'r') as file:
        reader = csv.reader(file)
        next(reader)  # Skip header row

        for row in reader:
            city, product, price = row
            price = float(price)
            city_prices[city] += price
            if not products[product] or price < products[product]:
                products[product] = price

    return city_prices, products

def find_cheapest_city(city_prices):
    cheapest_city, cheapest_price = min(city_prices.items(), key=lambda x: x[1])
    return cheapest_city, cheapest_price

def find_top_5_cheapest_products(products):
    top_5_cheapest_products = heapq.nsmallest(5, products.items(), key=lambda x: x[1])
    return top_5_cheapest_products

def main():
    city_prices, products = read_input_file('inputM.txt')

    max_workers = os.cpu_count() or 1  # Default to 1 if cpu_count() returns None
    with ThreadPoolExecutor(max_workers=max_workers) as executor:
        cheapest_city_future = executor.submit(find_cheapest_city, city_prices)
        top_5_cheapest_products_future = executor.submit(find_top_5_cheapest_products, products)

        cheapest_city, cheapest_price = cheapest_city_future.result()
        top_5_cheapest_products = top_5_cheapest_products_future.result()

    with open('output.txt', 'w') as file:
        file.write(f"{cheapest_city} {cheapest_price:.2f}\n")
        for product, price in top_5_cheapest_products:
            file.write(f"{product} {price:.2f}\n")

if __name__ == '__main__':
    main()
