from collections import defaultdict

def main():
    input_data = "input.txt"
    output_data = "output.txt"

    data = []
    with open(input_data, 'r') as file:
        for line in file:
            content = line.strip().split(',')
            data.append({
                "City": content[0],
                "Product": content[1],
                "Price": float(content[2])
            })

    aggregated_cities_total = defaultdict(float)
    for entry in data:
        key = entry["City"]
        value = entry["Price"]
        aggregated_cities_total[key] += value

    cheapest_city_name = min(aggregated_cities_total, key=aggregated_cities_total.get)
    cheapest_city_entries = [entry for entry in data if entry["City"] == cheapest_city_name ]
    sorted_cheapest_products = []
    unique_products = set()
    for entry in sorted(cheapest_city_entries, key=lambda x: (x["Price"], x["Product"])):
        if entry["Product"] not in unique_products:
            unique_products.add(entry["Product"])
            sorted_cheapest_products.append(entry)
        if len(unique_products) == 5:
            break

    with open(output_data, 'w') as output_file:
        output_file.write(f"{cheapest_city_name } {aggregated_cities_total[cheapest_city_name ]:.2f}\n")

        for entry in sorted_cheapest_products:
            output_file.write(f"{entry['Product']} {entry['Price']:.2f}\n")

if __name__ == "__main__":
    main()
