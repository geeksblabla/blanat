#include <sys/mman.h>
#include <float.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>


///////////// KEYWORDS //////////////////////
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
    "Basil", "Thyme", "Rosemary", "Sage", "Dill",
    "Oregano", "Cantaloupe", "Honeydew", "Coconut", "Pomegranate",
    "Jackfruit", "Starfruit", "Persimmon", "Ginger", "Turnip",
    "Jicama", "Kohlrabi", "Watercress", "Okra", "Artichoke",
    "Plantain", "Cactus_Pear", "Kiwano", "Squash_Blossom", "Dragon_Fruit",
    "Parsnip", "Rutabaga", "Salsify", "Bok_Choy", "Endive"
};

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
# define PAGE_SIZE 4096
# define THREADS 6
# define NL_SAMPLE 45
pthread_mutex_t lock; 
// # define DEBUG 
// # define BUILD
//////////////////////////////////////////////

/////////////   .bss  ////////////////////////

// offsets where the newline hits in every page
int offpages[100000000] ;
int total_offset_pages = 0;
int pages_idx = 0;
void *pages[THREADS];
int  fds[THREADS];
char *filename;

///////////   CONTSTS ///////////////////////

#define TCITIES 101
#define TGROC 100
#define TGROCPERCITY 1000000

///////////  ANAL BSS ///////////////////////

double total[TCITIES];
char cities_baskets[TCITIES][TGROCPERCITY];
float groc_cash[TCITIES][TGROCPERCITY];
float sorted_winner_basket[TGROCPERCITY];
int cbaskets_idx[TCITIES];

/////////////////////////////////////////////

// case of 1m input.txt this should allocate 5767 page, which is kinda fucked
// but allows the threads to read
void newlines_offset(){
	int fd = open(filename, O_RDONLY);
	struct stat filex;
	int readstatus = 1;
	int coffset = PAGE_SIZE;
	char buffer[NL_SAMPLE + 1];
	int pagesbuffer = 1;

	while (readstatus != 0 && lseek(fd, coffset - NL_SAMPLE, SEEK_SET) > 0){
		readstatus = read(fd, buffer, NL_SAMPLE);
		char* newline = strrchr(buffer, '\n');
		offpages[pagesbuffer] =  coffset - (NL_SAMPLE - labs(newline - buffer));
		coffset = offpages[pagesbuffer++] + PAGE_SIZE;
		total_offset_pages++;
	}
	#ifdef DEBUG
	while (offpages[++readstatus] != 0){
		printf("%d, ", offpages[readstatus]);
	}
	#endif
	close(fd);
}

void init_pages(){
	int i = -1;
	while (++i < THREADS){
		pages[i] = mmap(0, PAGE_SIZE + 1, PROT_WRITE | PROT_READ, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0 );
	}
	// initing fds
	i = -1;
	while (++i < THREADS){
		fds[i] = open(filename, O_RDONLY);
	}
}

/////////////////////////////////////  LOOKUP ///////////////////////////////////////

char *skip_seps(char *pepe){
	int i = 0;
	while (strchr("\n\t ,", pepe[i]) != NULL )
		i++;
	return &pepe[i];
}

int search_city(char *city){
	int i = -1;
	city = skip_seps(city);
	while (++i < TCITIES){
		if (!strcmp(city, kcities[i]))
			return (i);
	}
	return -1;
}

int search_groc(char *groc){
	int i = -1;
	groc = skip_seps(groc);
	while (++i < TGROC){
		if (!strcmp(groc, kgroc[i]))
			return (i);
	}
	return -1;
}

/////////////////////////////////////  LOOKUP ///////////////////////////////////////

///////////////////////////////////// ANALYTICS //////////////////////////////////////

