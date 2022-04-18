// O(N + M + Q log M)

#include "circuit.h"

#include <algorithm>
#include <vector>

const int kMod = 1000002022;

std::vector<int> dp0, dp1;

void compute(int u) {
  int l = u * 2 + 1, r = u * 2 + 2;
  int diff = (1LL * dp0[l] * dp1[r] + 1LL * dp1[l] * dp0[r]) % kMod;
  dp0[u] = (2LL * dp0[l] * dp0[r] + diff) % kMod;
  dp1[u] = (2LL * dp1[l] * dp1[r] + diff) % kMod;
}

void init(int N, int M, std::vector<int>, std::vector<int> A) {
  dp0.resize(N + M);
  dp1.resize(N + M);
  for (int i = N; i < N + M; ++i) {
    if (A[i - N]) {
      dp1[i] = 1;
    } else {
      dp0[i] = 1;
    }
  }

  for (int i = N - 1; i >= 0; --i) {
    compute(i);
  }
}

int count_ways(int L, int R) {
  for (int i = L; i <= R; ++i) {
    std::swap(dp0[i], dp1[i]);
    for (int u = (i - 1) / 2; u > 0; u = (u - 1) / 2) {
      compute(u);
    }
    compute(0);
  }
  return dp1[0];
}
