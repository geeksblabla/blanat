package main

import (
	"fmt"
	"log"
	"os"
	"runtime/debug"
	"runtime/pprof"
	"runtime/trace"
	"sort"
	"sync"
	"syscall"
	"unsafe"
)

const (
	inputFile           = "input.txt"
	maxConstraintsCount = 101
	concurrency         = 32
	profile             = false
	memProfile          = false
)

var (
	fruits = []string{"Apple", "Banana", "Orange", "Strawberry", "Grapes",
		"Watermelon", "Pineapple", "Mango", "Kiwi", "Peach",
		"Plum", "Cherry", "Pear", "Blueberry", "Raspberry",
		"Blackberry", "Cantaloupe", "Honeydew", "Coconut", "Pomegranate",
		"Lemon", "Lime", "Grapefruit", "Avocado", "Papaya",
		"Guava", "Fig", "Passion_Fruit", "Apricot", "Nectarine",
		"Cucumber", "Carrot", "Broccoli", "Spinach", "Kale",
		"Lettuce", "Tomato", "Bell_Pepper", "Zucchini", "Eggplant",
		"Cabbage", "Cauliflower", "Brussels_Sprouts", "Radish", "Beet",
		"Asparagus", "Artichoke", "Green_Beans", "Peas", "Celery",
		"Onion", "Garlic", "Potato", "Sweet_Potato", "Yam",
		"Butternut_Squash", "Acorn_Squash", "Pumpkin", "Cranberry", "Goji_Berry",
		"Currant", "Date", "Clementine", "Cranberry", "Rhubarb",
		"Chard", "Collard_Greens", "Parsley", "Cilantro", "Mint",
		"Basil", "Thyme", "Rosemary", "Sage", "Dill",
		"Oregano", "Cantaloupe", "Honeydew", "Coconut", "Pomegranate",
		"Jackfruit", "Starfruit", "Persimmon", "Ginger", "Turnip",
		"Jicama", "Kohlrabi", "Watercress", "Okra", "Artichoke",
		"Plantain", "Cactus_Pear", "Kiwano", "Squash_Blossom", "Dragon_Fruit",
		"Parsnip", "Rutabaga", "Salsify", "Bok_Choy", "Endive"}

	cities = []string{"Casablanca", "Rabat", "Marrakech", "Fes", "Tangier",
		"Agadir", "Meknes", "Oujda", "Kenitra", "Tetouan",
		"Safi", "El_Jadida", "Beni_Mellal", "Errachidia",
		"Taza", "Essaouira", "Khouribga", "Guelmim",
		"Jorf_El_Melha", "Laayoune", "Ksar_El_Kebir", "Sale", "Bir_Lehlou",
		"Arfoud", "Temara", "Mohammedia", "Settat",
		"Béni_Mellal", "Nador", "Kalaat_MGouna",
		"Chichaoua", "Chefchaouen", "Al_Hoceima", "Taourirt",
		"Taroudant", "Guelta_Zemmur", "Dakhla", "Laâyoune",
		"Tiznit", "Tinghir", "Ifrane", "Azrou", "Bab_Taza",
		"Berrechid", "Sidi_Slimane", "Souk_Larbaa", "Tiflet", "Sidi_Bennour",
		"Larache", "Tan-Tan", "Sidi_Ifni", "Goulmima",
		"Midelt", "Figuig", "Azilal", "Jerada", "Youssoufia",
		"Ksar_es_Seghir", "Tichka", "Ait_Melloul",
		"Layoune", "Ben_guerir", "Ouarzazate", "Inezgane",
		"Oujda_Angad", "Sefrou", "Aourir",
		"Oulad_Teima", "Tichla", "Bni_Hadifa",
		"Fquih_Ben_Salah", "Guercif", "Bouarfa", "Demnate",
		"Ahfir", "Berkane", "Akhfenir", "Boulemane",
		"Khenifra", "Bir_Anzerane", "Assa", "Smara", "Boujdour",
		"Tarfaya", "Ouazzane", "Zagora", "had_soualem",
		"Saidia", "Bab_Berred", "Midar", "Moulay_Bousselham",
		"Khemisset", "Guerguerat", "Asilah", "Sidi_Bouzid", "Tafraout",
		"Imzouren", "Zemamra", "Sidi_Kacem", "Drarga", "Skhirate"}

	fidx = createIndex(fruits, hashf, 4753)

	cidx = createIndex(cities, hashc, 12545)
)

func assert(err error) {
	if err != nil {
		panic(err)
	}
}
func createIndex(data []string, hash func(string) uint64, size uint32) []int {
	table := make([]int, size)
	for pos, val := range data {
		h := hash(val)
		table[h] = pos
	}
	return table
}

type task struct {
	data     []byte
	products [][]int64
	sum      []int64
}

func newTask(data []byte) *task {
	products := make([][]int64, maxConstraintsCount)
	for i := 0; i < maxConstraintsCount; i++ {
		products[i] = make([]int64, maxConstraintsCount)
		for j := 0; j < maxConstraintsCount; j++ {
			products[i][j] = 100001
		}
	}
	sum := make([]int64, maxConstraintsCount)
	return &task{
		products: products,
		sum:      sum,
		data:     data,
	}
}

