from collections import defaultdict


def get_city_totals(lines):
    """Calculate total price for each city"""

    city_totals = defaultdict(float)
    for line in lines:
        city, _, price = line.strip().split(",")
        city_totals[city] += float(price)
    return city_totals


def find_cheapest_city(lines):
    """Find the city with the lowest total price"""

    city_totals = get_city_totals(lines)
    cheapest_city_name = min(city_totals, key=city_totals.get)
    return cheapest_city_name, city_totals.get(cheapest_city_name)


def get_top_n_products(city, lines, n=5):
    """Get the top n cheapest products for a city"""

    city_products = []
    for line in lines:
        if line.startswith(city):
            _, product, price = line.strip().split(",")
            city_products.append({"product": product, "price": float(price)})
    return sorted(
        city_products,
        key=lambda x: (x.get("price", 0), x.get("product")),
    )[:n]


def main(input_path, output_path):
    """Main function"""

    with open(input_path, "r", encoding="utf-8") as f:
        lines = f.readlines()

    cheapest_city, cheapest_total = find_cheapest_city(lines)
    top_products = get_top_n_products(cheapest_city, lines)

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
