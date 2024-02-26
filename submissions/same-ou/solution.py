from collections import defaultdict
from heapq import nsmallest
import multiprocessing
import os

def get_chunks_offsets(filename, chunks_number):
    offsets = []
    start = 0
    with open(filename, 'r') as file:
        file_size = file.seek(0, 2)
        file.seek(0) # return to the begining of the file
        chunk_size = file_size // chunks_number
        
        for _ in range(chunks_number - 1):
            seek_position = start + chunk_size
            file.seek(seek_position)
            
            while file.tell() < file_size:
                char = file.read(1)
                if char =='\n':
                    next_offset = file.tell()
                    offsets.append((start,next_offset))
                    start = next_offset
                    break
    offsets.append((offsets[-1][1], file_size))        
    return offsets

def process_chunk(filename, start, end):
    city_prices = defaultdict(float)
    city_products = defaultdict(lambda: defaultdict(float))
    print('{} reading {}'.format(multiprocessing.current_process().name, filename))      
    with open(filename, 'r') as file:
        file.seek(start)
        chunk = file.read(end - start)
        lines = chunk.split('\n')
        for line in lines:

            line = line.strip()
            if line.count(",") != 2:
                continue
            city, product, price = line.strip().split(",")
            try:
                price = float(price)
            except ValueError:
                continue
            city_prices[city] += price

            if not city_products[city][product] or price < city_products[city][product]:
                city_products[city][product] = price


    result = defaultdict(dict)
    for city, products in city_products.items():
        result[city]['products'] = products
        result[city]['total_price'] = city_prices[city]

    return dict(result)

def map_function(start, end):
    return process_chunk('input.txt', start, end) 

def find_cheapest_city(merged_results):
    cheapest_city = min(merged_results, key=lambda city: merged_results[city]['total_price'])
    cheapest_price = merged_results[cheapest_city]['total_price']
    cheapest_products = nsmallest(5, merged_results[cheapest_city]['products'].items(), key=lambda x: (x[1], x[0]))
    return cheapest_city, cheapest_price, cheapest_products

def merge_dicts(prev, new):
    for city, infos in new.items():
        if city not in prev:
            prev[city] = infos
        else :
            for product, price in infos['products'].items():
                if product not in prev[city]['products'] or price < prev[city]['products'][product]:   
                    prev[city]['products'][product] = price
            prev[city]['total_price'] += new[city]['total_price']

if __name__ == '__main__':
    filename = 'input.txt'
    num_processes = len(os.sched_getaffinity(0))
    offsets = get_chunks_offsets(filename, num_processes)
    pool = multiprocessing.Pool(num_processes)
    map_response = pool.starmap(map_function, offsets)
    final_result = {}
    for result in map_response:
        merge_dicts(final_result,result)
    cheapest_city, lowest_price, cheapest_products = find_cheapest_city(final_result)
    
    
    with open('output.txt', 'w') as file:
        file.write(f"{cheapest_city} {lowest_price:.2f}\n")
        for product, price in cheapest_products:
            file.write(f"{product} {price}\n")
