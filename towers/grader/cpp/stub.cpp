#include "towers.h"

#include <cassert>
#include <cstdio>

#include <string>
#include <vector>

int main() {
  int N;
  assert(1 == scanf("%d", &N));
  std::vector<int> H(N);
  for (int i = 0; i < N; ++i) {
    assert(1 == scanf("%d", &H[i]));
  }
  init(N, H);

  while (true) {
    int L, R, D;
    if (scanf("%d %d %d", &L, &R, &D) != 3) {
      return 0;
    }
    if (L < 0 || R < 0 || D < 0) {
      break;
    }
    int result = max_towers(L, R, D);
    {
      std::string out_secret = "T";
      printf("%s\n", out_secret.c_str());
    }
    printf("%d\n", result);
    fflush(stdout);
  }
  return 0;
}
