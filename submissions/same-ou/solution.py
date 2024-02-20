### blanat competition submission
### Author: same-ou

from collections import defaultdict
import heapq

def map(file_path):
    city_products = defaultdict(list)

    with open(file_path, 'r') as file:
        for line in file:
            city, product, price = line.strip().split(',')
            heapq.heappush(city_products[city], (float(price), product))

    return city_products

def reduce(city_products):
    cheapest_city = None
    lowest_price = float('inf')
    cheapest_products = []

    for city, products in city_products.items():
        total_price = sum(price for price, _ in products)
        if total_price < lowest_price:
            lowest_price = total_price
            cheapest_city = city
            cheapest_products = heapq.nsmallest(5, products)
    return cheapest_city, lowest_price,cheapest_products

def main(file_path):
    city_products = map(file_path)
    cheapest_city, lowest_price,cheapest_products = reduce(city_products)

    with open("output.txt", "w") as output_file:
        output_file.write(f"{cheapest_city} {lowest_price:.2f}\n")
        for price, product in cheapest_products:
            output_file.write(f"{product} {price:.2f}\n")

if __name__ == "__main__":
    main("./input.txt")