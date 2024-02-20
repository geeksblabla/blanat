// Compile with: rustc -C opt-level=3 solution.rs -o solution
use std::collections::HashMap;
use std::io::{BufRead, BufReader};
use std::fs::File;
use std::io::Write;

fn main() {
    let file = File::open("input.txt").unwrap();
    let reader = BufReader::new(file);

    // [ city => (total_price, [ product => lowest_price ] ) ]
    let mut cities: HashMap<String, (f64, HashMap<String, f64>)> = HashMap::new();

    for line in reader.lines() {
        let line = line.unwrap();
        let parts: Vec<&str> = line.split(',').collect();

        if parts.len() != 3 { // Skip invalid lines
            continue;
        }

        let city = parts[0].to_string();
        let product = parts[1].to_string();
        let price: f64 = parts[2].parse().unwrap();

        // If the city is not in the hash map, add it
        cities.entry(city.clone()).or_insert((0.0, HashMap::new()));

        // Set the product in the map if it's not already there
        let city_entry = cities.get_mut(&city).unwrap();
        city_entry.1.entry(product.clone()).or_insert(price);

        // Update the price if the new price is lower
        let product_entry = city_entry.1.get_mut(&product).unwrap();
        if price < *product_entry {
            *product_entry = price;
        }

        // Update the total price
        city_entry.0 += price;
    }
    // Get the city with the max total price value 
    let (city, (total, products)) = cities.iter().max_by(|a, b| b.1.0.partial_cmp(&a.1.0).unwrap()).unwrap();

    // Write the output to the output.txt file
    let mut ofile = File::create("output.txt").unwrap();
    writeln!(ofile, "{}: {:.2}", city, total).unwrap();

    // Sort the products by value or name if the values are equal and print the cheapest 5 products
    let mut products: Vec<_> = products.iter().collect();
    products.sort_by(|a, b| a.1.partial_cmp(b.1).unwrap().then(a.0.cmp(b.0)));
    for (product, price) in products.iter().take(5) {
        writeln!(ofile, "{}: {:.2}", product, price).unwrap();
    }
}
