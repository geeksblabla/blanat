const os = require("node:os");
const fsAsync = require("node:fs/promises");
const fs = require("node:fs");
const workerThreads = require("worker_threads");

const MAX_CITY_NAME_LENGTH = 16;
const MAX_VEGETABLE_NAME_LENGTH = 17;
const MAX_PRICE_LENGTH = 6;
const MAX_LINE_LENGTH =
  MAX_CITY_NAME_LENGTH + MAX_VEGETABLE_NAME_LENGTH + MAX_PRICE_LENGTH;
const CHAR_SEPARATOR = ",".charCodeAt(0);
const CHAR_NEWLINE = "\n".charCodeAt(0);
const CHAR_DOT = ".".charCodeAt(0);

const CITY_COLUMN = 1;
const VEGETABLE_COLUMN = 2;
const PRICE_COLUMN = 3;
const KEY_SEPARATOR = "#";

const INPUT_FILE = "input.txt";
const OUTPUT_FILE = "output.txt";

const vegetableNames = getVegetableNames();

(function main() {
  if (workerThreads.isMainThread) {
    initMainThread();
  } else {
    const { fileName, start, end } = workerThreads.workerData;
    if (start > end - 1) {
      workerThreads.parentPort.postMessage([new Map(), new Map()]);
    } else {
      const readStream = fs.createReadStream(fileName, {
        start: start,
        end: end - 1,
      });

      parseStream(readStream);
    }
  }
})();

async function initMainThread() {
  const fileName = INPUT_FILE;

  const threadsCount = os.cpus().length;
  const file = await fsAsync.open(fileName);
  const { size } = await file.stat();

  const chunkSize = Math.max(Math.floor(size / threadsCount), 1);

  const chunkOffsets = [];

  let offset = 0;
  const bufFindNl = Buffer.alloc(MAX_LINE_LENGTH);

  while (true) {
    offset += chunkSize;

    if (offset >= size) {
      chunkOffsets.push(size);
      break;
    }

    await file.read(bufFindNl, 0, MAX_LINE_LENGTH, offset);

    const newLineIndex = bufFindNl.indexOf(CHAR_NEWLINE);
    bufFindNl.fill(0);

    if (newLineIndex === -1) {
      chunkOffsets.push(size);
      break;
    } else {
      offset += newLineIndex + 1;
      chunkOffsets.push(offset);
    }
  }

  await file.close();

  const totalsMap = new Map();
  const pricesMap = new Map();

  let stoppedWorkers = 0;

  for (let i = 0; i < chunkOffsets.length; i++) {
    const start = i === 0 ? 0 : chunkOffsets[i - 1];
    const end = chunkOffsets[i];

    const worker = new workerThreads.Worker(__filename, {
      workerData: {
        fileName,
        start,
        end,
      },
    });

    worker.on("message", ([totalsFromMessage, pricesFromMessage]) => {
      for (let [key, price] of pricesFromMessage.entries()) {
        const existing = pricesMap.get(key);

        if (!existing || price < existing) {
          pricesMap.set(key, price);
        }
      }

      for (let [city, total] of totalsFromMessage.entries()) {
        totalsMap.set(city, (totalsMap.get(city) || 0) + total);
      }
    });

    worker.on("error", (err) => {
      console.error(err);
    });

    worker.on("exit", (code) => {
      if (code !== 0) {
        new Error("Worker stopped");
      }

      stoppedWorkers++;

      if (stoppedWorkers === chunkOffsets.length) {
        tallyAndPrintResult(totalsMap, pricesMap);
      }
    });
  }
}

function tallyAndPrintResult(totalsMap, pricesMap) {
  let resultCity;
  let resultTotal;

  for (const [city, total] of totalsMap.entries()) {
    if (!resultCity || total < resultTotal) {
      resultCity = city;
      resultTotal = total;
    }
  }

  const vegetablePrices = vegetableNames.reduce((acc, vegetableName) => {
    const price = pricesMap.get(
      `${resultCity}${KEY_SEPARATOR}${vegetableName}`
    );
    return price ? [...acc, [vegetableName, price]] : acc;
  }, []);

  const result = [
    `${resultCity} ${round(resultTotal)}`,
    ...vegetablePrices
      .sort(([firstName, firstTotal], [secondName, secondTotal]) => {
        const result = firstTotal - secondTotal;

        return result !== 0 ? result : firstName.localeCompare(secondName);
      })
      .slice(0, 5)
      .map(([vegetable, price]) => `${vegetable} ${round(price)}`),
    "", // empty line at the end
  ].join("\n");

  fs.writeFileSync(OUTPUT_FILE, result, "utf-8");
}

function round(num) {
  return (num / 100).toFixed(2);
}

