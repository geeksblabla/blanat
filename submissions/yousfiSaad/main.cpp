
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <iterator>
#include <mutex>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <utility>
#include <vector>

const char *INPUT_FILE_NAME = "input.txt";
const char *OUTPUT_FILE_NAME = "output.txt";
const size_t MAX_SIZE = 101;

namespace custom_map {

struct Node {
  bool guess = false;
  char guess_content[100];
  size_t nexts[300];
  bool end{false};
  size_t value{0};
  Node(long value = 0) : value(value) {
    memset(nexts, 0, 300 * sizeof(size_t));
  }
};

constexpr size_t MAX_NODES = 200;
// constexpr char eos = '\0';
typedef std::pair<long, bool> t2;
class CMap {
  std::array<Node, MAX_NODES> nodes_arr;
  size_t nodes_arr_size = 1;
  char reverse_index[102][100];
  size_t reverse_index_size{0};

public:
  size_t number_of_nodes() const { return nodes_arr_size; }
  size_t size() const { return reverse_index_size; }
  const char *name_from_idx(size_t i) const {
    if (i >= reverse_index_size)
      return nullptr;
    return reverse_index[i];
  }

  t2 get(const char *c) {
    constexpr char EOS = '\0';
    size_t next_i = 0;
    for (int i = 0;; ++i) {
      Node &node = nodes_arr[next_i];
      if (node.guess && (0 == strcmp(&(c[i]), node.guess_content))) {
        return t2(node.value, false);
      } else if (c[i] == EOS) {
        if (node.end)
          return t2(node.value, false);
        else {
          strcpy(reverse_index[reverse_index_size], c);
          node.value = reverse_index_size++;
          node.end = true;
          return t2(node.value, true);
        }
      } else if (!node.guess) {
        strcpy(reverse_index[reverse_index_size], c);
        node.value = reverse_index_size++;
        node.guess = true;
        strcpy(node.guess_content, &(c[i]));
        return t2(node.value, true);
      } else if (node.nexts[c[i]] == 0) {
        next_i = nodes_arr_size++;
        node.nexts[c[i]] = next_i;
      } else {
        next_i = node.nexts[c[i]];
      }
    }
  }

  t2 get(const char *c, size_t size) {
    constexpr char EOS = ',';
    size_t next_i = 0;
    for (int i = 0;; ++i) {
      Node &node = nodes_arr[next_i];
      if (node.guess && (0 == memcmp(&(c[i]), node.guess_content, size - i)) &&
          node.guess_content[size] == '\0') {
        return t2(node.value, false);
      } else if (c[i] == EOS) {
        if (node.end)
          return t2(node.value, false);
        else {
          memcpy(reverse_index[reverse_index_size], c, size);
          reverse_index[reverse_index_size][size] = '\0';
          node.value = reverse_index_size++;
          node.end = true;
          return t2(node.value, true);
        }
      } else if (!node.guess) {
        memcpy(reverse_index[reverse_index_size], c, size);
        reverse_index[reverse_index_size][size] = '\0';
        node.value = reverse_index_size++;
        node.guess = true;
        size_t size_minus_i = size - i;
        memcpy(node.guess_content, &(c[i]), size_minus_i);
        node.guess_content[size_minus_i] = '\0';
        // strcpy(node.guess_content, &(c[i]));
        return t2(node.value, true);
      } else if (node.nexts[c[i]] == 0) {
        next_i = nodes_arr_size++;
        node.nexts[c[i]] = next_i;
      } else {
        next_i = node.nexts[c[i]];
      }
    }
  }
};

} // namespace custom_map

#define MAX_CITIES 101
#define MAX_PRODUCTS 101

struct Product {
  long min{100000};
};

struct City {
  long sum{0};
  Product products[MAX_PRODUCTS + 1];
};

struct Data {
  City cities[MAX_CITIES + 1];
  custom_map::CMap products_index;
  custom_map::CMap cities_index;
};

