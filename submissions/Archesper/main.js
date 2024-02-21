const fs = require("fs");
const readline = require("readline");
const readStream = fs.createReadStream("input.txt");
const writeStream = fs.createWriteStream("output.txt");

const readlineInterface = readline.createInterface({ input: readStream });

async function processData() {
  let cheapestCityCost = Infinity;
  let cheapestCity = null;
  const products = new Map();
  const cities = new Map();

  for await (const line of readlineInterface) {
    let [city, item, price] = line.split(",");
    price = parseFloat(price);
    cities.set(city, cities.has(city) ? cities.get(city) + price : price);
    const currentProductData = products.get(city);
    if (currentProductData) {
      const currentProductPrice = currentProductData.get(item);
      if (!currentProductPrice || price < currentProductPrice) {
        currentProductData.set(item, price);
      }
    } else {
      products.set(city, new Map([[item, price]]));
    }
  }
  cities.forEach((livingCost, city) => {
    if (livingCost < cheapestCityCost) {
      cheapestCity = city;
      cheapestCityCost = livingCost;
    }
  });
  const cheapestCityProducts = Array.from(products.get(cheapestCity));
  const productOutput = cheapestCityProducts
    .sort((a, b) => a[1] - b[1] || a[0].localeCompare(b[0]))
    .slice(0, 5)
    .map((productData) => `${productData[0]} ${productData[1].toFixed(2)}`);

  writeStream.write(`${cheapestCity} ${cheapestCityCost.toFixed(2)}\n`);
  writeStream.write(productOutput.join("\n"));
}
processData();

