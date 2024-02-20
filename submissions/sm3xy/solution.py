from collections import defaultdict
from heapq import nsmallest

def get_cheapest_city(filename):
    city_prices = defaultdict(float)
    city_products = defaultdict(lambda: defaultdict(float))
    txt_file = open(filename)
    for line in txt_file:
        city, product, price = line.strip().split(",")
        price = float(price)
        city_prices[city] += price

        if not city_products[city][product] or price < city_products[city][product]:
            city_products[city][product] = price
    txt_file.close()

    cheapest_city = min(city_prices, key=city_prices.get)
    cheapest_price = city_prices[cheapest_city]
    cheapest_products = nsmallest(5, city_products[cheapest_city].items(), key=lambda x: (x[1], x[0]))

    return cheapest_city, cheapest_price, cheapest_products

def format_output(cheapest_city, cheapest_price, cheapest_products):
    output_lines = [f"{cheapest_city} {cheapest_price:.2f}"]
    output_lines.extend([f"{product} {price:.2f}" for product, price in cheapest_products])
    return output_lines

if __name__ == "__main__":
    input_filename = "input.txt"
    cheapest_city, cheapest_price, cheapest_products = get_cheapest_city(input_filename)
    output_lines = format_output(cheapest_city, cheapest_price, cheapest_products)
    with open("output.txt", "w") as output_file:
        output_file.write("\n".join(output_lines))
