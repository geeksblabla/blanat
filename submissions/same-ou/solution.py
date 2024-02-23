### blanat competition submission
### Author: same-ou
from collections import defaultdict
import multiprocessing
from heapq import nsmallest
import threading
import queue

def get_chunks_offsets(filename, chunks_number):
    offsets = [0]
    with open(filename, 'r') as file:
        file_size = file.seek(0, 2)
        file.seek(0) # return to the begining of the file
        chunk_size = file_size // chunks_number
        
        for _ in range(chunks_number - 1):
            seek_position = offsets[-1] + chunk_size
            file.seek(seek_position)
            
            while file.tell() < file_size:
                char = file.read(1)
                if char =='\n':
                    offsets.append(file.tell())
                    break
            
    return file_size,offsets

def process_chunk(filename, start, end):
    city_prices = defaultdict(float)
    city_products = defaultdict(lambda: defaultdict(float))
          
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

def process_chunk_and_put_result(filename, start, end, result_queue):
    result = process_chunk(filename, start, end)
    result_queue.put(result)

def find_cheapest_city(merged_results):
    cheapest_city = min(merged_results, key=lambda city: merged_results[city]['total_price'])
    cheapest_price = merged_results[cheapest_city]['total_price']
    cheapest_products = nsmallest(5, merged_results[cheapest_city]['products'].items(), key=lambda x:  (x[1], x[0]))
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

def main():
    filename = 'input.txt'
    num_threads = multiprocessing.cpu_count()
    file_size, offsets = get_chunks_offsets(filename, num_threads)
    result_queue = queue.Queue()
    threads = []
    for i in range(num_threads):  
        start = offsets[i]
        end = offsets[i + 1] if i < num_threads - 1 else file_size
        
        t = threading.Thread(target=process_chunk_and_put_result, args=(filename, start, end,result_queue))
        threads.append(t)
        t.start()

    # Wait for all threads to finish
    for t in threads:
        t.join()

    # Retrieve results from the queue
    final_result = {}
    while not result_queue.empty():
        merge_dicts(final_result,result_queue.get())
    cheapest_city, lowest_price, cheapest_products = find_cheapest_city(final_result)

    with open('output.txt', 'w') as file:
        file.write(f"{cheapest_city} {lowest_price}\n")
        for product, price in cheapest_products:
            file.write(f"{product} {price}\n")

if __name__ == "__main__":
    main()
