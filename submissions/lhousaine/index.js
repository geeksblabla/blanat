const fs = require('fs');
const readline = require('readline');

const highWaterMarkBytes = 8 * 1024 * 1024; // 8 MB
const inputStream = fs.createReadStream('../../input.txt', {
  highWaterMark: highWaterMarkBytes,
});
const outputStream = fs.createWriteStream('output.txt');

const getCityProductsSum = (cityProducts) =>
  cityProducts.reduce((acc, current) => acc + current.price, 0);

const updateCityPrices = (city, product, priceValue, citiesPrices) => {
  const cityData = citiesPrices.get(city);
  if (cityData) {
    cityData.push({
      name: product,
      price: priceValue,
    });
  } else {
    citiesPrices.set(city, [{ name: product, price: priceValue }]);
  }
};

const findCheapestCity = (citiesPrices) => {
  let cheapestCity = {};
  let minSum = Infinity;

  for (const [city, products] of citiesPrices.entries()) {
    const citySum = getCityProductsSum(products);
    if (citySum < minSum) {
      minSum = citySum;
      cheapestCity = { city, citySum, products };
    }
  }

  return cheapestCity;
};

const sortAndSliceCheapestProducts = (products) => {
  products.sort((product1, product2) => {
    return (
      product1.price - product2.price ||
      product1.name.localeCompare(product2.name)
    );
  });

  return products
    .filter((product1, index) => products[index + 1]?.name !== product1.name)
    .slice(0, 5);
};

const displayCheapestCity = (city, citySum, cheapestProducts) => {
  const outputResult = [];
  outputResult.push(`${city} ${citySum.toFixed(2)}`);

  cheapestProducts.forEach((product) => {
    outputResult.push(`${product.name} ${product.price.toFixed(2)}`);
  });

  outputStream.write(outputResult.join('\n'), (err) => {
    if (err) {
      console.error('Error occurred while writing to output file:', err);
    } else {
      console.log('Output file has been written successfully.');
    }
    outputStream.end();
  });
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
      const { city, citySum, products } = findCheapestCity(citiesPrices);
      const cheapestProducts = sortAndSliceCheapestProducts(products);
      displayCheapestCity(city, citySum, cheapestProducts);
    } catch (err) {
      console.error('Error occurred while processing:', err);
    }
  });
}

main();
