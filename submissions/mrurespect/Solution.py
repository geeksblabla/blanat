from collections import defaultdict
import heapq

def process_input_file(input_file):
    city_prices = defaultdict(float)

    with open(input_file, 'r') as file:
        for line in file:
            city, _, price = line.strip().split(',')
            city_prices[city] += float(price)

    return city_prices

def find_cheapest_city(city_prices):
    cheapest_city, total_price = min(city_prices.items(), key=lambda x: x[1])
    return cheapest_city, total_price

def find_cheapest_products(input_file, cheapest_city):
    products = defaultdict(list)

    with open(input_file, 'r') as file:
        for line in file:
            city, product, price = line.strip().split(',')
            if city == cheapest_city:
                heapq.heappush(products[product], float(price))

    sorted_products = sorted([(product, heapq.heappop(prices)) for product, prices in products.items()],
                             key=lambda x: x[1])

    return sorted_products[:5]

def write_output_file(cheapest_city, total_price, cheapest_products, output_file):
    with open(output_file, 'w') as file:
        file.write(f"{cheapest_city} {total_price:.2f}\n")
        for product, price in cheapest_products:
            file.write(f"{product} {price:.2f}\n")

def main(input_file, output_file):
    city_prices = process_input_file(input_file)
    cheapest_city, total_price = find_cheapest_city(city_prices)
    cheapest_products = find_cheapest_products(input_file, cheapest_city)
    write_output_file(cheapest_city, total_price, cheapest_products, output_file)

if __name__ == "__main__":
    main("input.txt", "output.txt")
