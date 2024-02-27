#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

#define HASH_SIZE 101
#define MAX_PRODUCTS 5
#define MAX_LEN 25
#define NUM_THREADS 16

typedef struct {
	char *product;
	double price;
}	productPair;

typedef struct Node {
	char *city;
	int product_count;
	double total;
	productPair *products[MAX_PRODUCTS];
	struct Node *next;
} Node;

typedef struct {
	Node *buckets[HASH_SIZE];
} hashMap;

typedef struct {
	int id;
	char *start;
	char *end;
	hashMap *hashmap;
} threadArgs;

void insertionSort(productPair *arr[], int n) {
    int i, j;
    for (i = 1; i < n; i++) {
        productPair *key = arr[i];
        double keyPrice = key->price;
        const char* keyProduct = key->product;
        int cmp;

        j = i - 1;
        while (j >= 0) {
            cmp = (arr[j]->price > keyPrice) ? 1 :
                  (arr[j]->price < keyPrice) ? -1 :
                  strcmp(arr[j]->product, keyProduct);

            if (cmp > 0) {
                arr[j + 1] = arr[j];
                j = j - 1;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
}

unsigned int hash(const char *key) {
	unsigned int hash = 0;
	while (*key) {
		hash = (hash * 31) + *key++;
	}
	return hash % HASH_SIZE;
}

Node *createNode(const char* city) {
	Node *node = (Node *)malloc(sizeof(Node));
	if (node != NULL) {
		node->city = strdup(city);
		for (int i = 0; i < MAX_PRODUCTS; i++) {
			node->products[i] = NULL;
		}
		node->product_count = 0;
		node->total = 0;
		node->next = NULL;
	}
	return node;
}

hashMap *initHashMap() {
	hashMap *map = (hashMap *)malloc(sizeof(hashMap));
	if (map != NULL) {
		for (int i = 0; i < HASH_SIZE; i++) {
			map->buckets[i] = NULL;
		}
	}
	return map;
}

int is_duplicate(productPair *products[], const char *product) {
	for (int i = 0; i < 5 && products[i]; i++) {
		if (strcmp(products[i]->product, product) == 0)
			return i;
	}
	return -1;
}

void insert(hashMap *map, const char* city, const char* product, double price) {
	unsigned int index = hash(city);
	Node *node = map->buckets[index];
	Node *prev = NULL;

	while (node != NULL && strcmp(node->city, city) != 0) {
		prev = node;
		node = node->next;
	}

	if (node == NULL) {
		node = createNode(city);
		if (prev == NULL) {
			map->buckets[index] = node;
		} else {
			prev->next = node;
		}
	}

	node->total += price;

	int dup_index = is_duplicate(node->products, product);
	if (dup_index != -1) {
		if (price < node->products[dup_index]->price)
			node->products[dup_index]->price = price;
		return;
	}
	if (node->product_count < 5) {
		productPair *new = (productPair *)malloc(sizeof(productPair));
		new->price = price;
		new->product = strdup(product);
		node->products[node->product_count] = new;
		node->product_count++;
		if (node->product_count == 5)
			insertionSort(node->products, 5);
		return ;
	}
	if (price > node->products[4]->price)
		return ;
	if (price == node->products[4]->price && strcmp(product, node->products[4]->product) > 0)
		return ;
	char *tmp = node->products[4]->product;
	node->products[4]->product = strdup(product);
	free(tmp);
	node->products[4]->price = price;
		insertionSort(node->products, 5);
}

void *process_mapped_file(char* map, size_t size, hashMap *hashmap) {
	char *start = map;
	char *end = start + size;

	while (start < end) {
		char city[MAX_LEN] = {'\0'};
		for (int i = 0; i < MAX_LEN; i++) {
			if (*start != ',') {
				city[i] = *start;
				start++;
				continue;
			}
			start++;
			break;
		}

		char product[MAX_LEN] = {'\0'};
		for (int i = 0; i < MAX_LEN; i++) {
			if (*start != ',') {
				product[i] = *start;
				start++;
				continue;
			}
			start++;
			break;
		}

		//we can convert the string to double using our own calculations.
		double price = 0;
		int decimal = 0;
		while (start < end && *start != '\n') {
			if (*start == '.') {
				decimal = 1;
				start++;
				continue;
			}
			if (decimal == 0) {
				price = price * 10 + (*start - '0');
			} else {
				decimal *= 10;
				price += (double)(*start - '0') / decimal;
			}
			start++;
		}
		while (start < end && *start != '\n')
			start++;
		if (start < end)
			start++;
		insert(hashmap, city, product, price);
	}
	return hashmap;
}

Node *get_lowest_city(hashMap *hashmap) {
	double minTotal = __DBL_MAX__;
	Node* minNode = NULL;
	for (int i = 0; i < HASH_SIZE; i++) {
		Node *current = hashmap->buckets[i];
		while (current) {
			if (current->total < minTotal) {
				minTotal = current->total;
				minNode = current;
			}
			current = current->next;
		}
	}
	return minNode;
}

void *thread_function(void *arg) {
	threadArgs *data = (threadArgs *)arg;
	if (data->id != 0) {
		while (*data->start != '\n' && data->start < data->end)
			data->start++;
		if (data->start < data->end)
			data->start++;
	}
	pthread_exit(process_mapped_file(data->start, data->end - data->start, data->hashmap));
}

void freeHashMap(hashMap *map) {
	for (int i = 0; i < HASH_SIZE; i++) {
		Node *node = map->buckets[i];
		while (node != NULL) {
			Node *tmp = node;
			node = node->next;
			for (int i = 0; i < 5; i++) {
				free(tmp->products[i]->product);
				free(tmp->products[i]);
			}
			free(tmp->city);
			free(tmp);
		}
	}
	free(map);
}

void printHashMap(hashMap *map) {
	for (int i = 0; i < HASH_SIZE; i++) {
		Node *node = map->buckets[i];
		while (node != NULL) {
			printf("%s %.2f\n", node->city, node->total);
			for (int i = 0; i < 5; i++) {
				printf("\t%s %.2f\n", node->products[i]->product, node->products[i]->price);
			}
			node = node->next;
		}
	}
}

int	main(int ac, char **av) {
	if (ac != 2)
		return 1;
	int fd = open(av[1], O_RDONLY);
	if (fd == -1) {
		return 1;
	}
	struct stat sb;
	if (fstat(fd, &sb) == -1) {
		return 1;
	}

	char *map = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (map == MAP_FAILED) {
		return 1;
	}

	pthread_t threads[NUM_THREADS];
	threadArgs args[NUM_THREADS];
	size_t chunk_size = sb.st_size / NUM_THREADS;
	char *ptr = map;
	
	for (int i = 0; i < NUM_THREADS; i++) {
		args[i].hashmap = initHashMap();
		args[i].start = ptr;
		args[i].end = (i == NUM_THREADS - 1) ? map + sb.st_size : ptr + chunk_size;
		args[i].id = i;
		pthread_create(&threads[i], NULL, thread_function, &args[i]);
		ptr += chunk_size;
	}

	hashMap *final_map = initHashMap();

	for (int i = 0; i < NUM_THREADS; i++) {
		void *ret_arg;
		pthread_join(threads[i], &ret_arg);
		hashMap *result = (hashMap *)ret_arg;
		// Aggregating the results into the final_map.
		for (int j = 0; j < HASH_SIZE; j++) {
			Node *node = result->buckets[j];
			while (node != NULL) {
				Node *current = final_map->buckets[j];
				while (current != NULL && strcmp(current->city, node->city) != 0) {
					current = current->next;
				}
				if (current == NULL) {
					current = createNode(node->city);
					current->next = final_map->buckets[j];
					final_map->buckets[j] = current;
				}
				current->total += node->total;
				for (int k = 0; k < 5; k++) {
					if (node->products[k] == NULL)
						break;
					int dup_index = is_duplicate(current->products, node->products[k]->product);
					if (dup_index != -1) {
						if (node->products[k]->price < current->products[dup_index]->price)
							current->products[dup_index]->price = node->products[k]->price;
						continue;
					}
					if (current->product_count < 5) {
						productPair *new = (productPair *)malloc(sizeof(productPair));
						new->price = node->products[k]->price;
						new->product = strdup(node->products[k]->product);
						current->products[current->product_count] = new;
						current->product_count++;
						if (current->product_count == 5)
							insertionSort(current->products, 5);
						continue;
					}
					if (node->products[k]->price > current->products[4]->price)
						continue;
					if (node->products[k]->price == current->products[4]->price && strcmp(node->products[k]->product, current->products[4]->product) > 0)
						continue;
					char *tmp = current->products[4]->product;
					current->products[4]->product = strdup(node->products[k]->product);
					free(tmp);
					current->products[4]->price = node->products[k]->price;
					insertionSort(current->products, 5);
				}
				node = node->next;
			}
		}
	}

	Node *lowest = get_lowest_city(final_map);

	FILE *file = fopen("output.txt", "w");
	if (file == NULL) {
		return 1;
	}
	fprintf(file, "%s %.2f\n", lowest->city, lowest->total);
	for (int i = 0; i < 5; i++) {
		fprintf(file, "%s %.2f", lowest->products[i]->product, lowest->products[i]->price);
		if (i != 4)
			fprintf(file, "\n");
	}
	fclose(file);
	freeHashMap(final_map);

	munmap(map, sb.st_size);
	close(fd);
	return 0;
}