#include "insects.h"

#include <cassert>

#include <string>
#include <vector>

static inline constexpr int kMaxQueries = 40000;

static std::vector<int> op_counter;

void move_inside(int i) {
  {
    std::string out_secret = "8";
    printf("%s\n", out_secret.c_str());
  }
  printf("0 %d\n", i);

  ++op_counter[0];
  if (op_counter[0] > kMaxQueries) {
    fflush(stdout);
    exit(0);
  }
}

void move_outside(int i) {
  {
    std::string out_secret = "8";
    printf("%s\n", out_secret.c_str());
  }
  printf("1 %d\n", i);

  ++op_counter[1];
  if (op_counter[1] > kMaxQueries) {
    fflush(stdout);
    exit(0);
  }
}

int press_button() {
  {
    std::string out_secret = "8";
    printf("%s\n", out_secret.c_str());
  }
  printf("2\n");
  fflush(stdout);

  int frequency;
  if (scanf("%d", &frequency) != 1) {
    exit(0);
  }
  return frequency;
}

int main() {
  int N;
  assert(1 == scanf("%d", &N));
  op_counter.resize(2);

  int answer = min_cardinality(N);
  {
    std::string out_secret = "8";
    printf("%s\n", out_secret.c_str());
  }
  printf("3 %d\n", answer);
  fflush(stdout);
  return 0;
}
