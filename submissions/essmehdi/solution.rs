use std::{collections::HashMap, fs::File, io::{BufRead, BufReader}};
use std::io::Write;

const INPUT_FILE: &str = "./input.txt";
const OUTPUT_FILE: &str = "./output.txt";

fn main() {
    let input = File::open(INPUT_FILE).unwrap();
    let buffer_reader = BufReader::new(input);

    let mut products_map_by_city: HashMap<String, (HashMap<String, f64>, f64)> = HashMap::new();

    for line in buffer_reader.lines() {
        let line = line.unwrap();
        let split = line.split(",").collect::<Vec<&str>>();
        let [city, product, price] = [split[0], split[1], split[2]];
        let price = price.parse::<f64>().unwrap();

        let city_entry = products_map_by_city
            .entry(city.to_string())
            .or_insert((HashMap::new(), 0f64));

        city_entry.0.entry(product.to_string())
            .and_modify(|old| if *old > price { *old = price })
            .or_insert(price);

        city_entry.1 += price;
    }

    let cheapest_city = products_map_by_city.iter().min_by(|a, b| a.1.1.partial_cmp(&b.1.1).unwrap()).unwrap().0;
    let cheapest_products = &products_map_by_city[cheapest_city].0;

    let mut output = File::create(OUTPUT_FILE).unwrap();
    writeln!(output, "{} {:.2}", cheapest_city, products_map_by_city[cheapest_city].1).unwrap();

    let mut cheapest_products_sorted = cheapest_products.iter().collect::<Vec<_>>();
    cheapest_products_sorted.sort_by(|a, b| {
        a.1.partial_cmp(b.1).unwrap().then(a.0.cmp(b.0))
    });
    
    for (product, price) in cheapest_products_sorted.iter().take(5) {
        writeln!(output, "{} {:.2}", product, price).unwrap();
    }
}