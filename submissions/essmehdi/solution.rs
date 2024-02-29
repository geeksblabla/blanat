use std::cmp::Ordering;
use std::io::Write;
use std::{
    collections::HashMap,
    error::Error,
    fs::File,
    io::{BufRead, BufReader, Seek},
    sync::{Arc, Mutex},
    thread,
};
use std::hash::{BuildHasher, Hash, Hasher};

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

const PRODUCTS: [&str; 100] = [
    "Apple", "Banana", "Orange", "Strawberry", "Grapes",
    "Watermelon", "Pineapple", "Mango", "Kiwi", "Peach",
    "Plum", "Cherry", "Pear", "Blueberry", "Raspberry",
    "Blackberry", "Cantaloupe", "Honeydew", "Coconut", "Pomegranate",
    "Lemon", "Lime", "Grapefruit", "Avocado", "Papaya",
    "Guava", "Fig", "Passion_Fruit", "Apricot", "Nectarine",
    "Cucumber", "Carrot", "Broccoli", "Spinach", "Kale",
    "Lettuce", "Tomato", "Bell_Pepper", "Zucchini", "Eggplant",
    "Cabbage", "Cauliflower", "Brussels_Sprouts", "Radish", "Beet",
    "Asparagus", "Artichoke", "Green_Beans", "Peas", "Celery",
    "Onion", "Garlic", "Potato", "Sweet_Potato", "Yam",
    "Butternut_Squash", "Acorn_Squash", "Pumpkin", "Cranberry", "Goji_Berry",
    "Currant", "Date", "Clementine", "Cranberry", "Rhubarb",
    "Chard", "Collard_Greens", "Parsley", "Cilantro", "Mint",
    "Basil", "Thyme", "Rosemary", "Sage", "Dill",
    "Oregano", "Cantaloupe", "Honeydew", "Coconut", "Pomegranate",
    "Jackfruit", "Starfruit", "Persimmon", "Ginger", "Turnip",
    "Jicama", "Kohlrabi", "Watercress", "Okra", "Artichoke",
    "Plantain", "Cactus_Pear", "Kiwano", "Squash_Blossom", "Dragon_Fruit",
    "Parsnip", "Rutabaga", "Salsify", "Bok_Choy", "Endive"
];

const CITIES: [&str; 101] = [
    "Casablanca", "Rabat", "Marrakech", "Fes", "Tangier",
    "Agadir", "Meknes", "Oujda", "Kenitra", "Tetouan",
    "Safi", "El_Jadida", "Beni_Mellal", "Errachidia",
    "Taza", "Essaouira", "Khouribga", "Guelmim",
    "Jorf_El_Melha", "Laayoune", "Ksar_El_Kebir", "Sale", "Bir_Lehlou",
    "Arfoud", "Temara", "Mohammedia", "Settat",
    "Béni_Mellal", "Nador", "Kalaat_MGouna",
    "Chichaoua", "Chefchaouen", "Al_Hoceima", "Taourirt",
    "Taroudant", "Guelta_Zemmur", "Dakhla", "Laâyoune",
    "Tiznit", "Tinghir", "Ifrane", "Azrou", "Bab_Taza",
    "Berrechid", "Sidi_Slimane", "Souk_Larbaa", "Tiflet", "Sidi_Bennour",
    "Larache", "Tan-Tan", "Sidi_Ifni", "Goulmima",
    "Midelt", "Figuig", "Azilal", "Jerada", "Youssoufia",
    "Ksar_es_Seghir", "Tichka", "Ait_Melloul",
    "Layoune", "Ben_guerir", "Ouarzazate", "Inezgane",
    "Oujda_Angad", "Sefrou", "Aourir",
    "Oulad_Teima", "Tichla", "Bni_Hadifa",
    "Fquih_Ben_Salah", "Guercif", "Bouarfa", "Demnate",
    "Ahfir", "Berkane", "Akhfenir", "Boulemane",
    "Khenifra", "Bir_Anzerane", "Assa", "Smara", "Boujdour",
    "Tarfaya", "Ouazzane", "Zagora", "had_soualem",
    "Saidia", "Bab_Berred", "Midar", "Moulay_Bousselham",
    "Khemisset", "Guerguerat", "Asilah", "Sidi_Bouzid", "Tafraout",
    "Imzouren", "Zemamra", "Sidi_Kacem", "Drarga", "Skhirate"
];

