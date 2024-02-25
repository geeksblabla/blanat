import os
import multiprocessing as mp
import mmap
import collections
import threading


def find_end(mm, offset):
    while True:
        mm.seek(offset)
        if mm.read(1) == b"\n":
            return offset + 1
        offset += 1


def worker(start, end, filename, result_queue):
    with open(filename, "r+b") as f:
        mm = mmap.mmap(f.fileno(), 0)
        if start > 0:
            start = find_end(mm, start)

        end = find_end(mm, end) if end < mm.size() else end

        mm.seek(start)
        chunk = mm.read(end - start)

        example = collections.defaultdict(default_value_factory)
        for row in chunk.rstrip(b"\n").split(b"\n"):
            city, product, price = row.split(b",")
            price = float(price)

            city_data = example[city]

            if not city_data[product] or price < city_data[product]:
                city_data[product] = price

            city_data["total"] += price

        result_queue.put(example)
        mm.close()


# I need this since multiprocessing cant pickle lambda functions
def default_value_factory():
    return collections.defaultdict(float)


def queue_consumer(result_queue, results):
    while True:
        item = result_queue.get()
        if item is None:  # Stop thread when None is received
            break
        results.append(item)


def main():
    filename = "input.txt"
    file_size = os.path.getsize(filename)

    cpu_count = mp.cpu_count()
    chunk_size = file_size // cpu_count

    processes = []
    result_queue = mp.Queue()
    results = []

    # Start the consumer thread
    consumer_thread = threading.Thread(
        target=queue_consumer, args=(result_queue, results))
    consumer_thread.start()

    for i in range(cpu_count):
        start = i * chunk_size
        end = start + chunk_size if i < cpu_count - 1 else file_size
        p = mp.Process(target=worker, args=(
            start, end, filename, result_queue))
        processes.append(p)
        p.start()

    for p in processes:
        p.join()

    # Signal the consumer thread to terminate
    result_queue.put(None)
    consumer_thread.join()

    # Combine results from individual chunks
    final_example = {}
    for chunk_result in results:
        for key, value in chunk_result.items():
            if key not in final_example:
                final_example[key] = value
            else:
                final_example[key]["total"] += value.pop("total")
                for k, v in value.items():
                    if final_example[key].get(k) is None:
                        final_example[key][k] = v
                    else:
                        final_example[key][k] = min(final_example[key][k], v)

    post_processing(final_example)


def post_processing(example: dict):
    min_key = min(example, key=lambda k: example[k]['total'])
    answer = {
        "city": min_key,
        "total": example[min_key]['total']
    }
    del example[min_key]['total']
    sorted_products = sorted(example[min_key].items(),
                             key=lambda item: item[1])[:6]
    answer["products"] = {k: v for k, v in sorted_products}

    with open("output.txt", "w") as f:
        f.write(f"{answer['city'].decode()} {answer['total']:.2f}\n")
        for k, v in answer['products'].items():
            f.write(f"{k.decode()} {v:.2f}\n")


if __name__ == "__main__":
    main()
