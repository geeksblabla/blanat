// Compile with: rustc -O solution.rs -o solution
use std::fs::File;
use std::io::{BufRead, BufReader, Seek, SeekFrom, Result, Write};
use std::sync::{Arc, Mutex};
use std::thread;
use std::collections::HashMap;

#[derive(Debug, Default)]
struct ProcessResult {
    // [ city => (total_price, [ product => lowest_price ] ) ]
    cities: HashMap<String, (f64, HashMap<String, f64>)>,
}

// We get the offset that each thread will start processing from
fn find_chunk_offsets(file_path: &str, num_chunks: usize) -> Result<Vec<u64>> {
    let file = File::open(file_path)?;
    let file_size = file.metadata()?.len();
    let chunk_size = file_size / num_chunks as u64;
    let mut offsets = vec![0];
    let mut reader = BufReader::new(file);

    for _ in 0..num_chunks - 1 {
        let seek_pos = offsets.last().unwrap() + chunk_size;
        reader.seek(SeekFrom::Start(seek_pos))?;
        
        let mut buf = Vec::new();
        reader.read_until(b'\n', &mut buf)?;
        let next_offset = seek_pos + buf.len() as u64;
        offsets.push(next_offset);
    }

    Ok(offsets)
}

fn process_file_chunk(file_path: &str, start_offset: u64, end_offset: u64) -> Result<ProcessResult> {
    let mut file = File::open(file_path)?;

    // Seek to the start of the chunk the thread is responsible for
    file.seek(SeekFrom::Start(start_offset))?;

    let reader = BufReader::new(file);
    let mut result = ProcessResult::default();

    let mut current_pos = start_offset;
    for line in reader.lines() {
        let line = line?;
        current_pos += line.len() as u64 + 1; // +1 for the newline character
        if current_pos > end_offset && end_offset != std::u64::MAX {
            break;
        }

        let parts: Vec<&str> = line.split(',').collect();
        if parts.len() != 3 { // Skip invalid lines
            continue;
        }

        let city = parts[0].to_string();
        let product = parts[1].to_string();
        let price: f64 = parts[2].parse().unwrap();

        // Insert the city and product if they don't exist, and update the price if it's lower
        let city_entry = result.cities.entry(city).or_insert_with(|| (0.0, HashMap::new()));
        city_entry.1.entry(product).and_modify(|e| *e = e.min(price)).or_insert(price);

        // Update the total price
        city_entry.0 += price;
    }

    Ok(result)
}

fn merge_results(results: Vec<ProcessResult>) -> ProcessResult {
    let mut final_result = ProcessResult::default();
    
    for result in results {
        for (city, (total_price, products)) in result.cities {
            let city_entry = final_result.cities.entry(city).or_insert_with(|| (0.0, HashMap::new()));
            city_entry.0 += total_price;

            for (product, price) in products {
                city_entry.1.entry(product).and_modify(|e| *e = e.min(price)).or_insert(price);
            }
        }
    }

    final_result
}

fn output_results(final_results: ProcessResult) -> Result<()> {
    let mut ofile = File::create("output.txt")?;

    if let Some((city, (total, products))) = final_results.cities.iter().min_by(|(_, (a, _)), (_, (b, _))| a.partial_cmp(b).unwrap()) {
        writeln!(ofile, "{} {:.2}", city, total).unwrap();
        
        let mut products: Vec<_> = products.iter().collect();
        products.sort_by(|a, b| a.1.partial_cmp(b.1).unwrap().then(a.0.cmp(b.0)));

        for (product, price) in products.iter().take(5) {
            writeln!(ofile, "{} {:.2}", product, price).unwrap();
        }
    }
    Ok(())
}

fn main() -> Result<()> {
    let file_path = "input.txt";
    let num_threads = std::thread::available_parallelism().unwrap().get();
    // dbg!("Using {} threads", num_threads);
    let offsets = find_chunk_offsets(file_path, num_threads)?;

    let results = Arc::new(Mutex::new(Vec::new()));
    let mut handles = vec![];

    for i in 0..num_threads {
        let start_offset = offsets[i];
        let end_offset = if i < num_threads - 1 { offsets[i + 1] } else { std::u64::MAX };

        let thread_results = Arc::clone(&results);
        let file_path = file_path.to_string();

        let handle = thread::spawn(move || {
            let result = process_file_chunk(&file_path, start_offset, end_offset).unwrap();
            thread_results.lock().unwrap().push(result);
        });

        handles.push(handle);
    }

    // Join all the threads
    for handle in handles {
        handle.join().unwrap();
    }

    // Collect the results from the threads
    let mut results_vec = Vec::new();
    for _ in 0..num_threads {
        let result = results.lock().unwrap().pop().unwrap();
        results_vec.push(result);
    }

    // Merge the results and output the final result
    let merged_results = merge_results(results_vec);
    output_results(merged_results)?;

    Ok(())
}
