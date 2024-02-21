const fs = require('fs');
const path = require('path');

const filePath = path.relative('/', 'm.txt');

fs.readFile(filePath, 'utf8', (err, data) => {
	if (err) {
		console.error('File reading error:', err);
		return;
	}

	const rows = data.split('\r\n');

	let resultMap = new Map();

	rows.forEach((row) => {
		const data = row.split(',');
		const city = data[0];
		const productName = data[1];
		const productPrice = parseFloat(data[2]);

		if (resultMap.has(city)) {
			const currentData = resultMap.get(city);
			const newPrice = currentData.price + productPrice;
			if (!currentData.products[productName]) {
				// If the product name does not exist, add the new product
				currentData.products[productName] = productPrice;
				//console.log(`Product '${name}' added with price ${price}.`);
			} else {
				//console.log(`Product '${name}' already exists.`);
				if (currentData.products[productName] > productPrice) {
					currentData.products[productName] = productPrice;
				}
			}
			const updatedProducts = currentData.products;
			resultMap.set(city, { price: newPrice, products: updatedProducts });
		} else {
			let products = [];
			products[productName] = productPrice;
			resultMap.set(city, {
				price: productPrice,
				products,
			});
		}
	});

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

	fs.writeFile('output.txt', outputData, 'utf8', (err) => {
		if (err) {
			console.error('Error writing to output.txt:', err);
			return;
		}
	});
});
