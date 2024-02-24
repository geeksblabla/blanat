use std::{collections::HashMap, fs::File, io::{BufRead, BufReader, Seek}, sync::{Arc, Mutex}, thread};
use std::io::Write;

const INPUT_FILE: &str = "./input.txt";
const OUTPUT_FILE: &str = "./output.txt";

type ProductsIndex = HashMap<String, (HashMap<String, f64>, f64)>;

fn main() {
    let available_threads = std::thread::available_parallelism().unwrap().get();

    let input = File::open(INPUT_FILE).unwrap();
    let file_length = input.metadata().unwrap().len();
    let initial_chunk_size = file_length / available_threads as u64;
    let mut buffer_reader = BufReader::with_capacity(16 * 1024 * 1024, input);

    let mut offsets = vec![0; available_threads];

    for i in 1..available_threads {
        let limit = offsets[i-1] + initial_chunk_size;
        buffer_reader.seek(std::io::SeekFrom::Start(limit)).unwrap();
        let mut read: Vec<u8> = Vec::new();
        buffer_reader.read_until(b'\n', &mut read).unwrap();

        offsets[i] = limit + read.len() as u64;
    }

    let mut handles = vec![];
    let result: Arc<Mutex<ProductsIndex>> = Arc::new(Mutex::new(HashMap::with_capacity(101)));
    
    for i in 0..available_threads {
        let start = offsets[i];
        let end = if i == available_threads - 1 {
            0
        } else {
            offsets[i+1]
        };
        let result_clone = result.clone();
        handles.push(thread::spawn(move || {
            process_chunk(result_clone.clone(), start, end);
        }));
    }

    for handle in handles {
        handle.join().unwrap();
    }

    let result = result.lock().unwrap();
    
    let cheapest_city = result
                        .iter()
                        .min_by(|a, b| {
                            a.1.1.partial_cmp(&b.1.1).unwrap()
                        })
                        .unwrap()
                        .0;
    let cheapest_products = &result[cheapest_city].0;

    let mut output = File::create(OUTPUT_FILE).unwrap();
    writeln!(output, "{} {:.2}", cheapest_city, result[cheapest_city].1).unwrap();

    let mut cheapest_products_sorted = cheapest_products.iter().collect::<Vec<_>>();
    cheapest_products_sorted.sort_by(|a, b| {
        a.1.partial_cmp(b.1).unwrap().then(a.0.cmp(b.0))
    });
    
    for (product, price) in cheapest_products_sorted.iter().take(5) {
        writeln!(output, "{} {:.2}", product, price).unwrap();
    }
}

fn process_chunk(result: Arc<Mutex<ProductsIndex>>, start: u64, end: u64) {

    let mut products_map_by_city: ProductsIndex = HashMap::new();

    let mut input = File::open(INPUT_FILE).unwrap();
    input.seek(std::io::SeekFrom::Start(start)).unwrap();
    
    let reader = BufReader::new(input);
    let mut cursor = start;

    for line in reader.lines() {
        let line = line.unwrap();
        cursor += line.len() as u64 + 1;

        if end != 0 && cursor > end {
            break;
        }

        let split = line.split(",").collect::<Vec<&str>>();
        let [city, product, price] = [split[0], split[1], split[2]];
        let price = price.parse::<f64>().unwrap();

        let city_entry = products_map_by_city
            .entry(String::from(city))
            .or_insert_with(|| (HashMap::new(), 0f64));

        city_entry.0.entry(String::from(product))
            .and_modify(|old| if *old > price { *old = price })
            .or_insert_with(|| price);

        city_entry.1 += price;
    }

    let mut result_lock = result.lock();
    let result = result_lock.as_mut().unwrap();

    for (city, (products, total)) in products_map_by_city {
        let city_entry = 
            result
                .entry(city)
                .or_insert_with(|| (HashMap::new(), 0.0));

        city_entry.1 += total;

        for (product, price) in products {
            city_entry.0.entry(product)
                .and_modify(|old| if *old > price { *old = price })
                .or_insert_with(|| price);
        }
    }
}
