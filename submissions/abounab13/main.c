
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h> 

typedef struct s_product{
	char *product_name;
	long double price;
	struct s_product *next;
} t_product;

typedef struct s_data{
	char *city;
	long double total;
	t_product *head_products;
	struct s_data *next;
}	t_data;

typedef struct s_alpha{
	char alpha;
	t_data *head_cities;
}	dictionary;

t_product *create_product(char *name, long double prix);
void delete_last_node(t_product **products);
t_data *create_city(char **splitted);
int check_ifexist(t_product *products, char *product);
void add_if_cheap(t_product **products, char **splited);
void check_exist_or_add(t_data **cities, char **splited);
void free_all(dictionary alphabet);
t_data *sort_min(t_data **citiess);



int strlen_lst(t_product *products)
{
	int i = 0;
	
	while (products)
	{
		products = products->next;
		i++;
	}
	return (i);
}

void delete_last_node(t_product **products)
{
	t_product *cpy = *products;

	if (strlen_lst(cpy) > 2)
	{
		while (cpy->next->next)
			cpy = cpy->next;
		free(cpy->next);
		cpy->next = NULL;
	}
}

t_data *create_city(char **splitted)
{
	t_data *newc = (t_data *) malloc (sizeof(t_data));
	if (newc)
	{
		newc->city = strdup(*splitted);
		newc->total = atof(*(splitted + 2));
		newc->next = NULL;
		newc->head_products = create_product(*(splitted + 1), newc->total);
		return (newc);
	}
	else
		return 0;
}

t_product *create_product(char *name, long double prix)
{
	t_product *newp = (t_product *) malloc (sizeof(t_product));
	if (newp)
	{
		newp->product_name = strdup(name);
		newp->price = prix;
		newp->next = NULL;
		return (newp);
	}
	else
		return 0;
}

int check_ifexist(t_product *producta, char *products)
{
	t_product *cpy = producta;

	while (cpy)
	{
		if (!strcmp(cpy->product_name, products))
			return (1);
		cpy = cpy->next;
	}
	return (0);
}

void add_if_cheap(t_product **products, char **splited)
{
	t_product *cpy = *products;
	long double fprice = atof(*(splited + 2));
	
	if (check_ifexist(cpy, *(splited + 1)))
	{
		while (cpy)
		{
			if (!strcmp(cpy->product_name, *(splited + 1)))
			{
				if (cpy->price > fprice)
					cpy->price = fprice;
				break;
			}
			cpy = cpy->next;
		}
	}
	else
	{
		t_product *newp = create_product(*(splited + 1), fprice);
		if (*products)
		{
			if (cpy->price > fprice)
			{
				newp->next = cpy;
				*products = newp;
			}
			else
			{
				while (cpy->next)
				{
					if (cpy->next->price > fprice)
					{
						newp->next = cpy->next;
						cpy->next = newp;
						break;
					}
					cpy = cpy->next;
				}
				if (strlen_lst(*products) < 5 && !cpy->next)
					cpy->next = newp;

			}
			if (strlen_lst(*products) > 5)
				delete_last_node(products);
		}
		else
			*products = newp;
	}
}

t_data *sort_min(t_data **citiess)
{
	t_data *cpy;
	t_data *saver;

	cpy = (*citiess)->next;
	if (cpy && cpy->total < (*citiess)->total)
	{
		saver = cpy->next;
		(*citiess)->next = saver;
		cpy->next = *citiess;
		*citiess = cpy;
	}
	return (*citiess);
}

void check_exist_or_add(t_data **citiess, char **splited)
{
	if (*citiess)
	{
		t_data *cpy = *citiess;
		t_data *saver = cpy;
		int found = 0;
		
		while (cpy)
		{	
			if (splited && !strcmp(*splited, cpy->city))
			{
				found = 1;
				cpy->total += atof(*(splited + 2));
				add_if_cheap(&cpy->head_products, splited);
			}
			if (!(cpy->next))
				saver = cpy;
			cpy = cpy->next;
		}
		if(!found)
		{
			t_data *mdina = create_city(splited);
			if (mdina)
				saver->next = mdina;
		}
	}
	else
	{
		t_data *mdina = create_city(splited);
		if (mdina)
			*citiess = mdina;
	}
}

