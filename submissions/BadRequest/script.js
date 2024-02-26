const fs = require('fs')
const readline = require('readline')

const start = Date.now()
const citiesTotalPrice = new Map()
const productsPricesByCity = {}

const readStream = fs.createReadStream('input.txt')
const rl = readline.createInterface({
  input: readStream,
})

const sortEntries = (entries) => {
  entries.sort(([keyA, valueA], [keyB, valueB]) => {
    // Compare values
    if (valueA !== valueB) {
      return valueA - valueB
    }
    // If values are equal, compare keys in asc order
    return keyA.localeCompare(keyB)
  })

  return entries
}

rl.on('line', (line) => {
  const [city, product, price] = line.split(',')

  // Update citiesTotalPrice
  const currentSum = citiesTotalPrice.get(city) || 0
  citiesTotalPrice.set(city, currentSum + Number(price))

  // Update productsPricesByCity
  productsPricesByCity[city] = productsPricesByCity[city] || {}
  productsPricesByCity[city][product] = Math.min(
    Number(price),
    productsPricesByCity[city][product] || Number.MAX_SAFE_INTEGER
  )
})

rl.on('close', () => {
  const citiesTotalPriceArray = Array.from(citiesTotalPrice)

  // Find the cheapest city
  const cheapestCityEntry = citiesTotalPriceArray.reduce((min, currentValue) =>
    currentValue[1] < min[1] ? currentValue : min
  )

  const data = productsPricesByCity[cheapestCityEntry[0]]

  // Sort the product prices
  const sortedProducts = sortEntries(Object.entries(data)).slice(0, 5)

  const writeStream = fs.createWriteStream('output.txt')
  writeStream.write(
    `${cheapestCityEntry[0]} ${cheapestCityEntry[1].toFixed(2)}\n`
  )

  writeStream.write(
    sortedProducts
      .map(([product, price]) => `${product} ${price.toFixed(2)}`)
      .join('\n') + '\n'
  )

  const end = Date.now()
  console.log(`Execution time: ${end - start} ms`)
})
