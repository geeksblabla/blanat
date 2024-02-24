const fs = require('fs');
const readline = require('readline');

const inputStream = fs.createReadStream('input.txt');
const outputStream = fs.createWriteStream('output.txt');

const getCityProductsSum = (cityProducts) =>
  Object.values(cityProducts).reduce((acc, current) => acc + current, 0);

const updateCityPrices = (city, product, priceValue, citiesPrices) => {
  const cityData = citiesPrices.get(city);
  if (!cityData?.[product] || priceValue < cityData[product]) {
    citiesPrices.set(city, {
      ...(cityData || {}),
      [product]: priceValue,
    });
  }
};

const findCheapestCity = (citiesPrices) => {
  let cheapestCity = {};
  let minSum = Infinity;

  for (const [city, products] of citiesPrices.entries()) {
    const citySum = getCityProductsSum(products);
    if (citySum < minSum) {
      minSum = citySum;
      cheapestCity = { city, products };
    }
  }

  return cheapestCity;
};

const sortAndSliceCheapestProducts = (cheapestCity) => {
  return Object.entries(cheapestCity.products)
    .sort(([productA, priceA], [productB, priceB]) => {
      return priceA - priceB || productA.localeCompare(productB);
    })
    .slice(0, 5);
};

const displayCheapestCity = (cheapestCity, cheapestProducts) => {
  const outputResult = [];
  outputResult.push(
    `${cheapestCity.city} ${getCityProductsSum(cheapestCity.products).toFixed(
      2
    )}`
  );

  cheapestProducts.forEach(([product, price]) => {
    outputResult.push(`${product} ${price.toFixed(2)}`);
  });

  outputStream.write(outputResult.join('\n'));
  outputStream.end();
};

function main() {
  inputStream.on('error', (err) => {
    console.error('Error occurred while opening the file:', err);
  });

  const rl = readline.createInterface({
    input: inputStream,
    crlfDelay: Infinity,
  });

  rl.on('error', (err) => {
    console.error('Error occurred while reading the file:', err);
  });

  const citiesPrices = new Map();
  rl.on('line', (line) => {
    try {
      const [city, product, price] = line.split(',');
      const priceValue = parseFloat(price);
      updateCityPrices(city, product, priceValue, citiesPrices);
    } catch (err) {
      console.error('Error occurred while processing line:', err);
    }
  });

  rl.on('close', () => {
    try {
      const cheapestCity = findCheapestCity(citiesPrices);
      const cheapestProducts = sortAndSliceCheapestProducts(cheapestCity);
      displayCheapestCity(cheapestCity, cheapestProducts);
    } catch (err) {
      console.error('Error occurred while processing:', err);
    }
  });
}

main();
