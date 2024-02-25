const fs = require('fs');
const readline = require('readline');

const highWaterMarkBytes = 16 * 1024 * 1024; // 16 MB
const inputStream = fs.createReadStream('input.txt', {
  highWaterMark: highWaterMarkBytes,
});

const outputStream = fs.createWriteStream('output.txt');

const updateCityPrices = (city, product, priceValue, citiesPrices) => {
  let cityData = citiesPrices.get(city);
  if (!cityData) {
    cityData = { sum: 0, products: new Map() };
    citiesPrices.set(city, cityData);
  }

  const currentPrice = cityData.products.get(product);
  if (currentPrice === undefined || priceValue < currentPrice) {
    cityData.products.set(product, priceValue);
  }
  cityData.sum += priceValue;
};

const findCheapestCity = (citiesPrices) => {
  let cheapestCity = {};
  let minSum = Infinity;

  for (const [city, data] of citiesPrices.entries()) {
    if (data.sum < minSum) {
      minSum = data.sum;
      cheapestCity = { city, citySum: data.sum, products: data.products };
    }
  }

  return cheapestCity;
};

const sortAndSliceCheapestProducts = (products) => {
  return Array.from(products.entries())
    .sort(([nameA, priceA], [nameB, priceB]) => {
      return priceA - priceB || nameA.localeCompare(nameB);
    })
    .slice(0, 5)
    .map(([name, price]) => ({ name, price }));
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
