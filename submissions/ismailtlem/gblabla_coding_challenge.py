from typing import Any


def get_cities(lines: list[str]) -> set:
    """Get all cities"""

    cities = set()
    for line in lines:
        cities.add(line.split(",")[0])
    return cities


def read_input_file(input_path: str) -> list[str]:
    """Read the input file"""

    with open(input_path, "r", encoding="utf-8") as f:
        lines = f.readlines()
    return lines


def get_city_total(city: str, lines: list[str]) -> int:
    """Get the city total"""

    city_lines = [line for line in lines if city == line.split(",")[0]]
    return sum(float(city_line.split(",")[2].strip()) for city_line in city_lines)


def find_cheapest_city(lines: list[str]) -> dict[str, Any]:
    """Find the cheapest city"""

    cities = get_cities(lines)
    cities_data = [
        {"name": city, "total": get_city_total(city, lines)} for city in cities
    ]
    return min(cities_data, key=lambda x: x["total"])


def main(input_path: str, output_path: str) -> Any:
    """Main function"""

    lines = read_input_file(input_path)
    cheapest_city = find_cheapest_city(lines)
    cheapest_city_lines = [
        {"product": line.split(",")[1], "price": line.split(",")[2].strip()}
        for line in lines
        if cheapest_city.get("name") == line.split(",")[0]
    ]
    output_lines = [
        cheapest_city.get("name") + " " + str(round(cheapest_city.get("total"), 2))
    ]
    sorted_products = sorted(
        cheapest_city_lines,
        key=lambda x: (x.get("price", 0), x.get("product")),
    )[:5]
    for product in sorted_products:
        output_lines.append(product.get("product") + " " + product.get("price"))
    with open(output_path, "w", encoding="utf-8") as f:
        f.write("\n".join(output_lines))
    return None


if __name__ == "__main__":
    INPUT_FILE_PATH = "input.txt"
    OUTPUT_FILE_PATH = "output.txt"
    main(INPUT_FILE_PATH, OUTPUT_FILE_PATH)
