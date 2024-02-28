/*  *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */
/* BLA BLA BLA BLA BLA BLA BLA BLA BLA BLA BLA BLA BLA BLA BLA BLA BLA BLA BLA */
/* L                                     _\                                  L */
/* A   program.cpp                        /`b <(WAL3ADAAAAAAAAAAAAAAAAAB)    B */
/* B                                 /####J                                  A */
/* L                                  |\ ||    11111  333333 333333 7777777  L */
/* A   By: zmoumen <zmoumen@student.1337.ma>      11      33     33     77   B */
/* B                                              11  333333 333333    77    A */
/* L   Created: 2024/02/20 13:37:42               11      33     33   77     L */
/* A   Updated: 2024/02/28 20:24:02              1111 333333 333333  77.ma   B */
/* B                                                                         A */
/* ALB ALB ALB ALB ALB ALB ALB ALB ALB ALB ALB ALB ALB ALB ALB ALB ALB ALB ALB */
/*  *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *  */

#include <iostream>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <thread>
#include <sys/wait.h>

typedef unsigned long ulong;

ulong MAX_ULONG = 18446744073709551615UL;

#define IMIN(x, y) ((y) ^ (((x) ^ (y)) & -((x) < (y))))
#define NB_CITIES 101
#define NB_PRODUCTS 94

typedef struct
{
	const char*	string;
	size_t		index;
}				name_index_t;

typedef struct
{
	size_t		fsize;
	char*		mmapd;
	char*		orig;
	int			fd;
}				buffer_t;

typedef struct
{
	size_t		start;
	size_t		end;
}				chunk_t;

typedef struct 
{
	ssize_t		ct_id;
	ssize_t		pd_id;
	ulong		price;
}				parse_data;

typedef struct
{
	ulong		total;
	ulong		products[NB_PRODUCTS];
}				city_data_t;

typedef struct
{
	city_data_t	cities[NB_CITIES];
}				cities_list_t;



const char* ct[NB_CITIES] = {
	"Jorf_El_Melha",	"Kalaat_MGouna",	"Ksar_El_Kebir",	"Béni_Mellal",	"Bir_Anzerane",
	"Ait_Melloul",		"Beni_Mellal",		"Chefchaouen",		"Oujda_Angad",	"Oulad_Teima",	"Sidi_Bouzid",
	"Souk_Larbaa",		"Ksar_es_Seghir",	"had_soualem", 		"Sidi_Bennour", "Sidi_Slimane", "Moulay_Bousselham",
	"Fes",				"Assa",				"Safi",				"Sale",			"Taza",			"Ahfir",
	"Azrou",			"Midar",			"Nador",			"Oujda",		"Rabat",		"Smara",
	"Agadir",			"Aourir",			"Arfoud",			"Asilah",		"Azilal",		"Dakhla",
	"Drarga",			"Figuig",			"Ifrane",			"Jerada",		"Meknes",		"Midelt",
	"Saidia",			"Sefrou",			"Settat",			"Temara",		"Tichka",		"Tichla",
	"Tiflet",			"Tiznit",			"Zagora",			"Berkane",		"Bouarfa",		"Demnate",
	"Guelmim",			"Guercif",			"Kenitra",			"Larache",		"Layoune",		"Tan-Tan",
	"Tangier",			"Tarfaya",			"Tetouan",			"Tinghir",		"Zemamra",		"Akhfenir",
	"Bab_Taza",			"Boujdour",			"Goulmima",			"Imzouren",		"Inezgane",		"Khenifra",
	"Laayoune",			"Ouazzane",			"Skhirate",			"Tafraout",		"Taourirt",		"Berrechid",
	"Boulemane",		"Chichaoua",		"El_Jadida",		"Essaouira",	"Khemisset",	"Khouribga",
	"Laâyoune",			"Marrakech",		"Sidi_Ifni",		"Taroudant",	"Guelta_Zemmur","Fquih_Ben_Salah",
	"Al_Hoceima",		"Bab_Berred",		"Ben_guerir",		"Bir_Lehlou",	"Bni_Hadifa",	"Casablanca",
	"Errachidia",		"Guerguerat",		"Mohammedia",		"Ouarzazate",	"Sidi_Kacem",	"Youssoufia"
};

