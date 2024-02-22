const fs = require('fs');

const data = fs.readFileSync('input.txt',"utf-8");
const fileStreamOut = fs.createWriteStream('output.txt');

let cityWithTotal = new Map();
let cityWithProduct = new Map();

data.split('\r\n').map(line => {

   const [city , product , price] = line.split(',');
      const productwithprice = {}
      productwithprice[product] = parseFloat(price);
      (cityWithProduct.get(city) || []).push(productwithprice)
      cityWithProduct.set(city,cityWithProduct.get(city) || []);
      cityWithTotal.set(city,(parseFloat(cityWithTotal.get(city)) + parseFloat(price) || parseFloat(price)));
  
})
//sort results and get first city
const firstCityHasSmallPrice = Array.from(cityWithTotal.entries()).sort((a,b) => a[1]-b[1])[0];

//write reulsts in file 
fileStreamOut.write(`${firstCityHasSmallPrice[0]}  ${firstCityHasSmallPrice[1]} \n`)

cityWithProduct.get(firstCityHasSmallPrice[0].trim()).sort((a,b) => {
   return parseFloat(Object.values(a)[0]) !== parseFloat(Object.values(b)[0]) ?
         parseFloat(Object.values(a)[0]) - parseFloat(Object.values(b)[0]) 
        : Object.keys(a)[0].localeCompare(Object.keys(b)[0])
}).slice(0,5).map(product => fileStreamOut.write(`${Object.keys(product)[0]} ${Object.values(product)[0]} \n`))