class FileReader_mem {
  int fd;
  struct stat sb;
  char *addr = NULL;
  char *last_addr;
  off_t offset{0};
  off_t next_offset{0};

public:
  inline char *last_byte() const { return last_addr; }
  inline char *get_addr() const { return addr; }
  inline char *buffer() const { return addr + offset; }
  inline bool have_more_lines() { return next_offset < sb.st_size; }
  inline size_t file_size() const { return sb.st_size; }
  inline off_t line_size() { return next_offset - offset; }
  bool readLine() {
    offset = next_offset;
    while (next_offset < sb.st_size && *(addr + next_offset) != '\n') {
      next_offset++;
    }
    next_offset++;
    return sb.st_size > offset;
  }
  FileReader_mem(bool map = false, off_t offset = 0,
                 const off_t *pSize = nullptr) {
    fd = open(INPUT_FILE_NAME, O_RDONLY);
    fstat(fd, &sb);
    off_t size = pSize == NULL ? sb.st_size : *pSize;
    if (map) {
      // if (offset + size + 100 < sb.st_size) {
      //   size += 100;
      // }
      addr = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
      last_addr = addr + sb.st_size - 1;
    }
  }
  ~FileReader_mem() {
    if (addr != NULL)
      munmap(addr, sb.st_size);
    close(fd);
  }
};

size_t atoin(const char *price, long &r) {
  r = 0;
  long id;
  bool hasd = false;
  int i = 0;
  for (; price[i] != '\n'; i++) {
    if (price[i] == '.' || price[i] == ',') {
      hasd = true;
      id = i;
      continue;
    }
    r *= 10;
    r += (price[i] - '0');
  }
  if (hasd) {
    if ((id + 2) == i)
      r *= 10;
  } else {
    r *= 100;
  }
  return i;
}

long atoi2(const char *price) {
  long r = 0;
  long id;
  bool hasd = false;
  int i = 0;
  for (; price[i] != '\0'; i++) {
    if (price[i] == '.' || price[i] == ',') {
      hasd = true;
      id = i;
      continue;
    }
    r *= 10;
    r += (price[i] - '0');
  }
  if (hasd) {
    if ((id + 2) == i)
      r *= 10;
  } else {
    r *= 100;
  }
  return r;
}

void min2(Data &data, size_t &min_city_index) {
  bool first = true;
  for (size_t i = 0; i < data.cities_index.size(); i++) {
    if (data.cities[i].sum < data.cities[min_city_index].sum || first) {
      first = false;
      min_city_index = i;
    }
  }
}

struct cmp {
  const City &c;
  const Data &d;
  cmp(const Data &d, const City &c) : c{c}, d{d} {}
  bool operator()(const size_t &a_i, const size_t &b_i) {
    return c.products[a_i].min < c.products[b_i].min ||
           (c.products[a_i].min == c.products[b_i].min &&
            0 > strcmp(d.products_index.name_from_idx(a_i),
                       d.products_index.name_from_idx(b_i)));
  }
  // bool operator()(const std::pair<std::string, long> &a,
  //                 const std::pair<std::string, long> &b) const {
  //   return (a.second > b.second) ||
  //          (a.second == b.second && a.first.compare(b.first) > 0);
  // }
};

void printResult2(Data &data) {
  size_t min_city_index = 0;

  min2(data, min_city_index);

  FILE *fp = fopen(OUTPUT_FILE_NAME, "w+");

  fprintf(fp, "%s %ld.%02ld\n", data.cities_index.name_from_idx(min_city_index),
          data.cities[min_city_index].sum / 100,
          data.cities[min_city_index].sum % 100);

  std::array<size_t, 6> vv;

  const City &min_city = data.cities[min_city_index];
  cmp comparator{data, min_city};
  size_t i = 0;
  for (; i < 5 && i < data.products_index.size(); i++) {
    vv[i] = i;
  }
  // TODO: maybe optimize more
  // TODO: lower_bound
  for (; i < data.products_index.size(); i++) {
    vv[5] = i;
    std::sort(std::begin(vv), std::begin(vv) + 6, comparator);
  }

  for (int i = 0; i < 5 && i != vv.size(); ++i) {
    const size_t &j = vv[i];
    fprintf(fp, "%s %ld.%02ld\n", data.products_index.name_from_idx(j),
            min_city.products[j].min / 100, min_city.products[j].min % 100);
  }
  fflush(fp);
  fclose(fp);
}

