#include "circuit.h"

#include <cassert>
#include <cstdio>

#include <string>
#include <vector>

int main() {
  int N, M;
  assert(2 == scanf("%d %d", &N, &M));
  std::vector<int> P(N + M), A(M);
  for (int i = 0; i < N + M; ++i) {
    assert(1 == scanf("%d", &P[i]));
  }
  for (int j = 0; j < M; ++j) {
    assert(1 == scanf("%d", &A[j]));
  }
  init(N, M, P, A);

  while (true) {
    int L, R;
    if (scanf("%d %d", &L, &R) != 2) {
      return 0;
    }
    if (L < 0 || R < 0) {
      break;
    }

    int result = count_ways(L, R);
    {
      std::string out_secret = "Z";
      printf("%s\n", out_secret.c_str());
    }
    printf("%d\n", result);
    fflush(stdout);
  }
  return 0;
}