function parseStream(readStream) {
  let column = CITY_COLUMN;
  let cityBuffer = Buffer.allocUnsafe(MAX_CITY_NAME_LENGTH);
  let cityLength = 0;

  let vegetableBuffer = Buffer.allocUnsafe(MAX_VEGETABLE_NAME_LENGTH);
  let vegetableLength = 0;

  let priceBuffer = Buffer.allocUnsafe(MAX_PRICE_LENGTH);
  let priceLength = 0;

  const cityTotalsMap = new Map();
  const vegetablePricesMap = new Map();

  function parseChunk(chunk) {
    for (let i = 0; i < chunk.length; i++) {
      if (chunk[i] === CHAR_SEPARATOR) {
        column++;
      } else if (chunk[i] === CHAR_NEWLINE) {
        // finalized reading row
        const city = cityBuffer.toString("utf8", 0, cityLength);
        const vegetable = vegetableBuffer.toString("utf8", 0, vegetableLength);
        const price = parseFloatBufferIntoInt(priceBuffer, priceLength);

        const keyName = city + KEY_SEPARATOR + vegetable;
        const existingVegetablePrices = vegetablePricesMap.get(keyName);
        const existingCityTotals = cityTotalsMap.get(city);

        cityTotalsMap.set(city, (existingCityTotals || 0) + price);

        if (!existingVegetablePrices || price < existingVegetablePrices) {
          vegetablePricesMap.set(keyName, price);
        }

        column = CITY_COLUMN;
        vegetableLength = 0;
        cityLength = 0;
        priceLength = 0;
      } else if (column === CITY_COLUMN) {
        // read city
        cityBuffer[cityLength++] = chunk[i];
      } else if (column === VEGETABLE_COLUMN) {
        // read vegetable
        vegetableBuffer[vegetableLength++] = chunk[i];
      } else if (column === PRICE_COLUMN) {
        // read price
        priceBuffer[priceLength++] = chunk[i];
      }
    }
  }

  readStream.on("data", (chunk) => {
    parseChunk(chunk);
  });

  readStream.on("end", () => {
    workerThreads.parentPort.postMessage([cityTotalsMap, vegetablePricesMap]);
  });
}

function parseFloatBufferIntoInt(buffer, length) {
  let decimalIndex = buffer.findIndex((char) => char === CHAR_DOT);
  let result = 0;
  let multiplier = 1;

  for (let index = 0; index < length; index++) {
    if (index === decimalIndex) {
      multiplier = 1; // Reset the multiplier when encountering decimal point
    } else {
      result = result * 10 + parseOneDigit(buffer[index]);
      if (decimalIndex !== -1) {
        multiplier *= 0.1; // Adjust multiplier for digits after decimal point
      }
    }
  }

  return Math.round(result * multiplier * 100); // Apply multiplier to the result only once
}

function parseOneDigit(char) {
  return char - 0x30;
}

function getVegetableNames() {
  return [
    "Apple",
    "Banana",
    "Orange",
    "Strawberry",
    "Grapes",
    "Watermelon",
    "Pineapple",
    "Mango",
    "Kiwi",
    "Peach",
    "Plum",
    "Cherry",
    "Pear",
    "Blueberry",
    "Raspberry",
    "Blackberry",
    "Cantaloupe",
    "Honeydew",
    "Coconut",
    "Pomegranate",
    "Lemon",
    "Lime",
    "Grapefruit",
    "Avocado",
    "Papaya",
    "Guava",
    "Fig",
    "Passion_Fruit",
    "Apricot",
    "Nectarine",
    "Cucumber",
    "Carrot",
    "Broccoli",
    "Spinach",
    "Kale",
    "Lettuce",
    "Tomato",
    "Bell_Pepper",
    "Zucchini",
    "Eggplant",
    "Cabbage",
    "Cauliflower",
    "Brussels_Sprouts",
    "Radish",
    "Beet",
    "Asparagus",
    "Artichoke",
    "Green_Beans",
    "Peas",
    "Celery",
    "Onion",
    "Garlic",
    "Potato",
    "Sweet_Potato",
    "Yam",
    "Butternut_Squash",
    "Acorn_Squash",
    "Pumpkin",
    "Cranberry",
    "Goji_Berry",
    "Currant",
    "Date",
    "Clementine",
    "Cranberry",
    "Rhubarb",
    "Chard",
    "Collard_Greens",
    "Parsley",
    "Cilantro",
    "Mint",
    "Basil",
    "Thyme",
    "Rosemary",
    "Sage",
    "Dill",
    "Oregano",
    "Cantaloupe",
    "Honeydew",
    "Coconut",
    "Pomegranate",
    "Jackfruit",
    "Starfruit",
    "Persimmon",
    "Ginger",
    "Turnip",
    "Jicama",
    "Kohlrabi",
    "Watercress",
    "Okra",
    "Artichoke",
    "Plantain",
    "Cactus_Pear",
    "Kiwano",
    "Squash_Blossom",
    "Dragon_Fruit",
    "Parsnip",
    "Rutabaga",
    "Salsify",
    "Bok_Choy",
    "Endive",
  ];
}
