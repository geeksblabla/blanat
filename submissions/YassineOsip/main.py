'''
Code Sghiwr O Mtiwr (Bdal ki bghiti fl input you'll get a correct output f** Hard coded soutions men lakher)...
By Yassineosip
'''

from collections import defaultdict
from os import path
from multiprocessing import Pool
from heapq import nsmallest

def struct():
    return {"sum":0, "cheapest":float('inf')}

def dict_struct():
    return defaultdict(struct)

def nested_struct():
    return defaultdict(dict_struct)

def split_file():
    file_size=path.getsize(file_name)
    part_size=file_size//cpu_count
    start_end=[]
    with open(file_name, "rb") as file:
        def is_nline(p):
            if p==0:
                return True
            else:
                file.seek(p-1)
                return file.read(1)==b"\n"
        def gnl(p):
            file.seek(p)
            file.readline()
            return file.tell()
        start=0
        while start<file_size:
            end=min(file_size, start+part_size)
            while not is_nline(end) and end<file_size:
                end-=1
            if start==end:
                end=gnl(end)
            start_end.append((start, end))
            start=end
    return start_end

def process_part(start, end):
    part_cities_products_info=nested_struct()
    with open(file_name, "rb") as file: 
        file.seek(start)
        for line in file:
            if file.tell()>=end: 
                break
            city, product, price=line.decode('utf-8').strip().split(",")
            price=float(price)
            part_cities_products_info[city][product]["sum"]+=price
            if price<part_cities_products_info[city][product]["cheapest"]:
                part_cities_products_info[city][product]["cheapest"]=price
    return part_cities_products_info

def process_file():
    start_end=split_file()
    with Pool(cpu_count) as p:
        parts_data=p.starmap(process_part, start_end)
    file_cities_products_info=nested_struct()
    for part in parts_data:
        for city, products in part.items():
            for product, info in products.items():
                file_cities_products_info[city][product]["sum"]+=info["sum"]
                if info["cheapest"]<file_cities_products_info[city][product]["cheapest"]:
                    file_cities_products_info[city][product]["cheapest"]=info["cheapest"]
    return file_cities_products_info

if __name__=="__main__":
    cpu_count=8
    file_name="input.txt"
    cheapest_city=None
    cheapest_city_price=float('inf')
    cities_products_info=process_file()
    for city, products in cities_products_info.items():
       city_total_sum=sum(info["sum"] for info in products.values())
       if city_total_sum<cheapest_city_price:
           cheapest_city_price=city_total_sum
           cheapest_city=city
       cheapest_city_products=cities_products_info[cheapest_city]
    top_cheapest_products=nsmallest(5, cheapest_city_products.items(), key=lambda item:(item[1]['cheapest'], item[0]))
    len_top_cheapest_products=len(top_cheapest_products)
    top_cheapest_products_output=[f"{cheapest_city} {cheapest_city_price:.2f}", *(f"{p} {d['cheapest']:.2f}" for p, d in top_cheapest_products)]
    with open("output.txt", "w") as file:
        file.write("\n".join(top_cheapest_products_output))