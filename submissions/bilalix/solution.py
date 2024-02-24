import csv
from collections import defaultdict
import time

"""
This script is a desperate attempt to preserve Python's dignity
even though we know it won't beat C++ nor Rust ;)
"""

def find_cheapest_city(input_file_path, output_file_path):
    start_time = time.time()

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
    
    # Let's find the cheapest city
    cheapest_city = min(city_totals, key=city_totals.get)
    cheapest_total = city_totals[cheapest_city]
    
    # and sort the products by price and then alphabetically taking the first 5
    cheapest_products = sorted(city_products[cheapest_city].items(), key=lambda x: (x[1], x[0]))[:5]
    
    # And finally write the output file
    with open(output_file_path, mode='w', newline='') as file:
        file.write(f"{cheapest_city} {cheapest_total:.2f}\n")
        for product, price in cheapest_products:
            file.write(f"{product} {price:.2f}\n")

    end_time = time.time()
    execution_time_minutes = (end_time - start_time) / 60
    print(f"Execution time: {execution_time_minutes:.2f} minutes")

# Example usage
find_cheapest_city("input_1m.txt", "output.txt")
