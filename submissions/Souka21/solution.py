import csv

MAX_PRICE = 100.00
NUM_CHEAPEST_PRODUCTS = 5

def process_city_data(filename):
    """Process input CSV file to extract city data.

    Args:
        filename (str): The name of the input CSV file.

    Returns:
        dict: A dictionary containing city data indexed by city names.
            Each city entry contains:
                'total_price': Total price of all products in the city.
                'cheapest_products': List of tuples containing the names and prices of the cheapest products.
    """
    cities = {}

    with open(filename, 'r') as file:
        reader = csv.reader(file)
        next(reader)  # Skip header row
        for row in reader:
            city, product, price = row[0], row[1], float(row[2])
            if city not in cities:
                cities[city] = {'total_price': 0, 'cheapest_products': []}
            cities[city]['total_price'] += price
            cities[city]['cheapest_products'].append((product, price))

    return cities

def find_cheapest_city(cities):
    """Find the city with the lowest total price.

    Args:
        cities (dict): A dictionary containing city data indexed by city names.

    Returns:
        str: The name of the city with the lowest total price.
    """
    min_city = min(cities, key=lambda city: cities[city]['total_price'])
    return min_city

def find_cheapest_products(cities, min_city):
    """Find the cheapest products in a given city.

    Args:
        cities (dict): A dictionary containing city data indexed by city names.
        min_city (str): The name of the city for which to find the cheapest products.

    Returns:
        list: A list of tuples containing the names and prices of the cheapest products.
    """
    min_products = cities[min_city]['cheapest_products']
    min_products.sort(key=lambda x: x[1])  # Sort cheapest products by price
    return min_products[:NUM_CHEAPEST_PRODUCTS]

def write_output(filename, min_city, min_products):
    """Write the output to a file.

    Args:
        filename (str): The name of the output file.
        min_city (str): The name of the city with the lowest total price.
        min_products (list): A list of tuples containing the names and prices of the cheapest products.
    """
    with open(filename, 'w') as file:
        file.write(f"{min_city} {sum(product[1] for product in min_products):.2f}\n")
        for product in min_products:
            file.write(f"{product[0]} {product[1]:.2f}\n")

def main():
    """Main function to execute the program."""
    input_filename = 'input.txt'
    output_filename = 'output.txt'

    cities = {}
    with open(input_filename, 'r') as file:
        reader = csv.reader(file)
        next(reader)  # Skip header row
        for row in reader:
            city, product, price = row[0], row[1], float(row[2])
            if city not in cities:
                cities[city] = {'total_price': 0, 'cheapest_products': []}
            cities[city]['total_price'] += price
            cities[city]['cheapest_products'].append((product, price))

    min_city = min(cities, key=lambda city: cities[city]['total_price'])
    min_products = cities[min_city]['cheapest_products']
    min_products.sort(key=lambda x: x[1])  # Sort cheapest products by price
    min_products = min_products[:NUM_CHEAPEST_PRODUCTS]

    write_output(output_filename, min_city, min_products)

if __name__ == "__main__":
    main()

