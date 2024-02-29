// by RedOne KaiTo <radouane khouadri>

package main

import (
	"bufio"
	"fmt"
	"os"
	"runtime"
	"strconv"
	"strings"
	"sync"
)

const (
	inputFile  = "input.txt"
	outputFile = "output.txt"
	numWorkers = 20
)

type Entry struct {
	city  string
	item  string
	price float64
}

func worker(lines <-chan string, results chan<- Entry, wg *sync.WaitGroup) {
	defer wg.Done()
	for line := range lines {
		fields := strings.Split(line, ",")
		price, _ := strconv.ParseFloat(fields[2], 64)
		results <- Entry{city: fields[0], item: fields[1], price: price}
	}
}

func main() {
	file, err := os.Open(inputFile)
	if err != nil {
		fmt.Println("Error opening input file:", err)
		return
	}
	defer file.Close()

	lines := make(chan string, numWorkers*2) // Buffered channel
	results := make(chan Entry, numWorkers*2) // Buffered channel
	var wg sync.WaitGroup

	// Use all available CPU cores
	runtime.GOMAXPROCS(runtime.NumCPU())

	for i := 0; i < numWorkers; i++ {
		wg.Add(1)
		go worker(lines, results, &wg)
	}

	go func() {
		scanner := bufio.NewScanner(file)
		for scanner.Scan() {
			lines <- scanner.Text()
		}
		close(lines)
	}()

	go func() {
		wg.Wait()
		close(results)
	}()

	cityPrices := make(map[string]map[string]float64)
	for result := range results {
		if _, ok := cityPrices[result.city]; !ok {
			cityPrices[result.city] = make(map[string]float64)
		}
		cityPrices[result.city][result.item] += result.price
	}

	outputFile, err := os.Create(outputFile)
	if err != nil {
		fmt.Println("Error creating output file:", err)
		return
	}
	defer outputFile.Close()

	for city, items := range cityPrices {
		total := 0.0
		for _, price := range items {
			total += price
		}
		fmt.Fprintf(outputFile, "%s %.2f\n", city, total)
		for item, price := range items {
			fmt.Fprintf(outputFile, "%s %.2f\n", item, price)
		}
	}
}
