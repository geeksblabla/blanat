#define _LARGEFILE64_SOURCE
#include <sys/mman.h>
#include <float.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>
#include <sys/resource.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <semaphore.h> 
#include <sys/syscall.h>


///////////// KEYWORDS //////////////////////
int cgroc[] = {5, 6, 6, 10, 6, 10, 9, 5, 4, 5, 4, 6, 4, 9, 9, 10, 10, 8, 7, 11, 5, 4, 10, 7, 6, 5, 3, 13, 7, 9, 8, 6, 8, 7, 4, 7, 6, 11, 8, 8, 7, 11, 16, 6, 4, 9, 9, 11, 4, 6, 5, 6, 6, 12, 3, 16, 12, 7, 9, 10, 7, 4, 10, 9, 7, 5, 14, 7, 8, 4, 5, 5, 8, 4, 4, 7, 9, 9, 9, 6, 6, 6, 8, 10, 4, 9, 8, 11, 6, 14, 12, 7, 8, 7, 8, 6, };
char *kgroc[] = {
    "Apple", "Banana", "Orange", "Strawberry", "Grapes",
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
    "Basil", "Thyme", "Rosemary", "Sage", "Dill", "Oregano",
    "Jackfruit", "Starfruit", "Persimmon", "Ginger", "Turnip",
    "Jicama", "Kohlrabi", "Watercress", "Okra", "Artichoke",
    "Plantain", "Cactus_Pear", "Kiwano", "Squash_Blossom", "Dragon_Fruit",
    "Parsnip", "Rutabaga", "Salsify", "Bok_Choy", "Endive"
};


int ccities[] = {10, 5, 9, 3, 7, 6, 6, 5, 7, 7, 4, 9, 11, 10, 4, 9, 9, 7, 13, 8, 13, 4, 10, 6, 6, 10, 6, 12, 5, 13, 9, 11, 10, 8, 9, 13, 6, 9, 6, 7, 6, 5, 8, 9, 12, 11, 6, 12, 7, 7, 9, 8, 6, 6, 6, 6, 10, 14, 6, 11, 7, 10, 10, 8, 11, 6, 6, 11, 6, 10, 15, 7, 7, 7, 5, 7, 8, 9, 8, 12, 4, 5, 8, 7, 8, 6, 11, 6, 10, 5, 17, 9, 10, 6, 11, 8, 8, 7, 10, 6, 8, };
char *kcities[] = {
    "Casablanca", "Rabat", "Marrakech", "Fes", "Tangier",
    "Agadir", "Meknes", "Oujda", "Kenitra", "Tetouan",
    "Safi", "El_Jadida", "Beni_Mellal", "Errachidia",
    "Taza", "Essaouira", "Khouribga", "Guelmim",
    "Jorf_El_Melha", "Laayoune", "Ksar_El_Kebir", "Sale", "Bir_Lehlou",
    "Arfoud", "Temara", "Mohammedia", "Settat",
    "Béni_Mellal", "Nador", "Kalaat_MGouna",
    "Chichaoua", "Chefchaouen", "Al_Hoceima", "Taourirt",
    "Taroudant", "Guelta_Zemmur", "Dakhla", "Laâyoune",
    "Tiznit","Tinghir", "Ifrane", "Azrou", "Bab_Taza",
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
    "Imzouren", "Zemamra", "Sidi_Kacem", "Drarga", "Skhirate"
};


///////////// DEFINES ////////////////////////
# define PAGE_SIZE 4096 * 6
# define CACHELEN 400
# define NL_SAMPLE 45
pthread_mutex_t lock; 
sem_t *mutex; 
// # define DEBUG 
// # define BUILD
//////////////////////////////////////////////

/////////////   .bss  ////////////////////////

// offsets where the newline hits in every page
// long long offpages[100000000] ;
long long offpages[30];
long long coffset[30];
int total_offset_pages = 0;
int pages_idx = 0;
void *pages[30];
int fds[30];
char *filename;
int THREADS;

///////////   CONTSTS ///////////////////////

#define TCITIES 101
#define TGROC 100
#define TGROCPERCITY 100000000

