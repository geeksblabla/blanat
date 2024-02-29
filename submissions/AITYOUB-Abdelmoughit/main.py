from time import time
from collections import defaultdict
cities_with_products = {}
start = time()
with open("../../input.txt",'r') as data:
    for line in data:
        city, product, price = line.split(",")
        price = float(price)
        if city not in cities_with_products:
            cities_with_products[city] = {}
            cities_with_products[city]["total"] = 0
            cities_with_products[city]["products"] = {}
        if product not in cities_with_products[city]["products"]:
            cities_with_products[city]["products"][product] = price
            cities_with_products[city]["total"] += price
        else:
            if cities_with_products[city]["products"][product] > price:
                difference = cities_with_products[city]["products"][product] - price
                cities_with_products[city]["total"] -= difference
                cities_with_products[city]["products"][product] = price

sorted_cities_by_total = sorted(cities_with_products.items(), key=lambda item: item[1]["total"])
cheapest_city_with_products = sorted_cities_by_total[0]
city = cheapest_city_with_products[0]
total = round(cheapest_city_with_products[1]["total"], 2)
sorted_products = sorted(cheapest_city_with_products[1]["products"].items(), key=lambda product: (product[1], product[0]))
cheapest_5_products = sorted_products[:5]
with open("../../output.txt", 'w') as result:
    result.write(f"{city} {total}\n")
    print(f"{city} {total}")
    for product, price in cheapest_5_products:
        result.write(f"{product} {price}\n")
        print(f"{product} {price}")
deltatime = time() - start
print(f"taken time: {deltatime}s")
