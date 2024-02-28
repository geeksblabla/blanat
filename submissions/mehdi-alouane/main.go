package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
)

type Product struct {
	Name  string
	Price float64
}

type City struct {
	Name     string
	Products map[string]float64
	Total    float64
}

func main() {
	inputFile := "input.txt"
	outputFile := "output.txt"

	cities := readInput(inputFile)
	cheapestCity, cheapestProducts := findCheapestCity(cities)
	writeOutput(outputFile, cheapestCity, cheapestProducts)
}

func readInput(filename string) map[string]*City {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Println("Error opening input file:", err)
		os.Exit(1)
	}
	defer file.Close()

	cities := make(map[string]*City)

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		parts := strings.Split(line, ",")

		cityName := parts[0]
		productName := parts[1]
		price, err := strconv.ParseFloat(parts[2], 64)
		if err != nil {
			fmt.Println("Error parsing price:", err)
			continue
		}

		city, exists := cities[cityName]
		if !exists {
			city = &City{Name: cityName, Products: make(map[string]float64)}
			cities[cityName] = city
		}

		if existingPrice, exists := city.Products[productName]; !exists || price < existingPrice {
			city.Products[productName] = price
		}

		city.Total += price
	}

	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading input file:", err)
		os.Exit(1)
	}

	return cities
}

func findCheapestCity(cities map[string]*City) (*City, []Product) {
	var cheapestCity *City
	cheapestTotalPrice := float64(9999999)

	for _, city := range cities {
		if city.Total < cheapestTotalPrice {
			cheapestCity = city
			cheapestTotalPrice = city.Total
		}
	}

	var cheapestProducts []Product
	for name, price := range cheapestCity.Products {
		cheapestProducts = append(cheapestProducts, Product{Name: name, Price: price})
	}

	sort.Slice(cheapestProducts, func(i, j int) bool {
		if cheapestProducts[i].Price == cheapestProducts[j].Price {
			return cheapestProducts[i].Name < cheapestProducts[j].Name
		}
		return cheapestProducts[i].Price < cheapestProducts[j].Price
	})

	return cheapestCity, cheapestProducts[:5]
}

func writeOutput(filename string, cheapestCity *City, cheapestProducts []Product) {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Println("Error creating output file:", err)
		os.Exit(1)
	}
	defer file.Close()

	writer := bufio.NewWriter(file)
	defer writer.Flush()

	_, _ = fmt.Fprintf(writer, "%s %.2f\n", cheapestCity.Name, cheapestCity.Total)
	for _, product := range cheapestProducts {
		_, _ = fmt.Fprintf(writer, "%s %.2f\n", product.Name, product.Price)
	}
}
