use std::collections::HashMap;
use std::fs;
use std::io::{self, BufRead, Write};

fn main() {
    if let Ok(file) = fs::File::open("input.txt") {
        let reader = io::BufReader::new(file);
        let mut map: HashMap<String, HashMap<String, (f64, f64)>> = HashMap::new();
        let mut index_totals: HashMap<String, f64> = HashMap::new();

        for line in reader.lines() {
            if let Ok(line) = line {
                let parts: Vec<&str> = line.trim().split(',').collect();
                if parts.len() >= 3 {
                    let index = parts[0].trim().to_string();
                    let key = parts[1].trim().to_string();
                    let value = parts[2].trim().parse::<f64>().unwrap_or_default();
                    let inner_hashmap = map.entry(index.clone()).or_insert(HashMap::new());
                    if let Some((smallest_value, total)) = inner_hashmap.get_mut(&key) {
                        *smallest_value = value.min(*smallest_value);
                        *total += value;
                    } else {
                        inner_hashmap.insert(key, (value, value));
                    }
                    let total = index_totals.entry(index).or_insert(0.0);
                    *total += value;
                }
            }
        }

        if let Ok(mut output_file) = fs::File::create("output.txt") {
            let (smallest_index, smallest_value) = index_totals
                .iter()
                .min_by(|(_, v1), (_, v2)| v1.partial_cmp(v2).unwrap())
                .unwrap();
            writeln!(output_file, "{} {:.2}", smallest_index, smallest_value)
                .expect("Failed to write to output file");

            if let Some(inner_map) = map.get(smallest_index) {
                let mut sorted_values: Vec<(&String, &(f64, f64))> = inner_map.iter().collect();
                sorted_values.sort_by(|(_, (v1, i1)), (_, (v2, i2))| {
                    if v1 == v2 {
                        i1.partial_cmp(i2).unwrap()
                    } else {
                        v1.partial_cmp(v2).unwrap()
                    }
                });

                let smallest_values = sorted_values.iter().take(5);

                for (key, (value, _)) in smallest_values {
                    writeln!(output_file, "{} {:.2}", key, value)
                        .expect("Failed to write to output file");
                }
            }
        } else {
            eprintln!("Failed to create output file: {:?}", std::io::Error::last_os_error());
        }
    } else {
        eprintln!("Failed to read file: {:?}", std::io::Error::last_os_error());
    }
}
