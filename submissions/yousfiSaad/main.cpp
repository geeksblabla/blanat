#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <fstream>
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

typedef std::pair<long, bool> t2;
class CMap {
  std::vector<Node> nodes;
  char reverse_index[102][100];
  size_t reverse_index_size{0};

public:
  size_t number_of_nodes() const { return nodes.size(); }
  size_t size() const { return reverse_index_size; }
  const char *name_from_idx(size_t i) const {
    if (i >= reverse_index_size)
      return nullptr;
    return reverse_index[i];
  }
  CMap() {
    nodes.reserve(200);
    nodes.resize(1);
  }
  t2 get(const char *c) {
    size_t next_i = 0;
    for (int i = 0;; ++i) {
      Node &node = nodes[next_i];
      if (node.guess && (0 == strcmp(&(c[i]), node.guess_content))) {
        return t2(node.value, false);
      } else if (c[i] == '\0') {
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
        next_i = nodes.size();
        nodes.emplace_back();
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

typedef std::unordered_map<
    std::string, std::pair<long, std::unordered_map<std::string, long>>>
    m2;

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

class FileReader {
  std::ifstream in{INPUT_FILE_NAME};
  char reader_buffer[100];
  bool more = true;

public:
  bool readLine() {
    in.getline(reader_buffer, 99);
    more = !in.eof();
    return more;
  }
  inline char *buffer() { return reader_buffer; }
  inline bool have_more_lines() { return more; }
};

class FileReader_mem {
  int fd;
  struct stat sb;
  char *addr;
  off_t offset{0};
  off_t next_offset{0};

public:
  inline char *buffer() { return addr + offset; }
  inline bool have_more_lines() { return next_offset < sb.st_size; }
  inline off_t line_size() { return next_offset - offset; }
  bool readLine() {
    offset = next_offset;
    while (next_offset < sb.st_size && *(addr + next_offset) != '\n') {
      next_offset++;
    }
    // *(addr + next_offset++) = '\0';
    next_offset++;
    return sb.st_size > offset;
  }
  FileReader_mem() {
    fd = open(INPUT_FILE_NAME, O_RDONLY);
    fstat(fd, &sb);
    addr = (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  }
  ~FileReader_mem() {
    munmap(addr, sb.st_size);
    close(fd);
  }
};

long atoi(char *price) {
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

  // TODO: store only indices
  const City &min_city = data.cities[min_city_index];
  cmp comparator{data, min_city};
  size_t i = 0;
  for (; i < 5 && i < data.products_index.size(); i++) {
    vv[i] = i;
  }
  // TODO: maybe optimize more
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

void split(char *buffer, char *ps[2]) {
  char cc = -1;
  char c = 0;
  while (buffer[++cc] != '\0') {
    if (buffer[cc] == ',') {
      buffer[cc] = '\0';
      ps[c++] = buffer + cc + 1;
    }
  }
}

#define CHUNK_SIZE 1000
#define MAX_THREADS 32
char buffers[MAX_THREADS][CHUNK_SIZE][100];

void merge2(Data &data, Data &data_j) {
  char cache_p[MAX_PRODUCTS + 1];
  memset(cache_p, -1, (MAX_PRODUCTS + 1) * sizeof(char));
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

int main() {
  Data data;
  std::mutex mu_sums;
  std::vector<std::thread> threads;
  FileReader fr;
  FileReader_mem fr_m;
  std::mutex mu_fr;

  unsigned int n = 2 * std::thread::hardware_concurrency();
  // assert(n < MAX_THREADS);
  for (int j = 0; j < n; j++) {
    threads.emplace_back(
        [&fr_m, &data, &mu_fr, &mu_sums](int j) {
          Data data_j;
          while (true) {
            int l;
            {
              // std::scoped_lock<std::mutex> slock(mu_fr);
              mu_fr.lock();
              if (!fr_m.have_more_lines()) {
                mu_fr.unlock();
                break;
              }
              // copy lines into buffer
              l = 0;
              while (l < CHUNK_SIZE && fr_m.readLine()) {
                // strcpy(buffers[j][l], fr_m.buffer());
                memcpy(buffers[j][l], fr_m.buffer(), fr_m.line_size());
                buffers[j][l][fr_m.line_size() - 1] = '\0';
                ++l;
              }
              mu_fr.unlock();
            }
            // process data
            for (l--; l >= 0; l--) {
              char *p[2];
              char *b = buffers[j][l];
              split(b, p);
              size_t city_i = data_j.cities_index.get(b).first;
              City &city = data_j.cities[city_i];

              long nprice = atoi(p[1]);
              data_j.cities[city_i].sum += nprice;

              size_t product_i = data_j.products_index.get(p[0]).first;

              if (city.products[product_i].min > nprice) {
                city.products[product_i].min = nprice;
              }
            }
          }
          mu_sums.lock();
          merge2(data, data_j);
          mu_sums.unlock();
        },
        j);
  }

  for (int j = 0; j < n; j++)
    threads[j].join();

  // std::cout << "Threads joined\n";
  printResult2(data);

  return 0;
}