///////////  ANAL BSS ///////////////////////

double *total;
char **cities_baskets;
float **groc_cash;
float *sorted_winner_basket;
long long *cbaskets_idx;

/////////////////////////////////////////////

// case of 1m input.txt this should allocate 5767 page, which is kinda fucked
// but allows the threads to read
void newlines_offset(){
	int fd = open(filename, O_RDONLY);
	struct stat filex;
	long long chunk_size;
	int i = 0;
	long long tmpoffset;
	char buffer[45];

	fstat(fd, &filex);
	chunk_size = filex.st_size / THREADS;
	while (++i < THREADS){
		tmpoffset = offpages[i - 1] + chunk_size;
		lseek64(fd, tmpoffset, SEEK_SET);
		read(fd, buffer, NL_SAMPLE);
		offpages[i] = tmpoffset + (strrchr(buffer, '\n') + 1 - buffer);
	}
	offpages[i] = filex.st_size;
	memcpy(coffset, offpages, sizeof(long long) * (THREADS + 1));
	i = -1 ;
	// while (++i < THREADS + 1)
	// 	printf("offset %d -> %lld\n", i, offpages[i]);

}

void init_pages(){
	int i = -1;
	while (++i < THREADS){
		pages[i] = mmap(0, PAGE_SIZE + 1, PROT_WRITE | PROT_READ,
				MAP_SHARED|MAP_ANONYMOUS, -1, 0 );
	}
	// initing fds
	i = -1;
	while (++i < THREADS){
		fds[i] = open(filename, O_RDONLY);
	}
	// initing the variables now
	groc_cash = mmap(0, TCITIES * sizeof(float *), PROT_WRITE | PROT_READ
			, MAP_SHARED|MAP_ANONYMOUS, -1, 0 );
	i = -1;
	while (++i < TCITIES) {
		groc_cash[i] =  mmap(0,TGROCPERCITY * sizeof(float), PROT_WRITE | PROT_READ
			, MAP_SHARED|MAP_ANONYMOUS, -1, 0 );
	}
	total = mmap(0, TCITIES * sizeof(double), PROT_WRITE | PROT_READ,
			MAP_SHARED|MAP_ANONYMOUS, -1, 0 );

	cities_baskets = mmap(0, TCITIES * sizeof(int *), PROT_WRITE | PROT_READ
			, MAP_SHARED|MAP_ANONYMOUS, -1, 0 );
	i = -1;
	while (++i < TCITIES) {
		cities_baskets[i] =  mmap(0,TGROCPERCITY * sizeof(int), PROT_WRITE | PROT_READ
			, MAP_SHARED|MAP_ANONYMOUS, -1, 0 );
	}
	sorted_winner_basket = mmap(0, TGROCPERCITY * sizeof(float), PROT_WRITE | PROT_READ,
			MAP_SHARED|MAP_ANONYMOUS, -1, 0 );
	cbaskets_idx = mmap(0, TGROCPERCITY * sizeof(int), PROT_WRITE | PROT_READ,
			MAP_SHARED|MAP_ANONYMOUS, -1, 0 );
}

/////////////////////////////////////  LOOKUP ///////////////////////////////////////

int search_city(char *city){
	int i = -1;
	int len = 0;
	while (++i < TCITIES){
		if (strncmp(city, kcities[i], ccities[i] - 1) == 0 && *(city + ccities[i]) == ',')
			return (i);
	}
	return -1;
}

int search_groc(char *groc){
	int i = -1;
	while (++i < TGROC){
		if (strncmp(groc, kgroc[i], cgroc[i]) == 0)
			return (i);
	}
	return -1;
}

/////////////////////////////////////  LOOKUP ///////////////////////////////////////

///////////////////////////////////// ANALYTICS //////////////////////////////////////

