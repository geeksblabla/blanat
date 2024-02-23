from collections import defaultdict


def get_city_totals(lines):
    """Calculate total price for each city"""

    city_totals = defaultdict(float)
    for line in lines:
        city, _, price = line.strip().split(",")
        city_totals[city] += float(price)
    return city_totals


def read_large_file(file_path, chunk_size=256 * 256):
    """Read input file by chunks"""

    with open(file_path, "r") as file:
        while True:
            data = file.readlines(chunk_size)
            if not data:
                break
            yield data


def main(input_path, output_path):
    """Main function"""

    city_totals = defaultdict(float)
    cheapest_city_products = defaultdict(list)
    for chunk in read_large_file(input_path):
        for line in chunk:
            city, _, price = line.strip().split(",")
            city_totals[city] += float(price)
            _, product, price = line.strip().split(",")

    cheapest_city = min(city_totals, key=city_totals.get)
    cheapest_total = city_totals[cheapest_city]
    for chunk in read_large_file(input_path):
        for line in chunk:
            if line.startswith(cheapest_city):
                _, product, price = line.strip().split(",")
                cheapest_city_products[cheapest_city].append(
                    {"product": product, "price": float(price)}
                )
    top_products = sorted(
        cheapest_city_products[cheapest_city], key=lambda x: (x["price"], x["product"])
    )[:5]

    output_lines = [f"{cheapest_city} {round(cheapest_total, 2)}"]
    output_lines.extend(
        [f"{product['product']} {product['price']}" for product in top_products]
    )

    with open(output_path, "w", encoding="utf-8") as f:
        f.write("\n".join(output_lines))


if __name__ == "__main__":
    INPUT_FILE_PATH = "input.txt"
    OUTPUT_FILE_PATH = "output.txt"
    main(INPUT_FILE_PATH, OUTPUT_FILE_PATH)