fn main() -> Result<(), Box<dyn Error>> {
    let available_threads = std::thread::available_parallelism()?.get() * 2;

    let mut cities_map: HashMap<u64, usize, FxHasher> = HashMap::with_capacity_and_hasher(101, FxHasher::default());
    let mut products_map: HashMap<u64, usize, FxHasher> = HashMap::with_capacity_and_hasher(100, FxHasher::default());
    
    for i in 0..CITIES.len() {
        cities_map.insert(get_hash(&CITIES[i].as_bytes()[0..CITIES[i].len().min(8)]), i);
    }

    for i in 0..PRODUCTS.len() {
        products_map.insert(get_hash(&PRODUCTS[i].as_bytes()[0..PRODUCTS[i].len().min(8)]), i);
    }

    let input = File::open(INPUT_FILE)?;
    let file_length = input.metadata()?.len();
    let initial_chunk_size = file_length / available_threads as u64;

    let result = [[0.0; 100]; 101];
    let result = Arc::new(Mutex::new(result));

    let totals_result = [0.0; 101];
    let totals_result_arc = Arc::new(Mutex::new(totals_result));

    let cities_map_arc = Arc::new(cities_map);
    let products_map_arc = Arc::new(products_map);

    thread::scope(|scope| {
        for i in 0..available_threads {
            let start = initial_chunk_size * i as u64;
            let end = if i != available_threads - 1 {
                start + initial_chunk_size
            } else {
                0
            };
            let result_clone = result.clone();
            let totals_result_clone = totals_result_arc.clone();
            let cities_map_clone = cities_map_arc.clone();
            let products_map_clone = products_map_arc.clone();
            scope.spawn(move || {
                process_chunk(result_clone, totals_result_clone, start, end, cities_map_clone, products_map_clone);
            });
        }
    });

    let result = result.lock().expect("");
    let totals = totals_result_arc.lock().expect("");

    let cheapest_city_result = totals
        .iter()
        .enumerate()
        .map(|(index, total)| {
            (index, total)
        })
        .min_by(|a, b| {
            if *a.1 == 0.0 {
                return Ordering::Greater;
            }
            if *b.1 == 0.0 {
                return Ordering::Less
            }
            a.1.partial_cmp(&b.1).unwrap()
        })
        .unwrap();
    let (cheapest_city, total) = cheapest_city_result;

    let cheapest_products = &result[cheapest_city];    
    
    let mut cheapest_products_sorted = cheapest_products.iter().enumerate().collect::<Vec<_>>();
    cheapest_products_sorted.sort_by(|a, b| {
        if *a.1 == 0.0 {
            return Ordering::Greater;
        }
        if *b.1 == 0.0 {
            return Ordering::Less;
        }
        a.1.partial_cmp(b.1).unwrap().then(CITIES[a.0].cmp(CITIES[b.0]))
    });
    
    let mut output = File::create(OUTPUT_FILE)?;
    writeln!(output, "{} {:.2}", CITIES[cheapest_city], total)?;

    for (index, price) in cheapest_products_sorted.iter().take(5) {
        writeln!(output, "{} {:.2}", PRODUCTS[*index], price)?;
    }

    Ok(())
}