const char* pd[NB_PRODUCTS] = {
	"Fig",				"Yam",				"Date",				"Beet",			"Kale",			"Dill",
	"Sage",				"Lime",				"Pear",				"Peas",			"Basil",		"Okra",
	"Kiwi",				"Apple",			"Chard",			"Banana",		"Mint",			"Peach",
	"Mango",			"Garlic",			"Plum",				"Lemon",		"Cabbage",		"Guava",
	"Thyme",			"Jicama",			"Carrot",			"Ginger",		"Onion",		"Lettuce",
	"Endive",			"Papaya",			"Celery",			"Orange",		"Grapes",		"Radish",
	"Cherry",			"Kiwano",			"Avocado",			"Parsnip",		"Apricot",		"Parsley",
	"Rhubarb",			"Spinach",			"Broccoli",			"Kohlrabi",		"Potato",		"Turnip",
	"Salsify",			"Currant",			"Coconut",			"Cilantro",		"Tomato",		"Artichoke",
	"Cucumber",			"Eggplant",			"Cantaloupe",		"Pumpkin",		"Rutabaga",		"Honeydew",
	"Oregano",			"Jackfruit",		"Bok_Choy",			"Nectarine",	"Cactus_Pear",	"Plantain",
	"Clementine",		"Cauliflower",		"Pineapple",		"Bell_Pepper",	"Blueberry",	"Persimmon",
	"Asparagus",		"Acorn_Squash",		"Cranberry",		"Zucchini",		"Blackberry",	"Grapefruit",
	"Raspberry",		"Rosemary",			"Watermelon",		"Starfruit",	"Goji_Berry",	"Watercress",
	"Green_Beans",		"Dragon_Fruit",		"Pomegranate",		"Passion_Fruit","Collard_Greens","Strawberry",
	"Butternut_Squash",	"Sweet_Potato",		"Squash_Blossom",	"Brussels_Sprouts"
};


inline
static size_t	city_matrix(const char *str, size_t len)
{
	static unsigned short	city_coefs[] = {
		307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
		307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
		307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
		307, 307, 307, 307, 307, 307, 105,  50, 307,  25, 307, 307, 307,  65, 307,  35,   5,   0,   5, 307,
		307, 307, 307,  25, 307,  25, 307, 307, 307, 307, 307, 307, 307, 307, 307,  65,   0,   0,  40,  95,
		30,	   5,  25, 120,   0,   5,  55,  70,  60,  30,  10,  15,   5,  10,   0,  20,  30,   0,  60, 307,
		307,  10,  20,   0, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
		307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
		307, 307, 307, 307, 307, 307, 307, 307, 307,   0, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
		307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307,  15, 307, 307, 307, 307,
		307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
		307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307,
		307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307
	};
  size_t hval = len;
  switch (hval)
	{
	  default:  hval += city_coefs[(unsigned char)str[5]];
	  case 5:   hval += city_coefs[(unsigned char)str[4]+1];
	  case 4:
	  case 3:   hval += city_coefs[(unsigned char)str[2]];
	  case 2:   hval += city_coefs[(unsigned char)str[1]]; break;
	}
  return hval;
}

