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
