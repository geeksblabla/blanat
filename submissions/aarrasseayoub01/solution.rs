use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead, BufReader, Write};

fn main() -> io::Result<()> {
    let input_path = "input.txt";
    let output_path = "output.txt";

    let start = std::time::Instant::now();
    let (chosen_city, total_score, product_list) = process_data(input_path)?;
    let elapsed = start.elapsed();

    println!("Processing took: {:?} ms", elapsed.as_millis());

    output_results(output_path, &chosen_city, total_score, &product_list)?;

    Ok(())
}

fn process_data(path: &str) -> io::Result<(String, f64, Vec<(String, f64)>)> {
    let file = File::open(path)?;
    let buffered = BufReader::new(file);

    let mut city_scores: HashMap<String, f64> = HashMap::new();
    let mut min_prices: HashMap<String, f64> = HashMap::new();

    for line in buffered.lines() {
        let line = line?;
        let parts: Vec<&str> = line.split(',').collect();
        if parts.len() < 3 {
            continue;
        }

        let city_name = parts[0].to_string();
        let item = parts[1].to_string();
        let cost: f64 = parts[2].parse().unwrap_or(0.0);

        *city_scores.entry(city_name).or_insert(0.0) += cost;

        min_prices
            .entry(item)
            .and_modify(|e| *e = f64::min(*e, cost))
            .or_insert(cost);
    }

    let (chosen_city, &total_score) = city_scores.iter().min_by(|a, b| a.1.partial_cmp(&b.1).unwrap()).unwrap();
    let mut products: Vec<(String, f64)> = min_prices.into_iter().collect();
    products.sort_by(|a, b| a.1.partial_cmp(&b.1).unwrap());

    Ok((chosen_city.clone(), total_score, products.into_iter().take(5).collect()))
}

fn output_results(path: &str, city: &str, score: f64, items: &[(String, f64)]) -> io::Result<()> {
    let mut file = File::create(path)?;

    writeln!(file, "{} {:.2}", city, score)?;

    for (product, price) in items {
        writeln!(file, "{} {:.2}", product, price)?;
    }

    Ok(())
}

