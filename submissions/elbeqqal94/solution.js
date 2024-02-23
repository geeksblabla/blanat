const fs = require('fs');
const readline = require('readline');

const INPUT_FILE = '../../input.txt';
const OUTPUT_FILE = '../../output.txt';

const inputStream = fs.createReadStream(INPUT_FILE, { encoding: 'utf8' });
const outputStream = fs.createWriteStream(OUTPUT_FILE);

const rl = readline.createInterface({
	input: inputStream,
	output: outputStream,
	crlfDelay: Infinity,
});

let resultMap = new Map();

const startTime = process.hrtime();

rl.on('line', (line) => {
	const data = line.split(',');
	const city = data[0];
	const productName = data[1];
	const productPrice = parseFloat(data[2]);

	if (resultMap.has(city)) {
		const currentData = resultMap.get(city);
		const newPrice = currentData.price + productPrice;
		if (!currentData.products[productName]) {
			currentData.products[productName] = productPrice;
		} else {
			if (currentData.products[productName] > productPrice) {
				currentData.products[productName] = productPrice;
			}
		}
		const updatedProducts = currentData.products;
		resultMap.set(city, { price: newPrice, products: updatedProducts });
	} else {
		let products = {};
		products[productName] = productPrice;
		resultMap.set(city, {
			price: productPrice,
			products,
		});
	}
});

rl.on('close', () => {
	const min = [...resultMap.entries()].reduce(
		(min, current) => {
			const currentPrice = current[1].price;
			if (currentPrice < min.price) {
				return { price: currentPrice, data: current };
			} else {
				return min;
			}
		},
		{ price: Infinity, data: null }
	);

	const finalData = min.data;

	let outputData = `${finalData[0]} ${finalData[1].price.toFixed(2)}\n`;
	const convertedData = Object.entries(finalData[1].products);

	convertedData.sort((a, b) => {
		if (a[1] === b[1]) {
			return a[0].localeCompare(b[0]);
		} else {
			return a[1] - b[1];
		}
	});

	const firstFiveElements = convertedData.slice(0, 5);
	outputData += firstFiveElements
		.map((product) => `${product[0]} ${product[1].toFixed(2)}`)
		.join('\n');

	outputStream.write(outputData);
	outputStream.end();

	const endTime = process.hrtime(startTime);
	const execTimeInMs = (endTime[0] * 1000 + endTime[1] / 1000000).toFixed(2);
	console.log(`Execution time: ${execTimeInMs} ms`);
});
