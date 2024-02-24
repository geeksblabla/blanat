<?php
/*
 * Return each line using generator
 * @param string $path
 */
function getLines(string $path) {
    $handle = fopen($path, "r");

    while(!feof($handle)) {
        yield trim(fgets($handle));
    }

    fclose($handle);
}

/**
 * Process of getting infos from each line
 * @param $item
 * @param array $cities
 */
function getElement($item, array &$cities)
{
    $c = explode(",", $item);
    if (!isset($cities[$c[0]])) {
        $cities[$c[0]] = array();
        $cities[$c[0]]["products"] = array();
        $cities[$c[0]]["total"] = 0.00;
    }
    $cities[$c[0]]["city"] = $c[0];
    $cities[$c[0]]["products"][] = [
        "productName" => $c[1],
        "productPrice" => (float)$c[2]
    ];
    $cities[$c[0]]["total"] += (float)$c[2];
}

$cities = array();

//loop for getting elements from each lien of the input.txt file
foreach (getLines("input_petit.txt") as $item) {
    if(strlen($item)>0) {
        getElement($item, $cities);
    }
}
//sort elements by total price and the name of cities
array_multisort (array_column($cities, 'total'), SORT_ASC, array_column($cities, 'city'), SORT_ASC, $cities);

$outputFileName = "output.txt"; //output file name
//knowing that the array is sorted by total price and name, the process will print only the first element of the array
foreach ($cities as $cityName => $city) {
    file_put_contents($outputFileName,$cityName." ".$city["total"]."\n");
    //sort products of the city by price to print only the cheapests
    array_multisort (array_column($city["products"], 'productPrice'), SORT_ASC, array_column($city["products"], 'productName'), SORT_ASC, $city["products"]);
    $cte = 0; //counter for printing only 5 products
    $productPrinted = array();
    foreach ($city["products"] as $product) {
        if(in_array($product["productName"],$productPrinted))
        {
            continue;
        }
        $productPrinted[] = $product["productName"];
        file_put_contents($outputFileName,$product["productName"]." ".$product["productPrice"]."\n", FILE_APPEND);
        $cte++;
        if($cte === 5) break;
    }
    break;
}