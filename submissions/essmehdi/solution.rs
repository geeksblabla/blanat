use std::{collections::HashMap, error::Error, fs::File, io::{BufRead, BufReader, Seek}, sync::{Arc, Mutex}, thread};
use std::io::Write;

const INPUT_FILE: &str = "./input.txt";
const OUTPUT_FILE: &str = "./output.txt";

type ProductsIndex = HashMap<String, (HashMap<String, f64>, f64)>;

fn main() -> Result<(), Box<dyn Error>> {
    let available_threads = std::thread::available_parallelism()?.get();

    let input = File::open(INPUT_FILE)?;
    let file_length = input.metadata()?.len();
    let initial_chunk_size = file_length / available_threads as u64;

    let result: Arc<Mutex<ProductsIndex>> = Arc::new(Mutex::new(HashMap::with_capacity(101)));
    
    thread::scope(|scope| {
        for i in 0..available_threads {
            let start = initial_chunk_size * i as u64;
            let end = if i != available_threads - 1 { start + initial_chunk_size } else { 0 };
            let result_clone = result.clone();
            scope.spawn(move || {
                process_chunk(result_clone.clone(), start, end);
            });
        }
    });

    let result = result.lock().expect("");
    
    let cheapest_city = result
                                    .iter()
                                    .min_by(|a, b| {
                                        a.1.1.partial_cmp(&b.1.1).unwrap()
                                    })
                                    .unwrap()
                                    .0;
    let cheapest_products = &result[cheapest_city].0;

    let mut output = File::create(OUTPUT_FILE)?;
    writeln!(output, "{} {:.2}", cheapest_city, result[cheapest_city].1)?;

    let mut cheapest_products_sorted = cheapest_products.iter().collect::<Vec<_>>();
    cheapest_products_sorted.sort_by(|a, b| {
        a.1.partial_cmp(b.1).unwrap().then(a.0.cmp(b.0))
    });
    
    for (product, price) in cheapest_products_sorted.iter().take(5) {
        writeln!(output, "{} {:.2}", product, price)?;
    }

    Ok(())
}

fn process_chunk(result: Arc<Mutex<ProductsIndex>>, start: u64, end: u64) -> Result<(), Box<dyn Error>> {

    let mut products_map_by_city: ProductsIndex = HashMap::with_capacity(101);

    let mut input = File::open(INPUT_FILE)?;
    input.seek(std::io::SeekFrom::Start(start))?;
    
    let mut reader = BufReader::with_capacity(16 * 1024 * 1024, input);
    let mut buf: Vec<u8> = Vec::new();
    reader.read_until(b'\n', &mut buf)?;

    let mut cursor = start + buf.len() as u64;
    let mut last = false;

    for line in reader.lines() {
        let line = line?;
        cursor += line.len() as u64 + 1;

        if end != 0 && cursor > end {
            if last {
                break;
            }
            last = true;
        }

        let split = line.split(",").collect::<Vec<&str>>();
        let [city, product, price] = [split[0], split[1], split[2]];
        let price = price.parse::<f64>()?;

        let city_entry = products_map_by_city
            .entry(String::from(city))
            .or_insert_with(|| (HashMap::with_capacity(100), 0.0));

        city_entry.0.entry(String::from(product))
            .and_modify(|old| if *old > price { *old = price })
            .or_insert_with(|| price);

        city_entry.1 += price;
    }

    let mut result_lock = result.lock();
    let result = result_lock.as_mut().expect("");

    for (city, (products, total)) in products_map_by_city {
        let city_entry = 
            result
                .entry(city)
                .or_insert_with(|| (HashMap::with_capacity(100), 0.0));

        city_entry.1 += total;

        for (product, price) in products {
            city_entry.0.entry(product)
                .and_modify(|old| if *old > price { *old = price })
                .or_insert_with(|| price);
        }
    }

    Ok(())
}
