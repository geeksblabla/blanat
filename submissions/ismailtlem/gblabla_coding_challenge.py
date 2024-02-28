from collections import defaultdict


def read_large_file(file_path, chunk_size=1024 * 1024):
    """Read input file by chunks"""

    with open(file_path, "r", encoding="utf-8") as file:
        while True:
            chunk = file.readline(chunk_size)
            if not chunk:
                break
            line = chunk.strip()
            yield line


def format_float_number(number):
    """format float number to 2 decimals"""

    return "{:.2f}".format(number)


def main(input_path, output_path):
    """Main function"""

    city_totals = defaultdict(float)
    cheapest_city_products = defaultdict(set)
    cheapest_city = ""
    cheapest_total = float("inf")

    for line in read_large_file(input_path):
        city, product, price = line.split(",")
        city_totals[city] += float(price)
        if city_totals[city] < cheapest_total:
            cheapest_total = city_totals[city]
            cheapest_city = city
            cheapest_city_products.clear()
            cheapest_city_products.update({cheapest_city: set()})
        if city == cheapest_city:
            cheapest_city_products[cheapest_city].add((product, price))

    top_products = sorted(cheapest_city_products[cheapest_city], key=lambda x: (x[1], x[0]))
    lowest_prices = {}

    for product, price in top_products:
        if product not in lowest_prices or price < lowest_prices[product]:
            lowest_prices[product] = price
    unique_products = [(product, price) for product, price in top_products if price == lowest_prices[product]][:5]

    output_lines = [f"{cheapest_city} {format_float_number(city_totals[cheapest_city])}"]
    output_lines.extend([f"{product[0]} {format_float_number(float(product[1]))}" for product in unique_products])

    with open(output_path, "w", encoding="utf-8") as f:
        f.write("\n".join(output_lines))


if __name__ == "__main__":
    INPUT_FILE_PATH = "input.txt"
    OUTPUT_FILE_PATH = "output.txt"
    main(INPUT_FILE_PATH, OUTPUT_FILE_PATH)