func (t *task) run() {
	var (
		price      int
		sep1, sep2 int
		cursor     int
	)

	datalen := len(t.data)
	for {
		if cursor > datalen-2 {
			return
		}
		start := cursor
		cursor += 11
		for t.data[cursor] != '\n' {
			cursor++
			if t.data[cursor] == ',' {
				sep1 = start - cursor
			}
		}
		cursor++
		record := t.data[start : cursor-1]

		// todo: optimise

		price = (int(record[len(record)-1]) - '0')
		pos := 0
		k := 10
		for i := len(record) - 2; i > 0; i-- {
			if record[i] == ',' {
				sep2 = i
				if len(record)-pos == 2 {
					price = price * 10
				}
				break
			}
			if record[i] == '.' {
				pos = i
				continue
			}
			price = price + (int(record[i])-'0')*k
			k *= 10

		}

		for j := sep2 - 4; j > 0; j-- {
			if record[j] == ',' {
				sep1 = j
				break
			}
		}

		cx := cidx[(uint64(record[0])*uint64(record[sep1-1]) + uint64(record[sep1-2])*uint64(sep1) + uint64(record[1]))]
		fx := fidx[uint64(record[sep1+1])-uint64(record[sep1+2])+(uint64(record[sep1+3])*uint64(record[sep2-1]))-9363]
		if t.products[cx][fx] > 100 {
			t.products[cx][fx] = custMin(t.products[cx][fx], int64(price))
		}
		t.sum[cx] += int64(price)
	}
}

func bytesToString(b []byte) string {
	return unsafe.String(unsafe.SliceData(b), len(b))
}
func (t *task) mergeTask(tt *task) {

	for cidx := 0; cidx < maxConstraintsCount; cidx++ {
		for pidx := 0; pidx < maxConstraintsCount; pidx++ {
			t.products[cidx][pidx] = min(t.products[cidx][pidx], tt.products[cidx][pidx])
		}
		t.sum[cidx] += tt.sum[cidx]
	}
}

func generateResult(tasks []*task) {
	for i, t := range tasks {
		if i == 0 {
			continue
		}
		tasks[0].mergeTask(t)
	}
	wVal := tasks[0].sum[0]
	w := 0
	for i, val := range tasks[0].sum {
		if wVal > val {
			wVal = val
			w = i
		}
	}

	mp := make(map[string]int64, len(tasks[0].products[w]))
	for i := 0; i < len(fruits); i++ {
		mp[fruits[i]] = tasks[0].products[w][i]
	}
	products_list := ans(mp)
	out := fmt.Sprintf("%s %.2f\n%s", cities[w], float64(wVal)/100, products_list)

	file, err := os.Create("./output.txt")
	assert(err)
	defer file.Close()
	_, err = file.WriteString(out)
	assert(err)
}

func main() {

	// disable the gc, should not trigger but just in case.

	debug.SetGCPercent(-1)
	if profile {
		f, err := os.Create("./profiles/pprod1")
		if err != nil {
			log.Fatal("could not create CPU profile: ", err)
		}
		defer f.Close()
		if err := pprof.StartCPUProfile(f); err != nil {
			log.Fatal("could not start CPU profile: ", err)
		}
		defer pprof.StopCPUProfile()

		ff, err := os.Create("./profiles/trace")
		if err != nil {
			log.Fatal("could not create trace execution profile: ", err)
		}
		defer ff.Close()
		trace.Start(ff)
		defer trace.Stop()
	}

	f, data, err := mmap(inputFile)
	if err != nil {
		panic(err)
	}
	defer f.Close()

	// Compute start indices for each worker.
	stride := len(data) / concurrency
	starts := make([]int, concurrency)
	for i := range starts {
		if i == 0 {
			continue
		}
		start := i * stride
		for data[start-1] != '\n' {
			start++
		}
		starts[i] = start
	}

	// Create tasks.
	workers := make([]*task, concurrency)
	for i := range workers {
		start := starts[i]
		var end int
		if i == concurrency-1 {
			end = len(data)
		} else {
			end = starts[i+1]
		}
		workers[i] = newTask(data[start:end])
	}

	// start the worker

	var wg sync.WaitGroup
	for _, w := range workers {
		w := w
		wg.Add(1)
		go func() {
			defer wg.Done()
			w.run()
		}()
	}

	wg.Wait()

	generateResult(workers)

}

func hashf(str string) uint64 {
	return uint64(str[0]) - uint64(str[1]) + (uint64(str[2]) * uint64(str[len(str)-1])) - 9363

}

func hashc(str string) uint64 {
	return (uint64(str[0])*uint64(str[len(str)-1]) + uint64(str[len(str)-2])*uint64(len(str)) + uint64(str[1]))
}

func custMin(x, y int64) int64 {
	return y + ((x - y) & ((x - y) >> 63))
}

func ans(mp map[string]int64) string {
	type KV struct {
		key   string
		value int64
	}

	vec := make([]KV, len(mp))
	i := 0
	for k, v := range mp {
		vec[i].key = k
		vec[i].value = v
		i++
	}

	sort.Slice(vec, func(i, j int) bool {
		if vec[i].value != vec[j].value {
			return vec[i].value < vec[j].value
		}
		return vec[i].key < vec[j].key
	})

	return fmt.Sprintf("%s %.2f\n%s %.2f\n%s %.2f\n%s %.2f\n%s %.2f\n",
		vec[0].key, float64(vec[0].value)/100,
		vec[1].key, float64(vec[1].value)/100,
		vec[2].key, float64(vec[2].value)/100,
		vec[3].key, float64(vec[3].value)/100,
		vec[4].key, float64(vec[4].value)/100)
}

func mmap(filename string) (_ *os.File, data []byte, err error) {
	// Open the file.
	f, err := os.Open(filename)
	if err != nil {
		return nil, nil, err
	}
	defer func() {
		if err != nil {
			f.Close()
		}
	}()

	// Grab its size.
	fi, err := f.Stat()
	if err != nil {
		return nil, nil, err
	}

	// mmap the file.
	data, err = syscall.Mmap(int(f.Fd()), 0, int(fi.Size()), syscall.PROT_READ, syscall.MAP_SHARED)
	if err != nil {
		return nil, nil, err
	}
	return f, data, nil
}
