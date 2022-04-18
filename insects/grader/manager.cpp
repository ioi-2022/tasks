#include "testlib.h"

#include <cassert>
#include <csignal>
#include <cstdio>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <set>

inline FILE* openFile(const char* name, const char* mode) {
  FILE* file = fopen(name, mode);
  if (!file) {
    quitf(_fail, "Could not open file '%s' with mode '%s'", name, mode);
  }
  return file;
}

inline double roundToTwoDp(double score) {
  return round(score * 100) / 100;
}

static inline constexpr int kMaxQueries = 40000;

// Insect types are compressed to colors in the range [0, N).
static std::vector<int> color;
static std::vector<bool> in_box;

static std::vector<int> color_occurrences;
static std::multiset<int> max_occurrences;

static std::vector<int> op_counter(3, 0);

int main(int argc, char *argv[]) {
  testlibMode = _checker;
  ouf.mode = _output;

  if (argc < 3) {
    quit(_fail, "Insufficient number of args for manager of 'insects'");
  }

  {
    // Keep alive on broken pipes
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, NULL);
  }

  // Must be in this order
  FILE *fout = openFile(argv[2], "a");
  FILE *fin = openFile(argv[1], "r");

  int N;
  int is_partial;
  assert(2 == scanf("%d %d", &N, &is_partial));
  color.resize(N);
  in_box.assign(N, false);

  std::map<int, int> type_to_color;
  for (int i = 0; i < N; ++i) {
    int Ti;
    assert(1 == scanf("%d", &Ti));
    if (type_to_color.find(Ti) == type_to_color.end()) {
      int new_color = type_to_color.size();
      type_to_color[Ti] = new_color;
      max_occurrences.insert(0);
    }
    color[i] = type_to_color[Ti];
  }

  color_occurrences.assign(type_to_color.size(), 0);

  fprintf(fout, "%d\n", N);
  fflush(fout);

  while (true) {
    {
      std::string in_secret = "8";
      char secret[100];
      if (fscanf(fin, "%5s", secret) != 1) {
        quit(_sv, "Could not read secret (possibly, an unexpected termination"
                  " of the program)");
      }
      if (std::string(secret) != in_secret) {
        quit(_sv, "Secret mismatch (possible tampering with the output)");
      }
    }

    int op;
    if (fscanf(fin, "%d", &op) != 1) {
      quit(_fail, "Could not read op.");
    }

    if (op == 0 || op == 1) {
      ++op_counter[op];
      if (op_counter[op] > kMaxQueries) {
        quit(_sv, "Too many queries.");
      }

      int i;
      if (fscanf(fin, "%d", &i) != 1) {
        quit(_fail, "Could not read query parameters.");
      }
      if (i < 0 || i >= N) {
        quit(_sv, "Invalid query parameters.");
      }

      if (op == 0 && !in_box[i]) {
        in_box[i] = true;
        max_occurrences.erase(max_occurrences.find(color_occurrences[color[i]]));
        ++color_occurrences[color[i]];
        max_occurrences.insert(color_occurrences[color[i]]);
      }
      if (op == 1 && in_box[i]) {
        in_box[i] = false;
        max_occurrences.erase(max_occurrences.find(color_occurrences[color[i]]));
        --color_occurrences[color[i]];
        max_occurrences.insert(color_occurrences[color[i]]);
      }
    } else if (op == 2) {
      ++op_counter[2];
      if (op_counter[2] > kMaxQueries) {
        quit(_sv, "Too many queries.");
      }

      int frequency = *(max_occurrences.rbegin());
      fprintf(fout, "%d\n", frequency);
      fflush(fout);
    } else if (op == 3) {
      int answer;
      if (fscanf(fin, "%d", &answer) != 1) {
        quit(_fail, "Could not read answer.");
      }

      int Q = *std::max_element(op_counter.begin(), op_counter.end());

      for (int i = 0; i < N; ++i) {
        if (!in_box[i]) {
          ++color_occurrences[color[i]];
        }
      }
      int correct = N;
      for (int frequency : color_occurrences) {
        correct = std::min(correct, frequency);
      }

      if (answer == correct) {
        if (is_partial == 0) {
          quit(_ok);
        } else if (is_partial == 1) {
          if (Q <= 3 * N) {
            quit(_ok);
          } else if (Q <= 6 * N) {
            quitp(roundToTwoDp(81.0 - pow(Q * 1.0 / N, 2.0) * 2.0 / 3.0) / 75.0);
          } else if (Q <= 20 * N) {
            quitp(roundToTwoDp(225.0 / (Q * 1.0 / N - 2.0)) / 75.0);
          } else {
            quit(_wa, "Too many queries.");
          }
        } else {
          assert(false);
        }
      } else {
        quit(_wa, "Wrong answer.");
      }
    } else {
      quit(_fail, "Invalid value of op.");
    }
  }
}
