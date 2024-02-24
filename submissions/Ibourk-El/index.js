const fs = require("fs");
const readline = require("readline");

const inputFile = fs.createReadStream("input.txt");
const outputFile = fs.createWriteStream("output.txt");

const bestCity = async () => {
  try {
    const rl = readline.createInterface({
      input: inputFile,
      crlfDelay: Infinity,
    });
    let cityData = {};
    for await (const line of rl) {
      if (line !== "") {
        const [city, product, priceStr] = line.split(",");
        const price = parseFloat(priceStr);
        if (city in cityData) {
          // check if the product is in list
          if (product in cityData[city].product) {
            let ind = cityData[city].product[product];
            // change the value of the product to cheapes one
            if (cityData[city].price[ind].price > price) {
              cityData[city].price[ind].price = price;
            }
          } else {
            // add new product
            cityData[city].product[product] = cityData[city].index;
            cityData[city].price.push({ name: product, price: price });
            cityData[city].index += 1;
          }
          cityData[city].sum += price;
        } else {
          // add new city
          cityData[city] = {
            product: {},
            price: [{ name: product, price: price }],
            sum: price,
            index: 1,
          };
          cityData[city].product[product] = 0;
        }
      }
    }

    let smallestSum = Infinity;
    let bestCity = "";
    for (let city in cityData) {
      if (cityData[city].sum < smallestSum) {
        smallestSum = cityData[city].sum;
        bestCity = city;
      }
    }

    cityData[bestCity].price.sort((a, b) => {
      if (a.price === b.price) {
        if (a.name > b.name) return 1;
        else if (a.name < b.name) return -1;
        else return 0;
      }
      return a.price - b.price;
    });

    let result = `${bestCity} ${smallestSum.toFixed(2)} \n`;
    let i = 0;
    while (i < 5) {
      let t = cityData[bestCity].price[i].name;
      result += t + " " + cityData[bestCity].price[i].price.toFixed(2) + "\n";
      i++;
    }

    outputFile.write(result);
    outputFile.end();
  } catch (e) {
    console.log(e);
  }
};

bestCity();