fn process_chunk(
    result: Arc<Mutex<[[f64; 100]; 101]>>,
    totals: Arc<Mutex<[f64; 101]>>,
    start: u64,
    end: u64,
    cities_map: Arc<HashMap<u64, usize, FxHasher>>,
    products_map: Arc<HashMap<u64, usize, FxHasher>>
) -> Result<(), Box<dyn Error>> {

    let mut products_map_by_city = [[0.0; 100]; 101];
    let mut totals_chunk = [0.0; 101];

    let mut input = File::open(INPUT_FILE)?;
    input.seek(std::io::SeekFrom::Start(start))?;

    let mut reader = BufReader::with_capacity(16 * 1024 * 1024, input);
    let mut buf: Vec<u8> = Vec::new();
    reader.read_until(b'\n', &mut buf)?;

    let mut cursor = start + buf.len() as u64;
    let mut last = false;

    let mut city_vec = Vec::new();
    let mut product_vec = Vec::new();
    let mut price_vec = Vec::new();

    loop {
        let city_length = reader.read_until(b',', &mut city_vec)?;
        if city_length == 0 {
            break;
        }

        let product_length = reader.read_until(b',', &mut product_vec)?;
        let price_length = reader.read_until(b'\n', &mut price_vec)?;

        cursor += (city_length + price_length + product_length) as u64;
        if end != 0 && cursor > end {
            if last {
                break;
            }
            last = true;
        }

        let price = parse(&price_vec[0..&price_vec.len() - 1]);

        let city_length = (city_length - 1).min(8);
        let city_hash = get_hash(&city_vec[0..city_length]);
    
        let product_length = (product_length - 1).min(8);
        let product_hash = get_hash(&product_vec[0..product_length]);

        let product_price = &mut products_map_by_city[cities_map[&city_hash]][products_map[&product_hash]];
        if *product_price != 0.0 {
            if *product_price > price {
                *product_price = price;
            }
        } else {
            *product_price = price;

        }

        let total = &mut totals_chunk[cities_map[&city_hash]];
        *total += price;

        city_vec.clear();
        product_vec.clear();
        price_vec.clear();
    }

    let mut result_lock = result.lock();
    let result = result_lock.as_mut().expect("");
    let mut totals_lock = totals.lock();
    let totals = totals_lock.as_mut().expect("");

    for i in 0..101 {
        for j in 0..100 {
            let chunk_price = products_map_by_city[i][j];
            let result_price = &mut result[i][j];
            if *result_price != 0.0 {
                if *result_price > chunk_price {
                    *result_price = chunk_price;
                }
            } else {
                *result_price = chunk_price;
            }    
        }

        let total_price = &mut totals[i];
        *total_price += totals_chunk[i];
    }

    Ok(())
}

fn get_hash(to_hash: &[u8]) -> u64 {
    let mut hasher = FxHasher::default();
    to_hash.hash(&mut hasher);
    hasher.finish()
}

fn parse(to_parse: &[u8]) -> f64 {
    let (a, b, c, d, e) = match to_parse {
        [c, b'.', d] => (0, 0, c - b'0', d - b'0', 0),
        [b, c, b'.', d] => (0, b - b'0', c - b'0', d - b'0', 0),
        [a, b, c, b'.', d] => (a - b'0', b - b'0', c - b'0', d - b'0', 0),
        [c, b'.', d, e] => (0, 0, c - b'0', d - b'0', e - b'0'),
        [b, c, b'.', d, e] => (0, b - b'0', c - b'0', d - b'0', e - b'0'),
        [a, b, c, b'.', d, e] => (a - b'0', b - b'0', c - b'0', d - b'0', e - b'0'),
        [c] => (0, 0, c - b'0', 0, 0),
        [b, c] => (0, b - b'0', c - b'0', 0, 0),
        [a, b, c] => (a - b'0', b - b'0', c - b'0', 0, 0),
        _ => unreachable!(),
    };
    a as f64 * 100 as f64 + b as f64 * 10 as f64 + c as f64 + d as f64 * 0.1 + e as f64 * 0.01
}
