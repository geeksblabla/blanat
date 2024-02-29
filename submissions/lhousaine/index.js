const fs = require('fs');
const os = require('os');
const {
  Worker,
  isMainThread,
  parentPort,
  workerData,
} = require('worker_threads');

if (isMainThread) {
  const inputFile = 'input.txt';
  const outputFile = 'output.txt';
  const numCPUs = os.cpus().length;
  const { size } = fs.statSync(inputFile);
  const chunkSize = Math.max(Math.floor(size / numCPUs), 1);

  const fd = fs.openSync(inputFile);
  const chunkOffsets = [];
  const APROX_LINE_LENGTH = 100;
  const buffer = Buffer.alloc(APROX_LINE_LENGTH);

  let offset = 0;
  while (true) {
    offset += chunkSize;

    if (offset >= size) {
      chunkOffsets.push(size);
      break;
    }

    fs.readSync(fd, buffer, 0, APROX_LINE_LENGTH, offset);
    const newLineIndex = buffer.indexOf('\n'.charCodeAt(0));
    buffer.fill(0);

    if (newLineIndex === -1) {
      chunkOffsets.push(size);
      break;
    } else {
      offset += newLineIndex + 1;
      chunkOffsets.push(offset);
    }
  }
  fs.closeSync(fd);

  const citiesData = new Map();
  const workersData = [];
  let workersCompleted = 0;

  for (let i = 0; i < chunkOffsets.length; i++) {
    const start = i === 0 ? 0 : chunkOffsets[i - 1];
    const end = chunkOffsets[i];

    const worker = new Worker(__filename, {
      workerData: {
        inputFile,
        start,
        end,
      },
    });

    worker.on('message', (message) => {
      if (message.workerCitiesData) {
        const { workerCitiesData } = message;
        for (const [city, data] of workerCitiesData) {
          if (!citiesData.has(city)) {
            citiesData.set(city, { sum: 0 });
          }
          const mergedCityData = citiesData.get(city);
          mergedCityData.sum += data.sum;
        }
        workersData.push(workerCitiesData);
      }
    });

    worker.on('exit', () => {
      workersCompleted++;
      if (workersCompleted === numCPUs) {
        const { city, sum, products } = getCheapestCityDetails(
          citiesData,
          workersData
        );
        const cheapestProducts = findCityCheapestFiveProducts(products);
        displayCheapestCity(city, sum, cheapestProducts);
      }
    });

    worker.on('error', (err) => {
      console.error(`Worker error: ${err}`);
    });
  }

  const getCheapestCityDetails = (citiesData, workersData) => {
    let minSum = Infinity;
    let minCity = null;
    const minProducts = new Map();
    for (const [city, data] of citiesData) {
      if (data.sum < minSum) {
        minSum = data.sum;
        minCity = city;
      }
    }

    for (const workerData of workersData) {
      const workerCityData = workerData.get(minCity);
      if (workerCityData?.products) {
        for (const [product, price] of workerCityData.products) {
          if (!minProducts.has(product) || price < minProducts.get(product)) {
            minProducts.set(product, price);
          }
        }
      }
    }

    return {
      city: minCity,
      sum: minSum,
      products: minProducts,
    };
  };

  const findCityCheapestFiveProducts = (products) => {
    return Array.from(products)
      .sort(([nameA, priceA], [nameB, priceB]) => {
        return priceA - priceB || nameA.localeCompare(nameB);
      })
      .slice(0, 5)
      .map(([name, price]) => ({ name, price }));
  };

  const displayCheapestCity = (city, citySum, cheapestProducts) => {
    const outputStream = fs.createWriteStream(outputFile);
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
} else {
  const parseLineData = (line, wCitiesData) => {
    const [city, product, price] = line.split(',');
    const priceValue = parseFloat(price);
    if (!wCitiesData.has(city)) {
      wCitiesData.set(city, { sum: 0, products: new Map() });
    }
    const citySum = wCitiesData.get(city);
    citySum.sum += priceValue;
    if (
      !citySum.products.has(product) ||
      priceValue < citySum.products.get(product)
    ) {
      citySum.products.set(product, priceValue);
    }
  };

  const { inputFile, start, end } = workerData;
  const rs = fs.createReadStream(inputFile, {
    encoding: 'utf-8',
    start,
    end,
  });
  const workerCitiesData = new Map();
  let buffer = '';

  rs.on('data', function (chunk) {
    const lines = (buffer + chunk).split(/\r?\n/g);
    buffer = lines.pop();
    for (let i = 0; i < lines.length; ++i) {
      parseLineData(lines[i], workerCitiesData);
    }
  });

  rs.on('end', function () {
    parentPort.postMessage({ workerCitiesData });
  });
}
