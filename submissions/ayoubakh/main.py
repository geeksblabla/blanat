import csv

def read_csv(file_path):
    data = {}
    with open(file_path, mode='r') as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            city, product, price = row
            if city not in data:
                data[city] = {}
            if product not in data[city] or float(price) < float(data[city][product]):
                data[city][product] = price
    return data
    
def calculate_total_price(city_data):
    total_prices = {}
    for city, products in city_data.items():
        total_prices[city] = sum(float(price) for price in products.values())
    return total_prices

def find_cheapest_city(total_prices):
    return min(total_prices.items(), key=lambda x: x[1])

def find_cheapest_products(city_data, cheapest_city):
    products = {}
    for product, price in city_data[cheapest_city[0]].items():
        products[product] = float(price)
    sorted_products = sorted(products.items(), key=lambda x: (x[1], x[0]))[:5]
    return sorted_products

def write_output(cheapest_city, cheapest_products, output_file):
    with open(output_file, mode='w') as file:
        file.write(f"{cheapest_city[0]} {cheapest_city[1]:.2f}\n")
        for product, price in cheapest_products:
            file.write(f"{product} {price:.2f}\n")

def main(input_file, output_file):
    city_data = read_csv(input_file)
    total_prices = calculate_total_price(city_data)
    cheapest_city = find_cheapest_city(total_prices)
    cheapest_products = find_cheapest_products(city_data, cheapest_city)
    write_output(cheapest_city, cheapest_products, output_file)

if __name__ == "__main__":
    input_file = "input.txt"
    output_file = "output.txt"
    main(input_file, output_file)
