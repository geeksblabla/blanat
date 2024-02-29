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

    worker.on("message", (citiesTotals) => {
      for (const [city, { total, products }] of citiesTotals) {
        const currentCity = cities.get(city);
        if (!currentCity) {
          cities.set(city, { total, products });
        } else {
          currentCity.total += total;
          for (let [product, price] of products) {
            const savedProductPrice = currentCity.products.get(product);
            if (!savedProductPrice || price < savedProductPrice) {
              currentCity.products.set(product, price);
            }
          }
        }
      }
    });

    worker.on("exit", () => {
      completedWorkersCount++;
      if (completedWorkersCount === workerCount) {
        let output = [];
        const [cheapestCity, cheapestCityData] = [...cities].sort(
          function ([_, { total: totalA }], [__, { total: totalB }]) {
            return totalA - totalB;
          }
        )[0];
        output.push(
          `${cheapestCity} ${(cheapestCityData.total / 100).toFixed(2)}`
        );
        [...cheapestCityData.products]
          .sort(sortItems)
          .slice(0, 5)
          .forEach(([product, price]) => {
            output.push(`${product} ${(price / 100).toFixed(2)}`);
          });
        fs.writeFileSync("output.txt", output.join("\n"), "utf-8");
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

let cityBuffer = Buffer.allocUnsafe(APROX_NAME_SIZE);
let productBuffer = Buffer.allocUnsafe(APROX_NAME_SIZE);
let priceBuffer = Buffer.allocUnsafe(PRICE_SIZE);

function parseStream(readStream) {
  let col = 1;
  let citySize = 0;
  let productSize = 0;
  let priceSize = 0;

  const wcities = new Map();

  readStream.on("data", (chunk) => {
    for (let i = 0; i < chunk.length; i++) {
      if (chunk[i] === LINE_SEPARATOR_CHAR) {
        col++;
      } else if (chunk[i] === NEWLINE_CHAR) {
        const city = cityBuffer.toString("utf8", 0, citySize);
        const product = productBuffer.toString("utf8", 0, productSize);
        const price = bufferToInt(priceBuffer, priceSize);

        const lastSavedCity = wcities.get(city);
        if (!lastSavedCity) {
          wcities.set(city, { total: price, products: new Map([[product, price]]) });
        } else {
          lastSavedCity.total += price;
          const lastSavedProduct = lastSavedCity.products.get(product);
          if (
            !lastSavedProduct || price < lastSavedProduct
          ) {
            lastSavedCity.products.set(product, price);
          }
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
    parentPort.postMessage(wcities);
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
