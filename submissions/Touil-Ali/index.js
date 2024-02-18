const fs = require("fs");
const readline = require("readline");

const input = fs.createReadStream("input.txt");
const output = fs.createWriteStream("output.txt");

const calculateSumByCity = async () => {
  try {
    const rl = readline.createInterface({
      input: input,
      crlfDelay: Infinity,
    });

    let smallestCity = null;
    let smallestSum = Infinity;
    const cityData = new Map();
    const productData = new Map();

    for await (const line of rl) {
      const [city, product, priceStr] = line.split(",");
      const price = parseFloat(priceStr);

      if (!isNaN(price) && product !== "") {
        cityData.set(city, (cityData.get(city) || 0) + parseFloat(price));

        if (!productData.has(city)) {
          productData.set(city, new Set());
        }
        productData.get(city).add({ product, price });
      }
    }

    for (const [city, sum] of cityData) {
      if (sum < smallestSum) {
        smallestSum = sum;
        smallestCity = city;
      }
    }

    if (smallestCity) {
      const sortedProducts = Array.from(productData.get(smallestCity))
        .sort((a, b) => a.price - b.price || a.product.localeCompare(b.product))
        .reduce((acc, current) => {
          if (!acc.some((x) => x.product === current.product)) {
            acc.push(current);
          }
          return acc;
        }, [])
        .slice(0, 6);

      const products = sortedProducts.map(
        ({ product, price }) => `${product} ${price.toFixed(2)}`,
      );

      const outputArray = [
        `${smallestCity} ${smallestSum.toFixed(2)}`,
        ...products,
      ];

      output.write(outputArray.join("\n"));
      output.end();
    }
  } catch (e) {
    console.log(e);
  }
};

console.time("benchmark");
calculateSumByCity();
console.timeEnd("benchmark");
