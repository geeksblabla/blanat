<?php
// Usage
process_file('./input.txt');

function process_file($file_name)
{
    $products   = [];
    $cities     = [];

    foreach (read_large_file($file_name) as $line) {

        list($city, $product, $price) = array_map('trim', explode(',', $line));

        $price = (float)$price;

        $products[$product] = isset($products[$product]) ? min($products[$product], $price) : $price;

        $cities[$city] = isset($cities[$city]) ? $cities[$city] + $price : $price;
    }

    // get cheapest city
    asort($cities);
    $cheapest_city = array_slice($cities, 0, 1, true);
    unset($cities);

    // sort products and get the first 5 cheapest product
    asort($products);
    $products       = array_slice($products, 0, 5, true);
    $product_names  = array_keys($products);
    $product_prices = array_values($products);
    array_multisort($product_prices, SORT_ASC, $product_names, SORT_ASC, $products);

    $output = key($cheapest_city) . " " . $cheapest_city[key($cheapest_city)];
    foreach ($products as $product => $price) {
        $output .= PHP_EOL . $product . " " . $price;
    }

    file_put_contents('./output.txt',  $output);
}

function read_large_file($file_name)
{
    $file = fopen($file_name, 'r');

    if (!$file) throw new Exception('Could not open file');

    while (($line = fgets($file)) !== false) {
        yield trim($line);
    }

    fclose($file);
}