void basket_compute(char *page){
	char *spl = NULL;
	while (strchr(page, '\n') != NULL){
		char *tmpx = strchr(page, ',');
		*tmpx = '\0';
		// content now equals the name
		int cityidx = search_city(page);
		errx(cityidx , "city notfound %s" , page);
		page = tmpx + 1;
		tmpx = strchr(page, ',');
		*tmpx = '\0';

		pthread_mutex_lock(&lock); 
		cities_baskets[cityidx][cbaskets_idx[cityidx]] = search_groc(page);
		pthread_mutex_unlock(&lock); 
		errx(cities_baskets[cityidx][cbaskets_idx[cityidx]] == -1, "%s" , page);
		page = tmpx + 1;
		float cash = atof(page);
		pthread_mutex_lock(&lock); 
		total[cityidx] += cash;
		groc_cash[cityidx][cbaskets_idx[cityidx]] = cash;
		cbaskets_idx[cityidx] += 1;
		pthread_mutex_unlock(&lock); 
		page = strchr(page, '\n') + 1;
	}
}
/////////////////////////////////////////////////////////////////////////////////////


void *read_chunk(void *id){
	int iid = (int) id;
	int i = iid;
	// int readsperthread = ceil(total_offset_pages / THREADS);
	// int rpt = -1;		// read per thread
#ifdef DEBUG
	puts("init threads");
#endif
	while (offpages[i + 1] != 0) {
		lseek(fds[iid], offpages[i], SEEK_SET);
#ifdef DEBUG
		printf("thread [%d] reading from fd[%d] page[%d] offset %d total of %d bytes\n",
			iid , fds[iid], i, offpages[i], offpages[i + 1] - offpages[i]);
#endif
		/////////////////////////////// DELETE ME ////////////////////////////
		// pthread_mutex_lock(&lock); 
		// printf("%s", (char *) pages[iid]);
		// pthread_mutex_unlock(&lock); 
		/////////////////////////////// DELETE ME ////////////////////////////
		int zbyte = read(fds[iid], pages[iid], offpages[i + 1] - offpages[i]);
		if (zbyte > 0)
			((char *)pages[iid])[zbyte] = '\0';
		i += THREADS ;
	}
	basket_compute(pages[iid]);
	return (NULL);
}
/////////////////////////////////////////////////////////////////////////////////
void write_sorted(int lower, FILE *output){
	int i = -1;
	while (++i < cbaskets_idx[lower]){
		int x = 0;
		while (++x < cbaskets_idx[lower]){
			if (fabs(sorted_winner_basket[i] - groc_cash[lower][x]) < FLT_EPSILON){
				fprintf(output, "%s %lf\n", kgroc[cities_baskets[lower][x]],
						groc_cash[lower][x]);
				break;
			}
		}
	}
}
////////////////////////// ALGO /////////////////////////////////////////////////

int lowerbound(){
	int i = -1;
	float lower = total[0];
	while (++i < TCITIES - 1){
		if (total[i] > total[i + 1]){
			lower = i + 1;
		}
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
	fprintf(output, "%s %lf\n", kcities[winneridx], total[winneridx]);
	memcpy(sorted_winner_basket, groc_cash, cbaskets_idx[winneridx] * sizeof(float));
	qsort (sorted_winner_basket, cbaskets_idx[winneridx], sizeof(float), qcompare);
	write_sorted(winneridx, output);

}
/////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv){
	long long int i = -1;
	pthread_t threadsid[THREADS];

	filename = argv[1];
	if (argc < 2)
		filename = "input.txt";
	#ifdef BUILD
		errx(fd == -1 , "%s[%d] not found", filename, fd);
	#endif
	// caching the newline pages offsets
	newlines_offset();
	// initing pages
	init_pages();
	// initing the lock
	pthread_mutex_init(&lock, NULL);
	// offsets are loaded, now we load, threads
	while (++i < THREADS){
		pthread_create(&threadsid[i], NULL, read_chunk, (void *) i);
		pthread_join(threadsid[i], NULL);
	}
	// i = -1;
	// while (++i < THREADS){
	// 	puts(pages[i] + strlen(pages[i]) - 200);
	// 	puts("---------------------------");
	// }
	get_winner();
	return (0);
}
