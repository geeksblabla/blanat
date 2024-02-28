const fs = require("node:fs");
const os = require("node:os");
const {
  Worker,
  isMainThread,
  parentPort,
  workerData,
} = require("node:worker_threads");

const APROX_NAME_SIZE = 25;
const PRICE_SIZE = 6;
const APROX_LINE_SIZE = APROX_NAME_SIZE * 2 + PRICE_SIZE;
const LINE_SEPARATOR_CHAR = ",".charCodeAt(0);
const NEWLINE_CHAR = "\n".charCodeAt(0);

const cities = new Map();
const products = new Map();
const fileName = "input.txt";

function sortItems([keyA, priceA], [keyB, priceB]) {
  if (priceA === priceB) {
    return keyA.localeCompare(keyB);
  } else {
    return priceA - priceB;
  }
}

if (isMainThread) {
  const threadsNumber = os.cpus().length;
  let completedWorkersCount = 0;
  let workerCount = 0;
  const { size } = fs.statSync(fileName);
  const fd = fs.openSync(fileName);
  const chunkSize = Math.ceil(size / threadsNumber);
  let start = 0;
  let end = 0;
  let isLastChunk = false;
  const buffer = Buffer.alloc(APROX_LINE_SIZE);

  while (end < size) {
    end += chunkSize;

    if (end >= size) {
      end = size;
      isLastChunk = true;
    }

    if (!isLastChunk) {
      fs.readSync(fd, buffer, 0, APROX_LINE_SIZE, end);
      const newLineIndex = buffer.indexOf("\n".charCodeAt());
      buffer.fill(0);
      if (newLineIndex !== -1) {
        end += newLineIndex;
      }
    }

    const worker = new Worker(__filename, {
      workerData: {
        fileName,
        start,
        end,
      },
    });

    workerCount++;

    worker.on("message", ([citiesTotals, productsPrices]) => {
      for (let [product, price] of productsPrices.entries()) {
        const savedProductPrice = products.get(product);
        if (!savedProductPrice || price < savedProductPrice) {
          products.set(product, price);
        }
      }

      for (let [city, total] of citiesTotals.entries()) {
        cities.set(city, (cities.get(city) || 0) + total);
      }
    });

    worker.on("exit", () => {
      completedWorkersCount++;
      if (completedWorkersCount === workerCount) {
        let output = [];
        const [cheapestCity, cheapestCityPrice] = [...cities.entries()].sort(
          sortItems
        )[0];
        output.push(`${cheapestCity} ${(cheapestCityPrice / 100).toFixed(2)}`);
        [...products.entries()]
          .filter(([key]) => key.startsWith(cheapestCity))
          .sort(sortItems)
          .slice(0, 5)
          .forEach(([product, price]) => {
            output.push(
              `${product.slice(cheapestCity.length + 1)} ${(
                price / 100
              ).toFixed(2)}`
            );
          });
        fs.writeFileSync("output.txt", output.join("\n"), "utf-8");
        console.log(output.join("\n"));
      }
    });

    start = end + 1;
  }
} else {
  const { fileName, start, end } = workerData;
  const stream = fs.createReadStream(fileName, {
    start: start,
    end: end,
  });

  parseStream(stream);
}

function parseStream(readStream) {
  let col = 1;
  let cityBuffer = Buffer.allocUnsafe(APROX_NAME_SIZE);
  let citySize = 0;

  let productBuffer = Buffer.allocUnsafe(APROX_NAME_SIZE);
  let productSize = 0;

  let priceBuffer = Buffer.allocUnsafe(PRICE_SIZE);
  let priceSize = 0;

  const wcities = new Map();
  const wproducts = new Map();

  readStream.on("data", (chunk) => {
    for (let i = 0; i < chunk.length; i++) {
      if (chunk[i] === LINE_SEPARATOR_CHAR) {
        col++;
      } else if (chunk[i] === NEWLINE_CHAR) {
        const city = cityBuffer.toString("utf8", 0, citySize);
        const product = productBuffer.toString("utf8", 0, productSize);
        const price = bufferToInt(priceBuffer, priceSize);

        const lastSavedCityTotal = wcities.get(city);
        wcities.set(
          city,
          lastSavedCityTotal ? lastSavedCityTotal + price : price
        );

        const productKey = `${city}|${product}`;
        const lastSavedProductPrice = wproducts.get(productKey);
        if (
          !lastSavedProductPrice ||
          (lastSavedProductPrice && price < lastSavedProductPrice)
        ) {
          wproducts.set(productKey, price);
        }

        col = 1;
        citySize = 0;
        productSize = 0;
        priceSize = 0;
      } else if (col === 1) {
        cityBuffer[citySize++] = chunk[i];
      } else if (col === 2) {
        productBuffer[productSize++] = chunk[i];
      } else if (col === 3) {
        priceBuffer[priceSize++] = chunk[i];
      }
    }
  });

  readStream.on("end", () => {
    parentPort.postMessage([wcities, wproducts]);
  });
}

function bufferToInt(buffer, length) {
  let decimalPointIndex = buffer.findIndex(
    (char) => char === ".".charCodeAt(0)
  );
  let result = 0;
  let multiplier = 1;

  for (let index = 0; index < length; index++) {
    if (index === decimalPointIndex) {
      multiplier = 1;
    } else {
      result = result * 10 + (buffer[index] - 0x30);
      if (decimalPointIndex !== -1) {
        multiplier *= 0.1;
      }
    }
  }

  return Math.round(result * multiplier * 100);
}
