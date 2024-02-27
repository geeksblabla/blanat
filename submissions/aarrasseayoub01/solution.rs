// use std::collections::HashMap;
// use std::fs::File;
// use std::io::{self, BufRead, BufReader, Write};

// fn main() -> io::Result<()> {
//     let input_path = "input.txt";
//     let output_path = "output.txt";

//     let start = std::time::Instant::now();
//     let (chosen_city, total_score, product_list) = process_data(input_path)?;
//     let elapsed = start.elapsed();

//     println!("Processing took: {:?} ms", elapsed.as_millis());

//     output_results(output_path, &chosen_city, total_score, &product_list)?;

//     Ok(())
// }

// fn process_data(path: &str) -> io::Result<(String, f64, Vec<(String, f64)>)> {
//     let file = File::open(path)?;
//     let buffered = BufReader::new(file);

//     let mut city_scores: HashMap<String, f64> = HashMap::new();
//     let mut min_prices: HashMap<String, f64> = HashMap::new();

//     for line in buffered.lines() {
//         let line = line?;
//         let parts: Vec<&str> = line.split(',').collect();
//         if parts.len() < 3 {
//             continue;
//         }

//         let city_name = parts[0].to_string();
//         let item = parts[1].to_string();
//         let cost: f64 = parts[2].parse().unwrap_or(0.0);

//         *city_scores.entry(city_name).or_insert(0.0) += cost;

//         min_prices
//             .entry(item)
//             .and_modify(|e| *e = f64::min(*e, cost))
//             .or_insert(cost);
//     }

//     let (chosen_city, &total_score) = city_scores.iter().min_by(|a, b| a.1.partial_cmp(&b.1).unwrap()).unwrap();
//     let mut products: Vec<(String, f64)> = min_prices.into_iter().collect();
//     products.sort_by(|a, b| a.1.partial_cmp(&b.1).unwrap());

//     Ok((chosen_city.clone(), total_score, products.into_iter().take(5).collect()))
// }

// fn output_results(path: &str, city: &str, score: f64, items: &[(String, f64)]) -> io::Result<()> {
//     let mut file = File::create(path)?;

//     writeln!(file, "{} {}", city, score)?;

//     for (product, price) in items {
//         writeln!(file, "{} {:.3}", product, price)?;
//     }

//     Ok(())
// }
use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead, Write};
use std::path::Path;
use std::time::Instant;

fn main() -> io::Result<()> {
    let start_time = Instant::now();

    let input_file_path = "input.txt";
    let output_file_path = "output.txt";

    let mut city_scores = HashMap::<String, f64>::new();
    let mut city_products = HashMap::<String, Vec<(String, f64)>>::new();

    // Read lines from the input file and process them
    if let Ok(lines) = read_lines(input_file_path) {
        for line in lines.flatten() {
            let parts: Vec<&str> = line.split(',').collect();
            if parts.len() >= 3 {
                let city = parts[0].trim();
                let product = parts[1].trim();
                let score: f64 = parts[2].parse().unwrap_or(0.0);

                *city_scores.entry(city.to_string()).or_insert(0.0) += score;
                city_products.entry(city.to_string()).or_insert_with(Vec::new).push((product.to_string(), score));
            }
        }
    }

    // Find the city with the lowest total product score
    let (cheapest_city, &total_score) = city_scores.iter().min_by(|a, b| a.1.partial_cmp(b.1).unwrap()).unwrap();

    // Sort products within the cheapest city by score
    let mut products = city_products.get(cheapest_city).unwrap().clone();
    products.sort_by(|a, b| a.1.partial_cmp(&b.1).unwrap());

    // Write results to output file
    let mut output = File::create(output_file_path)?;
    writeln!(output, "{} {:.2}", cheapest_city, total_score)?;
    for (product, score) in products.iter().take(5) {
        writeln!(output, "{} {:.2}", product, score)?;
    }

    let end_time = Instant::now();
    let duration = end_time.duration_since(start_time);
    println!("Processing took: {:.2} seconds", duration.as_secs_f64());

    Ok(())
}

// Utility function to read lines from a file
fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where
    P: AsRef<Path>,
{
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}
