//to run the code use node solution.js in the terminal in this directory
// Importing the fs module for file operations
const fs = require('fs');

// Reading the input.txt file
fs.readFile('../../input.txt', 'utf8', (err, data) => {
    if (err) throw err;

    // Splitting the data into lines
    let lines = data.split('\n');

    // Creating maps to store city data and product data
    let cityData = new Map();
    let productData = new Map();

    // Variables to track the city with the smallest sum
    let smallestCity = null;
    let smallestSum = Infinity;

    // Looping through each line of data
    for (let i = 0; i < lines.length; i++) {
        let [city, product, priceStr] = lines[i].split(',');
        let price = parseFloat(priceStr);

        // Checking if the price is valid and the product is not empty
        if (!isNaN(price) && product !== '') {
            // Updating the cityData map with the sum of prices for each city
            cityData.set(city, (cityData.get(city) || 0) + price);

            // Updating the productData map with the products and prices for each city
            if (!productData.has(city)) {
                productData.set(city, new Set());
            }
            productData.get(city).add({ product, price });
        }
    }

    // Finding the city with the smallest sum
    for (let [city, sum] of cityData) {
        if (sum < smallestSum) {
            smallestSum = sum;
            smallestCity = city;
        }
    }

    // Checking if a smallest city is found
    if (smallestCity) {
        // Sorting and filtering the products for the smallest city
        let sortedProducts = Array.from(productData.get(smallestCity))
            .sort((a, b) => a.price - b.price || a.product.localeCompare(b.product))
            .reduce((acc, current) => {
                if (!acc.some(x => x.product === current.product)) {
                    acc.push(current);
                }
                return acc;
            }, [])
            .slice(0, 6);

        // Formatting the products and prices
        let products = sortedProducts.map(({ product, price }) => `${product} ${price.toFixed(2)}`);

        // Creating the output array
        let outputArray = [`${smallestCity} ${smallestSum.toFixed(2)}`, ...products];

        // Writing the output array to the output.txt file
        fs.writeFile('../../output.txt', outputArray.join('\n'), (err) => {
            if (err) throw err;
        });
    }
});