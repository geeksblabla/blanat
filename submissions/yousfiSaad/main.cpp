#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

const char *INPUT_FILE_NAME = "input.txt";
const char *OUTPUT_FILE_NAME = "output.txt";
const size_t MAX_SIZE = 101;

typedef std::unordered_map<
    std::string, std::pair<long, std::unordered_map<std::string, long>>>
    m2;

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

std::pair<std::string, long> min(m2 &sums) {
  std::pair<std::string, long> r;
  r.second = 0;
  bool first = true;
  for (m2::iterator it = sums.begin(); it != sums.end(); it++) {
    if (it->second.first < r.second || first) {
      first = false;
      r.first = it->first;
      r.second = it->second.first;
    }
  }
  return r;
}

struct cmp {
  bool operator()(const std::pair<std::string, long> &a,
                  const std::pair<std::string, long> &b) const {
    return (a.second > b.second) ||
           (a.second == b.second && a.first.compare(b.first) > 0);
  }
};

void printResult(m2 &sums) {
  std::pair<std::string, long> r = min(sums);

  FILE *fp = fopen(OUTPUT_FILE_NAME, "w+");

  fprintf(fp, "%s %ld.%02ld\n", r.first.c_str(), r.second / 100,
          r.second % 100);

  std::vector<std::pair<std::string, long>> v;

  for (const std::pair<const std::string, long> &n : sums[r.first].second)
    v.push_back(n);

  std::vector<std::pair<std::string, long>>::iterator it = v.begin();
  for (int i = 0; i < 5 && it != v.end(); ++it, ++i) {
    std::vector<std::pair<std::string, long>>::iterator min_it =
        std::max_element(it, v.end(), cmp());

    fprintf(fp, "%s %ld.%02ld\n", min_it->first.c_str(), min_it->second / 100,
            min_it->second % 100);
    std::iter_swap(it, min_it);
  }
  fflush(fp);
  fclose(fp);
}

void split(char *buffer, char *ps[2]) {
  char cc = -1;
  char c = 0;
  while (buffer[++cc] != '\0') {
    if (buffer[cc] == ',' || buffer[cc] == '\n' || buffer[cc] == '\r') {
      if (buffer[cc] == ',') {
        ps[c++] = buffer + cc + 1;
      }
      buffer[cc] = '\0';
    }
  }
}

char buffers[128][1000][100];

void merge(m2 &sums, m2 &sums_j) {
  for (auto it_j = sums_j.begin(); it_j != sums_j.end(); it_j++) {
    const std::string &city = it_j->first;
    auto [it, isnew] = sums.try_emplace(it_j->first);
    if (isnew)
      it->second.first = it_j->second.first;
    else
      it->second.first += it_j->second.first;

    auto &products_prices_j = it_j->second.second;
    auto &products_prices = it->second.second;

    for (auto itt_j = products_prices_j.begin();
         itt_j != products_prices_j.end(); itt_j++) {
      auto [itt, ok] = products_prices.try_emplace(itt_j->first);
      if (ok || itt->second > itt_j->second) {
        itt->second = itt_j->second;
      }
    }
  }
}

int main() {
  m2 sums;
  std::mutex mu_sums;
  std::vector<std::thread> threads;
  FileReader fr;
  std::mutex mu_fr;

  unsigned int n = std::thread::hardware_concurrency();
  for (int j = 0; j < n; j++) {
    threads.emplace_back([j, &fr, &sums, &mu_fr, &mu_sums] {
      m2 sums_j;
      while (true) {
        int l;
        {
          // std::scoped_lock<std::mutex> slock(mu_fr);
          mu_fr.lock();
          if (!fr.have_more_lines()) {
            mu_fr.unlock();
            break;
          }
          // copy lines into buffer
          l = 0;
          while (l < 1000 && fr.readLine()) {
            strcpy(buffers[j][l], fr.buffer());
            ++l;
          }
          mu_fr.unlock();
        }
        // process data
        for (l--; l >= 0; l--) {
          char *p[2];
          char *b = buffers[j][l];
          split(b, p);
          auto [it, ok] = sums_j.try_emplace(b);
          long nprice = atoi(p[1]);
          it->second.first += nprice;

          auto [itt, okk] = it->second.second.try_emplace(p[0]);

          if (okk || itt->second > nprice) {
            itt->second = nprice;
          }
        }
      }
      mu_sums.lock();
      merge(sums, sums_j);
      mu_sums.unlock();
    });
  }

  for (int j = 0; j < n; j++)
    threads[j].join();

  printResult(sums);

  return 0;
}