#define MAX_THREADS 128

void merge2(Data &data, Data &data_j) {
  size_t cache_p[MAX_PRODUCTS + 1];
  memset(cache_p, -1, (MAX_PRODUCTS + 1) * sizeof(size_t));
  for (size_t i = 0; data_j.cities_index.size() > i; ++i) {
    size_t j =
        data.cities_index.get(data_j.cities_index.name_from_idx(i)).first;
    data.cities[j].sum += data_j.cities[i].sum;

    City &city_j = data_j.cities[i];
    City &city = data.cities[j];
    for (size_t ii = 0; data_j.products_index.size() > ii; ++ii) {
      size_t jj = cache_p[ii];
      if (jj == -1)
        jj = data.products_index.get(data_j.products_index.name_from_idx(ii))
                 .first;
      Product &product_j = city_j.products[ii];
      Product &product = city.products[jj];
      if (product_j.min < product.min)
        product.min = product_j.min;
      std::cerr << "";
    }
  }
}

const char *mmemchr(const char *addr, char c, size_t size_limit) {
  size_t i = 0;
  while (i < size_limit && addr[i] != c) {
    i++;
  }
  if (i == size_limit) {
    return NULL;
  }
  return addr + i;
}

constexpr off_t CHUNK_SIZE_MEM = 100000;
constexpr off_t MAX_LINE_LEN = 100;
long total = 0;
std::mutex total_mutex;

void process_chunk2(Data &data_j, const char *addr, off_t size, bool first) {
  long nprice;
  const char *end = addr + size;
  const char *addrc = addr;
  if (!first) {
    addrc = 1 + mmemchr(addr, '\n', MAX_LINE_LEN);
  }
  while (addrc < end) {
    const char *c1 = 1 + mmemchr(addrc, ',', MAX_LINE_LEN);
    const char *c2 = 1 + mmemchr(c1, ',', MAX_LINE_LEN);

    size_t city_i = data_j.cities_index.get(addrc, c1 - addrc - 1).first;
    City &city = data_j.cities[city_i];
    addrc = 1 + c2 + atoin(c2, nprice);
    data_j.cities[city_i].sum += nprice;
    size_t product_i = data_j.products_index.get(c1, c2 - c1 - 1).first;
    if (city.products[product_i].min > nprice) {
      city.products[product_i].min = nprice;
    }
    // total_mutex.lock();
    // std::cout << data_j.cities_index.name_from_idx(city_i) << ","
    //           << data_j.products_index.name_from_idx(product_i) << ","
    //           << (float(nprice) / 100) << "\n";
    // total++;
    // total_mutex.unlock();
  }
}

int main() {
  Data data;
  std::mutex mu_sums;
  std::vector<std::thread> threads;
  FileReader_mem fr_m(true);
  std::mutex mu_fr;

  const unsigned int number_of_threads = 32;
      //2 * std::thread::hardware_concurrency();
  const off_t chunk_size = fr_m.file_size() / number_of_threads;
  for (unsigned int j = 0; j < number_of_threads; j++) {
    threads.emplace_back(
        [number_of_threads, j, &chunk_size, &fr_m, &data, &mu_sums] {
          Data data_j;
          off_t from = chunk_size * j;
          off_t last_chunk_size = fr_m.file_size() - from;

          // FileReader_mem fr_m_j(true);
          // process_chunk2(data_j, fr_m_j.get_addr() + from, chunk_size, j ==
          // 0);
          process_chunk2(data_j, fr_m.get_addr() + from, chunk_size, j == 0);

          mu_sums.lock();
          merge2(data, data_j);
          mu_sums.unlock();
        });
  }

  for (int j = 0; j < number_of_threads; j++)
    threads[j].join();

  // std::cout << "Total: " << total << "\n";
  printResult2(data);

  return 0;
}
