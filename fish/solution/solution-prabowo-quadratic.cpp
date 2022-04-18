#include "fish.h"

#include <climits>

#include <algorithm>
#include <numeric>
#include <utility>
#include <vector>

long long max_weights(int N, int M, std::vector<int> X, std::vector<int> Y,
                      std::vector<int> W) {
  std::vector<std::vector<long long>> fish(N + 2, std::vector<long long>(N + 1));
  for (int i = 0; i < M; ++i) {
    fish[X[i] + 1][Y[i] + 1] = W[i];
  }

  for (int i = 1; i <= N; ++i) {
    for (int j = 1; j <= N; ++j) {
      fish[i][j] += fish[i][j - 1];
    }
  }

  std::vector<long long> dpInc(N + 1, -1e18), dpDec(N + 1, -1e18);
  std::vector<long long> ndpInc(N + 1), ndpDec(N + 1);
  dpInc[0] = dpDec[0] = 0;
  for (int i = 1; i <= N + 1; ++i) {
    long long pref = 0LL;
    for (int j = 0; j <= N; ++j) {
      pref = std::max(pref, dpInc[j] - fish[i - 1][j]);
      ndpInc[j] = std::max(pref + fish[i - 1][j], dpDec[0]);
    }

    long long suff = 0LL;
    for (int j = N; j >= 0; --j) {
      suff = std::max(suff, std::max(dpInc[j], dpDec[j]) + fish[i][j]);
      ndpDec[j] = suff - fish[i][j];
    }

    std::swap(dpInc, ndpInc); std::swap(dpDec, ndpDec);
  }
  return std::max(dpInc[0], dpDec[0]);
}
