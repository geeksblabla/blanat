use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead, BufReader, Write};
use std::path::Path;
use std::sync::{Arc, Mutex};
use std::thread;


fn main() -> io::Result<()> {

    let input_file_path = "input.txt";
    let output_file_path = "output.txt";

    let file = File::open(input_file_path)?;
    let reader = BufReader::new(file);
    let lines: Vec<String> = reader.lines().collect::<Result<_, _>>()?;

    let num_threads = thread::available_parallelism().map_or(4, |n| n.get());
    let chunk_size = lines.len() / num_threads + 1;
    let mut threads = vec![];

    let city_scores_global = Arc::new(Mutex::new(HashMap::<String, f64>::new()));
    let city_products_global = Arc::new(Mutex::new(HashMap::<String, Vec<(String, f64)>>::new()));

    for chunk in lines.chunks(chunk_size) {
        let city_scores = Arc::clone(&city_scores_global);
        let city_products = Arc::clone(&city_products_global);

        let chunk = chunk.to_owned();
        threads.push(thread::spawn(move || {
            let mut local_city_scores = HashMap::new();
            let mut local_city_products = HashMap::new();

            for line in chunk {
                let parts: Vec<&str> = line.split(',').collect();
                if parts.len() >= 3 {
                    let city = parts[0].trim().to_string();
                    let product = parts[1].trim().to_string();
                    let score: f64 = parts[2].parse().unwrap_or_default();

                    *local_city_scores.entry(city.clone()).or_insert(0.0) += score;
                    local_city_products.entry(city).or_insert_with(Vec::new).push((product, score));
                }
            }

            let mut scores = city_scores.lock().unwrap();
            let mut products = city_products.lock().unwrap();
            for (city, score) in local_city_scores {
                *scores.entry(city.clone()).or_insert(0.0) += score;
            }
            for (city, prods) in local_city_products {
                products.entry(city).or_default().extend(prods);
            }
        }));
    }

    for thread in threads {
        thread.join().unwrap();
    }

    let city_scores = city_scores_global.lock().unwrap();
    let city_products = city_products_global.lock().unwrap();

    let (cheapest_city, &total_score) = city_scores.iter().min_by(|a, b| a.1.partial_cmp(b.1).unwrap()).unwrap();

    let mut products = city_products.get(cheapest_city).unwrap().clone();
    products.sort_by(|a, b| a.1.partial_cmp(&b.1).unwrap().then_with(|| a.0.cmp(&b.0)));

    let mut output = File::create(output_file_path)?;
    writeln!(output, "{} {:.2}", cheapest_city, total_score)?;
    for (i, (product, score)) in products.iter().enumerate().take(5) {
        if i == 4 {
            write!(output, "{} {:.2}", product, score)?;
        } else {
            writeln!(output, "{} {:.2}", product, score)?;
        }
    }

    Ok(())
}

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where
    P: AsRef<Path>,
{
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}
