use std::io::Write;
use std::{
    collections::HashMap,
    error::Error,
    fs::File,
    io::{BufRead, BufReader, Seek},
    sync::{Arc, Mutex},
    thread,
};
use std::hash::{BuildHasher, Hasher};

// A fast, non-secure, hashing algorithm.
// https://github.com/cbreeden/fxhash
//
// Copyright 2015 The Rust Project Developers. See the COPYRIGHT
// file at the top-level directory of this distribution and at
// http://rust-lang.org/COPYRIGHT.
//
// Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
// http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
// <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
// option. This file may not be copied, modified, or distributed
// except according to those terms.
pub struct FxHasher {
    hash: u64,
}

impl Default for FxHasher {
    fn default() -> Self {
        FxHasher { hash: 0 }
    }
}

impl Hasher for FxHasher {
    fn finish(&self) -> u64 {
        self.hash
    }

    fn write(&mut self, bytes: &[u8]) {
        // FxHash implementation
        const PRIME: u64 = 1099511628211;
        for &byte in bytes {
            self.hash = self.hash.wrapping_mul(PRIME) ^ byte as u64;
        }
    }
}

impl BuildHasher for FxHasher {
    type Hasher = FxHasher;

    fn build_hasher(&self) -> Self::Hasher {
        FxHasher::default()
    }
}

const INPUT_FILE: &str = "./input.txt";
const OUTPUT_FILE: &str = "./output.txt";

type ProductsIndex = HashMap<String, (HashMap<String, f64, FxHasher>, f64), FxHasher>;

fn main() -> Result<(), Box<dyn Error>> {
    let available_threads = std::thread::available_parallelism()?.get();

    let input = File::open(INPUT_FILE)?;
    let file_length = input.metadata()?.len();
    let initial_chunk_size = file_length / available_threads as u64;

    let result: Arc<Mutex<ProductsIndex>> = Arc::new(Mutex::new(HashMap::with_capacity_and_hasher(101, FxHasher::default())));

    thread::scope(|scope| {
        for i in 0..available_threads {
            let start = initial_chunk_size * i as u64;
            let end = if i != available_threads - 1 {
                start + initial_chunk_size
            } else {
                0
            };
            let result_clone = result.clone();
            scope.spawn(move || {
                process_chunk(result_clone.clone(), start, end);
            });
        }
    });

    let result = result.lock().expect("");

    let cheapest_city = result
        .iter()
        .min_by(|a, b| a.1 .1.partial_cmp(&b.1 .1).unwrap())
        .unwrap()
        .0;
    let cheapest_products = &result[cheapest_city].0;

    let mut output = File::create(OUTPUT_FILE)?;
    writeln!(output, "{} {:.2}", cheapest_city, result[cheapest_city].1)?;

    let mut cheapest_products_sorted = cheapest_products.iter().collect::<Vec<_>>();
    cheapest_products_sorted.sort_by(|a, b| a.1.partial_cmp(b.1).unwrap().then(a.0.cmp(b.0)));

    for (product, price) in cheapest_products_sorted.iter().take(5) {
        writeln!(output, "{} {:.2}", product, price)?;
    }

    Ok(())
}

fn process_chunk(
    result: Arc<Mutex<ProductsIndex>>,
    start: u64,
    end: u64,
) -> Result<(), Box<dyn Error>> {

    let mut products_map_by_city: ProductsIndex = HashMap::with_capacity_and_hasher(101, FxHasher::default());

    let mut input = File::open(INPUT_FILE)?;
    input.seek(std::io::SeekFrom::Start(start))?;

    let mut reader = BufReader::with_capacity(16 * 1024 * 1024, input);
    let mut buf: Vec<u8> = Vec::new();
    reader.read_until(b'\n', &mut buf)?;

    let mut cursor = start + buf.len() as u64;
    let mut last = false;

    let mut line_unprocessed: Vec<u8> = Vec::new();

    loop {
        let read_bytes = reader.read_until(b'\n', &mut line_unprocessed)?;
        if read_bytes == 0 {
            break;
        }

        let line = &line_unprocessed.as_slice()[..line_unprocessed.len() - 1];

        cursor += line.len() as u64 + 1;
        if end != 0 && cursor > end {
            if last {
                break;
            }
            last = true;
        }

        let split = line.split(|char| *char == b',').collect::<Vec<&[u8]>>();
        let price = unsafe { std::str::from_utf8_unchecked(split[2]) }.parse::<f64>()?;

        let city_entry = products_map_by_city
            .entry(unsafe { std::str::from_utf8_unchecked(split[0]).to_string() })
            .or_insert_with(|| (HashMap::with_capacity_and_hasher(100, FxHasher::default()), 0.0));

        city_entry
            .0
            .entry(unsafe { std::str::from_utf8_unchecked(split[1]).to_string() })
            .and_modify(|old| {
                if *old > price {
                    *old = price
                }
            })
            .or_insert_with(|| price);

        city_entry.1 += price;

        line_unprocessed.clear();
    }

    let mut result_lock = result.lock();
    let result = result_lock.as_mut().expect("");

    for (city, (products, total)) in products_map_by_city {
        let city_entry = result
            .entry(city)
            .or_insert_with(|| (HashMap::with_capacity_and_hasher(100, FxHasher::default()), 0.0));

        city_entry.1 += total;

        for (product, price) in products {
            city_entry
                .0
                .entry(product)
                .and_modify(|old| {
                    if *old > price {
                        *old = price
                    }
                })
                .or_insert_with(|| price);
        }
    }

    Ok(())
}
