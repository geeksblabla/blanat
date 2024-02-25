import csv
from collections import defaultdict

"""
This script is a desperate attempt to preserve Python's dignity
even though we know it won't beat C++ nor Rust ;)
'Python hada jhdi 3lik! ¯\_(ツ)_/¯'
"""

def read_and_process_data(input_file_path):
    """
    Reads the CSV file and processes the data to determine 
    the cheapest prices per product in each city
    """
    # This dictionary is where we will be storing the cheapest prices and city totals
    city_totals = defaultdict(float)
    city_products = defaultdict(lambda: defaultdict(lambda: float('inf')))
    
    # Bismillah..
    with open(input_file_path, mode='r', newline='') as file:
        reader = csv.reader(file)
        for row in reader:
            city, product, price = row[0], row[1], float(row[2])
            # We update the city product price only if the current price is lower
            if price < city_products[city][product]:
                # Update the total only if the product price improved
                if city_products[city][product] != float('inf'):
                    city_totals[city] -= city_products[city][product] - price
                else:
                    city_totals[city] += price
                city_products[city][product] = price

    return city_totals, city_products


def find_cheapest_city(city_totals):
    """
    Determines the cheapest city based on the total price of products
    """
    return min(city_totals, key=city_totals.get), city_totals[min(city_totals, key=city_totals.get)]


def sort_products(city_products, city):
    """
    Sorts the products for a given city by price and then alphabetically,
    and return the top 5
    """
    return sorted(city_products[city].items(), key=lambda x: (x[1], x[0]))[:5]


def write_output(output_file_path, cheapest_city, cheapest_total, cheapest_products):
    """
    Writes the output data to output.txt in our case
    """
    with open(output_file_path, mode='w', newline='') as file:
        file.write(f"{cheapest_city} {cheapest_total:.2f}\n")
        for product, price in cheapest_products:
            file.write(f"{product} {price:.2f}\n")


def main(input_file_path, output_file_path):
    # Let's Go
    city_totals, city_products = read_and_process_data(input_file_path)
    cheapest_city, cheapest_total = find_cheapest_city(city_totals)
    cheapest_products = sort_products(city_products, cheapest_city)
    write_output(output_file_path, cheapest_city, cheapest_total, cheapest_products)


# Example usage
main("input_1b.txt", "output.txt")