inline
name_index_t*	localize_city (const char *str, size_t len)
{
	static name_index_t	city_idx[] = {
		{""},{""},{""},{""},{""},{""},{"Drarga",35},{""},{"Ouazzane",72},{""},{"Ouarzazate",98},{""},
		{""},{"Laayoune",71},{""},{"Guerguerat",96},{"Jerada",38},{""},{"Guelta_Zemmur",87},{"Chichaoua",78},
		{"Bir_Lehlou",92},{""},{"Kenitra",55},{"Inezgane",69},{"Taza",21},{"Ben_guerir",91},{"Beni_Mellal",6},
		{"Guelmim",53},{"Fes",17},{"Safi",19},{""},{"Souk_Larbaa",11},{"Tinghir",62},{""},{""},{""},{""},
		{"Larache",56},{"Goulmima",67},{"Ksar_es_Seghir",12},{"Errachidia",95},{"Sefrou",42},{"Tan-Tan",58},
		{"Boujdour",66},{"Assa",18},{"Rabat",27},{"Chefchaouen",7},{"Guercif",54},{"Taourirt",75},{"Khouribga",82},
		{"Ahfir",22},{"Ifrane",37},{"Zemamra",63},{"Jorf_El_Melha",0},{"Essaouira",80},{"Nador",25},{"Ait_Melloul",5},
		{"Tarfaya",60},{"Ksar_El_Kebir",2},{"Laâyoune",83},{"Midar",24},{"Temara",44},{"Berkane",50},{""},{"Sale",20},
		{"Mohammedia",97},{"Saidia",41},{"Bouarfa",51},{""},{""},{"Youssoufia",100},{"Asilah",32},{"Sidi_Slimane",15},
		{"Bab_Taza",65},{""},{"Smara",28},{"Aourir",30},{"Tangier",59},{""},{"Boulemane",77},{"Sidi_Kacem",99},{""},
		{"Moulay_Bousselham",16},{""},{""},{"Azrou",23},{"had_soualem",13},	{"Layoune",57},	{"Tafraout",74},
		{"Khemisset",81},{"Bni_Hadifa",93},	{"Tiflet",47},{""},{"Khenifra",70},{""},{"Fquih_Ben_Salah",88},{""},{""},
		{"Skhirate",73},{"Taroudant",86},{"Oujda",26},{"Midelt",40},{"Tetouan",61},{""},{""},{"Bab_Berred",90},{"Dakhla",34},
		{""},{""},{"Sidi_Ifni",85},{""},{"Settat",43},{"Demnate",52},{"Akhfenir",64},{""},{""},{"Tiznit",48},{""},
		{"Imzouren",68},{"Marrakech",84},{""},{"Arfoud",31},{""},{""},{""},{""},{"Meknes",39},{""},{""},{""},{""},
		{"Azilal",33},{"Bir_Anzerane",4},{""},{"Berrechid",76},{""},{"Tichla",46},{""},{""},{""},{""},{"Oulad_Teima",9},
		{""},{"Kalaat_MGouna",1},{""},{""},{"Zagora",49},{"Béni_Mellal",3},{""},{""},{""},{"Sidi_Bouzid",10},{"Sidi_Bennour",14},
		{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{"Tichka",45},{""},{""},{""},{""},{"Oujda_Angad",8},{""},
		{""},{""},{""},{""},{""},{""},{""},{""},{"Agadir",29},{""},{""},{""},{"Casablanca",NB_PRODUCTS},{""},{""},{""},{""},{""},
		{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{"El_Jadida",79},{""},{""},{""},{""},{""},{""},{""},{""},
		{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{"Al_Hoceima",89},
		{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},
		{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},
		{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{"Figuig",36}
	};
	size_t key = city_matrix(str, len);
	return &city_idx[key];
}


inline
static size_t	products_matrix(const char *str, size_t len)
{
	static unsigned char products_coefs[] = {
	  	140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140,  40,  40,   0,  75,   0, 0,    15,  20, 140,  55,  50,  20,  30,  35,  95,
		  0, 140,  10,  25,  65, 140, 140,  20, 140,   5, 5,   140, 140, 140, 140, 140, 140,   0,   0,  65,
		 60,   0, 140,  65,  40,   5,  10,   0,  30,  50, 5,     0,  30, 140,   5,  20,   0,   0, 140,  55,
		140,  20, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140
	};
  return len + products_coefs[(unsigned char)str[2]]
			 + products_coefs[(unsigned char)str[0]]
			 + products_coefs[(unsigned char)str[len - 1]];
}

inline
name_index_t*	localize_product(const char *str, size_t len)
{
	static name_index_t product_idx[] = {
		{""},{""},{""},{""},{""},{""},{"Potato",46},{"Cabbage",22},{""},{"Pear",8},{"Clementine",66},{"Carrot",26},
		{"Currant",49},{"Plantain",65},{"Pineapple",68},{"Cantaloupe",56},{"Cauliflower",67},{"Rhubarb",42},
		{"Rutabaga",58},{"Persimmon",71},{"Guava",23},{""},{""},{""},{"Peas",9},{"Grapefruit",77},{"Cherry",36},
		{"Lettuce",29},{""},{"Cranberry",74},{""},{"Ginger",27},{"Parsley",41},{"Passion_Fruit",87},{"Starfruit",81},
		{"Watermelon",80},{"Papaya",31},{"Sweet_Potato",91},{"Cilantro",51},{"Mint",16},{"Mango",18},{"Grapes",34},
		{"Parsnip",39},{""},{"Beet",3},{"Peach",17},{"Green_Beans",84},{"Avocado",38},{""},{"Artichoke",53},
		{"Watercress",83},{"Banana",15},{"Apricot",40},{"Broccoli",44},{"Plum",20},{"Goji_Berry",82},{"Celery",32},
		{""},{"Rosemary",79},{"Raspberry",78},{"Strawberry",89},{"Pomegranate",86},{"Pumpkin",57},{""},{"Collard_Greens",88},
		{"Chard",14},{"Endive",30},{""},{"Bok_Choy",62},{"Blueberry",70},{"Blackberry",76},{""},{"Coconut",50},{"Eggplant",55},
		{"Lime",7},{"Apple",13},{"Brussels_Sprouts",93},{"Spinach",43},{"Cucumber",54},{"Date",2},{"Lemon",21},{"Cactus_Pear",64},
		{"Salsify",48},{"Zucchini",75},{"Kale",4},{""},{"Bell_Pepper",69},{"Dragon_Fruit",85},{"Honeydew",59},{"Squash_Blossom",92},
		{"Thyme",24},{"Garlic",19},{"Acorn_Squash",73},{""},{"Sage",6},{"Basil",10},{"Butternut_Squash",90},{""},{""},{"Asparagus",72},
		{""},{"Orange",33},{"Oregano",60},{"Kohlrabi",45},{"Okra",11},{""},{"Turnip",47},{""},{"Yam",1},{"Nectarine",63},{"Onion",28},
		{"Kiwano",37},{""},{""},{"Kiwi",12},{""},{"Radish",35},{""},{""},{""},{""},{"Tomato",52},{""},{""},{""},{""},{"Jicama",25},{""},
		{""},{"Jackfruit",61},{""},{""},{""},{"Fig",0},{""},{""},{""},{""},{""},{"Dill",5}
	};
	size_t key = products_matrix(str,len);
	return &product_idx[key];
}


inline
void	map_file(const char *fname, buffer_t &buffer)
{
	buffer.fd = open(fname, O_RDONLY);
	struct stat sb;
	fstat(buffer.fd, &sb);
	buffer.fsize = sb.st_size;
	buffer.mmapd = (char *)mmap(NULL, buffer.fsize, PROT_READ, MAP_PRIVATE, buffer.fd, 0);
	buffer.orig = buffer.mmapd;
}


inline
void unmap_file(buffer_t &buffer)
{
	munmap(buffer.orig, buffer.fsize);
	close(buffer.fd);
}


inline
ulong str_to_l(buffer_t &buff)
{
	ulong res = 0;
	for (;*buff.mmapd != '.'; buff.mmapd++) {
		res = res * 10 + *buff.mmapd - '0';
	}
	buff.mmapd++;
	char *st = buff.mmapd;
	for(;*buff.mmapd != '\n'; buff.mmapd++){
		res = res * 10 + *buff.mmapd - '0';
	}
	if (buff.mmapd - st < 2) {res *= 10;}
	buff.mmapd++;
	return res;
}


inline
std::string	l_to_str(ulong n)
{
	std::string res;
 
	res = std::to_string(n / 100);
	res += ".";
	if (n % 100 < 10) res += "0";
	res += std::to_string(n % 100);
	return res;
}


inline
void	parse_line(buffer_t &buffer, parse_data &data)
{
	char bfr[32];
	char c;
	size_t len;
	for (len = 0; (c = *buffer.mmapd++) != ','; len++) bfr[len] = c;
	data.ct_id = localize_city(bfr, len)->index;
	
	for (len = 0; (c = *buffer.mmapd++) != ','; len++) bfr[len] = c;
	data.pd_id = localize_product(bfr, len)->index;

	data.price = str_to_l(buffer);
}


inline
void	skip_line(buffer_t &buffer)
{
	for (; *buffer.mmapd != '\n'; buffer.mmapd++);
	buffer.mmapd++;
}


inline
void	initialize_units(cities_list_t &data)
{
	for (size_t i = 0; i < NB_CITIES; i++) {
		data.cities[i].total = 0;
		for (size_t j = 0; j < NB_PRODUCTS; j++) {
		   data.cities[i].products[j] = MAX_ULONG;
		}
	}
}


void	worker_main(const char* fname, chunk_t chunk, bool skip_first, cities_list_t &workers_data)
{
	buffer_t buffer;
	map_file(fname, buffer);
	buffer.mmapd += chunk.start;
	char *end = buffer.mmapd + (chunk.end - chunk.start);
	parse_data data;
	if (skip_first) skip_line(buffer);
	initialize_units(workers_data);
	while (buffer.mmapd < end) {
		parse_line(buffer, data);
		workers_data.cities[data.ct_id].total += data.price;
		workers_data.cities[data.ct_id].products[data.pd_id] = IMIN(workers_data.cities[data.ct_id].products[data.pd_id], data.price);
	}
	unmap_file(buffer);
}


inline
void	launch_workers(const char *fname,size_t num_workers, cities_list_t *workers_data)
{
	buffer_t buffer;
	map_file(fname, buffer);
	size_t chunk_size = buffer.fsize / num_workers;
	for (size_t i = 0; i < num_workers; i++) {
		chunk_t chunk = {i * chunk_size, (i + 1) * chunk_size};
		if (i == num_workers - 1) {
			chunk.end = buffer.fsize;
		}
		bool skip_first = i != 0;
		int id  = fork();
		if (id == 0) {
			worker_main(fname, chunk, skip_first, workers_data[i]);
			exit(0);
		}
	}
	unmap_file(buffer);
}


void	get_cheapest(const city_data_t &cdata, size_t cheapest_cidx, ulong &cheapest_price)
{
	size_t idx_arr[NB_PRODUCTS];
	for (size_t i = 0; i < NB_PRODUCTS; i++) {
		idx_arr[i] = i;
	}
	std::sort(idx_arr, idx_arr + NB_PRODUCTS, [&cdata](size_t a, size_t b) {
		if (cdata.products[a] != cdata.products[b]) return cdata.products[a] < cdata.products[b];
		return strcmp(pd[a], pd[b]) < 0;
	});
	std::ofstream out("output.txt");
	out << ct[cheapest_cidx] << " " << l_to_str(cdata.total) << std::endl;
	for (size_t i = 0; i < 5 && cdata.products[idx_arr[i]] != MAX_ULONG; i++) {
		out << pd[idx_arr[i]] << " " << l_to_str(cdata.products[idx_arr[i]]) << std::endl;
	}
	out.close();
}


inline
size_t	merge_data(cities_list_t &final_data, cities_list_t *workers_data, size_t thread_num)
{
	ssize_t cheapest_idx = -1;
	for (size_t ict = 0; ict < NB_CITIES; ict++)
	{
		for (size_t itr = 0; itr < thread_num; itr++)
		{
			city_data_t &fct = final_data.cities[ict];
			city_data_t &tct = workers_data[itr].cities[ict];
			fct.total += tct.total;
			for (size_t ipd = 0; ipd < NB_PRODUCTS; ipd++)
			{
				ulong &fprod = fct.products[ipd];
				ulong &tprod = tct.products[ipd];
				fprod = IMIN(fprod, tprod);
			}
		}
		if (cheapest_idx == -1 || (final_data.cities[ict].total != 0 && final_data.cities[ict].total < final_data.cities[cheapest_idx].total))
			cheapest_idx = ict;
	}
	return cheapest_idx;
}


int	main(){
	const char *fname = "input.txt";
	size_t num_workers = std::thread::hardware_concurrency() * 2;
	cities_list_t *workers_data = (cities_list_t *)mmap(NULL, sizeof(cities_list_t) * num_workers, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	launch_workers(fname, num_workers, workers_data);
	static cities_list_t final_data;
	initialize_units(final_data);
	for (size_t i = 0; i < num_workers; i++) wait(NULL);
	ssize_t cheapest_idx = merge_data(final_data, workers_data, num_workers);
	get_cheapest(final_data.cities[cheapest_idx], cheapest_idx, final_data.cities[cheapest_idx].total);
	return (0);
}

/*    _
     |  )
     |  |
     |  |_____
    /     (]__)
   /     (]___)
  /      (]___)
 /    ___(]_)
    /
*/