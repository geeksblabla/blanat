use std::cmp::Ordering;
use std::collections::{HashMap, BinaryHeap};
use std::fs::File;
use std::io::{BufRead, BufReader, Write};

#[derive(Clone)]
struct Product {
    price: f32,
    name: String,
}

impl Eq for Product {}

impl PartialEq<Self> for Product {
    fn eq(&self, other: &Self) -> bool {
        // not implement yet
        todo!()
    }
}

impl PartialOrd<Self> for Product {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}


impl Ord for Product {
    fn cmp(&self, other: &Self) -> Ordering {
        if self.price < other.price {
            Ordering::Less
        } else if self.price > other.price {
            Ordering::Greater
        } else {
            Ordering::Equal
        }
    }
}


fn map_city_products(file_path: &str) -> HashMap<String, BinaryHeap<Product>> {
    let mut city_products: HashMap<String, BinaryHeap<Product>> = HashMap::new();

    let file = File::open(file_path).expect("Failed to open file");
    let reader = BufReader::new(file);

    for line in reader.lines() {
        if let Ok(line) = line {
            let parts: Vec<&str> = line.trim().split(',').collect();
            let city = parts[0].to_string();
            let name = parts[1].to_string();
            let price = parts[2].parse::<f32>().expect("Invalid price");

            city_products.entry(city).or_insert(BinaryHeap::new()).push(Product { name, price });
        }
    }

    city_products
}

fn reduce(city_products: HashMap<String, BinaryHeap<Product>>) -> (String, f32, Vec<Product>) {
    let mut cheapest_city = String::new();
    let mut lowest_price = f32::INFINITY;
    let mut cheapest_products = Vec::new();

    for (city, products) in &city_products {
        let total_price = products.iter().map(|Product {
                                                   price, ..
                                               }| price).sum();
        if total_price < lowest_price {
            lowest_price = total_price;
            cheapest_city = city.clone();
            cheapest_products = products.iter().take(5).cloned().collect();
        }
    }

    (cheapest_city, lowest_price, cheapest_products)
}

fn main() {
    let file_path = "./input.txt";
    // map the products to cites
    let city_products = map_city_products(file_path);
    // reduce the hashmap
    let (cheapest_city, lowest_price, cheapest_products) = reduce(city_products);

    let mut output_file = File::create("output.txt").expect("Failed to create output file");
    writeln!(output_file, "{} {:.2}", cheapest_city, lowest_price).expect("Failed to write to output file");
    for Product in cheapest_products {
        writeln!(output_file, "{} {:.2}", Product.name, Product.price).expect("Failed to write to output file");
    }
}
