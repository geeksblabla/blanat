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
    city_products = defaultdict(lambda: defaultdict(float))

    for line in read_large_file(input_path):
        city, product, price = line.split(",")
        price = float(price)
        city_totals[city] += price
        if not city_products[city][product] or price < city_products[city][product]:
            city_products[city][product] = price
    cheapest_city = min(city_totals, key=city_totals.get)
    cheapest_products = sorted(city_products[cheapest_city].items(), key=lambda x: (x[1], x[0]))[:5]
    output_lines = [f"{cheapest_city} {format_float_number(city_totals[cheapest_city])}"]
    output_lines.extend([f"{product} {format_float_number(price)}" for product, price in cheapest_products])

    with open(output_path, "w", encoding="utf-8") as f:
        f.write("\n".join(output_lines))


if __name__ == "__main__":

    INPUT_FILE_PATH = "input.txt"
    OUTPUT_FILE_PATH = "output.txt"
    main(INPUT_FILE_PATH, OUTPUT_FILE_PATH)
