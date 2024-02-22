from typing import Any
import numpy as np


def get_cities(lines: np.ndarray) -> set:
    """Get all cities"""

    cities = set()
    for line in lines:
        cities.add(line.split(",")[0])
    return cities


def read_input_file(input_path: str) -> np.ndarray:
    """Read the input file using numpy"""

    return np.loadtxt(input_path, dtype="str")


def get_city_total(city: str, lines: np.ndarray) -> int:
    """Get the city total"""

    city_lines = np.array([line for line in lines if city == line.split(",")[0]])
    return sum(float(city_line.split(",")[2].strip()) for city_line in city_lines)


def find_cheapest_city(lines: np.ndarray) -> dict[str, Any]:
    """Find the cheapest city"""

    cities = get_cities(lines)
    cities_data = np.array(
        [{"name": city, "total": get_city_total(city, lines)} for city in cities]
    )
    return min(cities_data, key=lambda x: x["total"])


def main(input_path: str, output_path: str) -> Any:
    """Main function"""

    lines = read_input_file(input_path)
    cheapest_city = find_cheapest_city(lines)
    cheapest_city_lines = np.array(
        [
            {"product": line.split(",")[1], "price": line.split(",")[2].strip()}
            for line in lines
            if cheapest_city.get("name") == line.split(",")[0]
        ]
    )
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
