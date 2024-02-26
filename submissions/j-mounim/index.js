const fs = require("node:fs");
const os = require("node:os");
const readline = require("node:readline");
const {
  Worker,
  isMainThread,
  parentPort,
  workerData,
} = require("node:worker_threads");

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
  const { size } = fs.statSync(fileName);
  const fd = fs.openSync(fileName);
  const chunkSize = Math.floor(size / threadsNumber);

  const chunkOffsets = [];
  let offset = 0;
  const APROX_LINE_LENGTH = 100;
  const buffer = Buffer.alloc(APROX_LINE_LENGTH);

  while (true) {
    offset += chunkSize;

    if (offset >= size) {
      chunkOffsets.push(size);
      break;
    }

    fs.readSync(fd, buffer, 0, APROX_LINE_LENGTH, offset);
    const newLineIndex = buffer.indexOf("\n".charCodeAt(0));
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

  for (let i = 0; i < chunkOffsets.length; i++) {
    const start = i === 0 ? 0 : chunkOffsets[i - 1];
    const end = chunkOffsets[i];

    const worker = new Worker(__filename, {
      workerData: {
        fileName,
        start,
        end,
      },
    });

    worker.on("message", ([citiesTotals, productsPrices]) => {
      for (let [product, price] of productsPrices.entries()) {
        if (!products.has(product) || price < products.get(product)) {
          products.set(product, price);
        }
      }

      for (let [city, total] of citiesTotals.entries()) {
        cities.set(city, (cities.get(city) || 0) + total);
      }

      worker.on("exit", () => {
        completedWorkersCount++;
        if (completedWorkersCount === chunkOffsets.length) {
          let output = [];
          const [cheapestCity, cheapestCityPrice] = [...cities.entries()].sort(
            sortItems
          )[0];
          output.push(
            `${cheapestCity} ${Number(cheapestCityPrice).toFixed(2)}`
          );
          [...products.entries()]
            .filter(([key]) => key.startsWith(cheapestCity))
            .sort(sortItems)
            .slice(0, 5)
            .forEach(([product, price]) => {
              output.push(
                `${product.slice(cheapestCity.length + 1)} ${Number(
                  price
                ).toFixed(2)}`
              );
            });
          fs.writeFileSync("output.txt", output.join("\n"), "utf-8");
        }
      });
    });
  }
} else {
  const { fileName, start, end } = workerData;
  const stream = fs.createReadStream(fileName, {
    start: start,
    end: end - 1,
  });

  const reader = readline.createInterface({
    input: stream,
    crlfDelay: Infinity,
  });

  const wcities = new Map();
  const wproducts = new Map();

  reader.on("line", function (line) {
    const [city, product, price] = line.split(",");
    const parsedNumber = Number(price);

    const lastSavedCityPrice = wcities.get(city);
    wcities.set(city, lastSavedCityPrice ? lastSavedCityPrice + parsedNumber : parsedNumber);

    const productKey = `${city}|${product}`;
    const lastSavedProductPrice = wproducts.get(productKey);

    if (
      !lastSavedProductPrice ||
      (lastSavedProductPrice && parsedNumber < lastSavedProductPrice)
    ) {
      wproducts.set(productKey, parsedNumber);
    }
  });

  reader.on("close", () => {
    parentPort.postMessage([wcities, wproducts]);
  });
}
