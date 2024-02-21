from collections import defaultdict

def process_input(input_file):
    city_prices = defaultdict(float)
    city_products = defaultdict(list)

    with open(input_file, 'r') as file:
        for line in file:
            city, product, price_str = line.strip().split(",")
            price = float(price_str)
            city_prices[city] += price
            city_products[city].append((price, product))

    return city_prices, city_products

def find_cheapest(city_prices, city_products):
    cheapest_city = min(city_prices, key=city_prices.get)
    cheapest_price = city_prices[cheapest_city]
    cheapest_products = sorted(city_products[cheapest_city])[:5]

    return cheapest_city, cheapest_products, cheapest_price

def write_output(output_file, cheapest_city, cheapest_products, cheapest_price):
    lines = [f"{cheapest_city} {cheapest_price:.2f}"] + [f"{product} {price:.2f}" for price, product in cheapest_products]

    with open(output_file, "w") as output:
        output.write("\n".join(lines))

if __name__ == "__main__":
    input_file = "input.txt"
    output_file = "output.txt"

    city_prices, city_products = process_input(input_file)
    cheapest_city, cheapest_products, cheapest_price = find_cheapest(city_prices, city_products)
    write_output(output_file, cheapest_city, cheapest_products, cheapest_price)