void free_products(t_product **pds)
{
	t_product *cpy = *pds;
	t_product *saver;

	while (cpy)
	{
		saver = cpy->next;
		free(cpy);
		cpy = saver;
	}
}

void free_all(dictionary alphabet)
{
	t_data *cpy;
	
	if (alphabet.head_cities)
	{
		
			while (alphabet.head_cities)
			{
				cpy = alphabet.head_cities->next;
				free_products(&alphabet.head_cities->head_products);
				free(alphabet.head_cities);
				alphabet.head_cities = cpy;
			}
	}
}

static int	get_words(char const *s, char c, int *index)
{
	int		counter;
	int		found;

	counter = 0;
	while (*s)
	{
		found = 0;
		while (*s == c && *s)
			s++;
		while (*s != c && *s)
		{
			s++;
			found = 1;
		}
		counter += found;
	}
	*index = counter;
	return (counter + 1);
}

static int	ft_strsdup(char **array, const char *s, int len)
{
	int		i;
	char	*arr;

	i = -1;
	arr = (char *) malloc (sizeof(char) * (len + 1));
	if (!arr)
		return (0);
	while (++i < len && s[i])
		arr[i] = s[i];
	arr[i] = 0;
	*array = arr;
	return (1);
}

char	**free_arr(char ***array, int index)
{
	int	i;

	i = 0;
	while (i < index)
		free((*array)[i++]);
	free(*array);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	int		i;
	int		len;
	int		words;

	i = -1;
	words = 0;
	array = (char **) malloc (sizeof(char *) * get_words(s, c, &words));
	if (!array)
		return (NULL);
	while (++i < words)
	{
		len = 0;
		while (*s == c && *s)
			s++;
		while (*s != c && *s)
		{
			len++;
			s++;
		}
		if (!ft_strsdup(&array[i], s - len, len))
			return (free_arr(&array, i));
	}
	array[words] = 0;
	return (array);
}


t_data *get_min_city(dictionary word)
{
	t_data *cpy;
	t_data *saver = NULL;
	int min ;
	
	if (word.head_cities)
	{
		cpy = word.head_cities;
		min = cpy->total;
		while (cpy)
		{
			if (min > cpy->total)
			{
				min = cpy->total;
				saver = cpy;
			}
			cpy = cpy->next;
		}
		if (saver)
			return (saver);
		else
			return (word.head_cities);
	}
	return (0);
}

int main()
{
	FILE *f = fopen("input.txt", "r");
	char ligne[100];
	dictionary alphabet[26] = {{0,NULL}};
	char **splited;

	while (fgets(ligne, 100, f))
	{
		
		if ((splited = ft_split(ligne, ',')))
		{
			alphabet[toupper(**splited) - 'A'].alpha = **splited;
			check_exist_or_add(&alphabet[toupper(**splited) - 'A'].head_cities, splited);
			free(*splited);
			free(*(splited + 1));
			free(*(splited + 2));
			free(splited);
		}
	}
	int i = 0;
	while (!alphabet[i].head_cities)
		i++;
	t_data *min_res = get_min_city(alphabet[i]);
	t_product *get;

	while (i < 26)
	{
		if (alphabet[i].head_cities)
		{
			t_data *res = get_min_city(alphabet[i]);
			if (res && min_res->total > res->total)
				min_res = res;
		}
		i++;
	}
	fclose(f);
	f = fopen("output.txt", "wa");

	fprintf(f ,"%s %.2Lf\n", min_res->city, min_res->total);
	i = 0;
	get = min_res->head_products;
	while (get)
	{
		fprintf(f, "%s %.2Lf", get->product_name, get->price);
		if (i < 4)
			fprintf(f, "\n");
		i++;
		get = get->next;
	}

	i = 0;
	while (i < 26)
	{
		if (alphabet[i].head_cities)
			free_all(alphabet[i]);
		i++;
	}
	fclose(f);
}