// extract the index of the product, cash and city name
void basket_compute(char *page, int id){
	while (*page != '\0'){
		char *line = page;
		int cityidx = search_city(page);
		page += ccities[cityidx] + 1;

		int groc = search_groc(page);

		page += cgroc[groc] + 1;
		float cash = atof(page);

		sem_wait(mutex);
		total[cityidx] += cash;
		groc_cash[cityidx][cbaskets_idx[cityidx]] = cash;
		cities_baskets[cityidx][cbaskets_idx[cityidx]] = groc;
		cbaskets_idx[cityidx]++;
		sem_post(mutex);
		page = strchr(page, '\n') + 1;
	}
}
/////////////////////////////////////////////////////////////////////////////////////

void read_chunk(int id){
	int rbytes = 0;
	char *nlptr;

	if (id != 0)
		lseek64(fds[id], coffset[id], SEEK_SET);
	while (coffset[id] < offpages[id + 1]){
		rbytes = read(fds[id], pages[id], PAGE_SIZE - 1);
		if (coffset[id] + rbytes < offpages[id + 1]){
			nlptr = strrchr(pages[id] + PAGE_SIZE - 40, '\n') + 1;
		}
		else {
			nlptr = &((char **)pages)[id][offpages[id + 1] - coffset[id]];
		}
		*nlptr = '\0';

		coffset[id] += (nlptr - (char *) pages[id]);
		lseek64(fds[id], coffset[id], SEEK_SET);
		// sem_wait(mutex);
		// printf("%s", (char *)pages[id]);
		// sem_post(mutex);
		basket_compute(pages[id], id);
	}
	exit(0);
	// printf("thread [%d] coffset %lld -> %lld\n", id, coffset[id], offpages[id + 1]);
}

/////////////////////////////////////////////////////////////////////////////////
void write_sorted(int lower, FILE *output){
	int i = -1;
	int cursed[5] = {-1,-1,-1,-1,-1};
	int cursedptr = 0;
	while (cursedptr < 5){
		int x = 0;
		while (++x < cbaskets_idx[lower]){
			if (fabs(sorted_winner_basket[i] - groc_cash[lower][x]) < FLT_EPSILON){
				int p = -1;
				while (++p < 4){
					if (cities_baskets[lower][x] == cursed[p]){
						p = 69;
					}
				}
				if (p < 69){
					fprintf(output, "%s %.2lf\n", kgroc[cities_baskets[lower][x]],
							groc_cash[lower][x]);
					cursed[cursedptr++] = cities_baskets[lower][x];
					break;
				}
			}
		}
		i++;
	}
}
////////////////////////// ALGO /////////////////////////////////////////////////

int lowerbound(){
	int i = 0;
	int lower = 0;
	while (i < TCITIES - 1){
		if (total[lower] > total[i + 1] && (int)total[i + 1] > 0)
			lower = i + 1;
		i++;
	}
	return (lower);
}

int qcompare (const void * a, const void * b)
{
  float fa = *(const float*) a;
  float fb = *(const float*) b;
  return (fa > fb) - (fa < fb);
}

void get_winner(){
	int winneridx = lowerbound();
	FILE *output = fopen("output.txt", "a+");
	fprintf(output, "%s %.2f\n", kcities[winneridx], total[winneridx]);
	memcpy(sorted_winner_basket, groc_cash[winneridx], cbaskets_idx[winneridx] * sizeof(float));
	qsort (sorted_winner_basket, cbaskets_idx[winneridx], sizeof(float), qcompare);
	write_sorted(winneridx, output);
}
/////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv){

	filename = argv[1];
	if (argc < 2)
		filename = "input.txt";
	#ifdef BUILD
		errx(fd == -1 , "%s[%d] not found", filename, fd);
	#endif
	// caching the newline pages offsets
	THREADS = sysconf(_SC_NPROCESSORS_ONLN) * 2;
	newlines_offset();
	setenv("GLIBC_PTHREAD_MUTEX_SPIN_COUNT", "1000", 1);
	// initing pages
	init_pages();
	// initing the locks
	mutex = sem_open("/head_spinz", O_CREAT, 0644, 1);

	// spawning threads
	for (int i = 0; i < THREADS; i++) {
		if (fork() == 0) {
			read_chunk(i);
		}
    	}
	for (int i = 0; i < THREADS; i++) {
		wait(NULL);
	}
	get_winner();
	return (0);
}
