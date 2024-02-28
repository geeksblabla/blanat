'''
Future is loading ...
Wait for my real submition this is just for testing.
By Yassineosip
'''

from collections import defaultdict
from os import path
from multiprocessing import cpu_count, Pool

def struct():
    return {"sum": 0, "cheapest": float('inf')}

def dict_struct():
    return defaultdict(struct)

def nested_struct():
    return defaultdict(dict_struct)

def split_file():
    file_size = path.getsize(file_name)
    part_size = file_size // cpu_count
    start_end = []
    with open(file_name, "r+b") as file:
        def is_nline(p):
            if p == 0:
                return True
            else:
                file.seek(p-1)
                return file.read(1) == b"\n"
        def gnl(p):
            file.seek(p)
            file.readline()
            return file.tell()
        start = 0
        while start < file_size:
            end = min(file_size, start+part_size)
            while not is_nline(end) and end < file_size:
                end-=1
            if start == end:
                end = gnl(end)
            start_end.append((start, end))
            start = end
    return start_end

def process_part(start, end):
    part_cities_products_info = nested_struct()
    with open(file_name, "r") as file:
        file.seek(start)
        while file.tell() < end:
            line = file.readline()
            if not line:
                break
            city, product, price = line.strip().split(",")
            price = float(price)
            part_cities_products_info[city][product]["sum"] += price
            if price < part_cities_products_info[city][product]["cheapest"]:
                part_cities_products_info[city][product]["cheapest"] = price
    return part_cities_products_info

def process_file():
    start_end = split_file()
    with Pool(cpu_count) as p:
        parts_data = p.starmap(process_part, start_end)
    file_cities_products_info = nested_struct()
    for part in parts_data:
        for city, products in part.items():
            for product, info in products.items():
                file_cities_products_info[city][product]["sum"] += info["sum"]
                if info["cheapest"] < file_cities_products_info[city][product]["cheapest"]:
                    file_cities_products_info[city][product]["cheapest"] = info["cheapest"]
    return file_cities_products_info

if __name__ == "__main__":
    cpu_count = cpu_count()
    file_name = "input.txt"
    cities_products_info = process_file()
    cities_total_sum = {city: sum(info["sum"] for info in products.values()) for city, products in cities_products_info.items()}
    cheapest_city = min(cities_total_sum, key=cities_total_sum.get)
    cheapest_city_price = f"{cities_total_sum[cheapest_city]:.2f}"
    cheapest_city_products = cities_products_info[cheapest_city]
    filtered_sorted_products = sorted(((product, details["cheapest"]) for product, details in cheapest_city_products.items()), key=lambda x:(x[1], x[0]))[:5]
    with open("output.txt", "w") as file:
        file.write(cheapest_city+" "+cheapest_city_price+"\n")
        for index, (product, price) in enumerate(filtered_sorted_products):
            file.write(product + " " + f"{price:.2f}")
            if index < len(filtered_sorted_products) - 1:
                file.write("\n")