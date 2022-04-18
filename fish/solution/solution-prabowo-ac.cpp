#include "fish.h"

#include <climits>

#include <algorithm>
#include <numeric>
#include <utility>
#include <vector>

long long max_weights(int N, int M, std::vector<int> X, std::vector<int> Y,
                      std::vector<int> W) {
  std::vector<std::vector<std::pair<int, long long>>> fish(N + 2);
  for (int i = 0; i < M; ++i) {
    fish[X[i] + 1].emplace_back(Y[i], W[i]);
  }

  fish[0] = {{-1, 0LL}};
  for (int i = 1; i <= N; ++i) {
    fish[i].emplace_back(-1, 0LL);
    std::sort(fish[i].begin(), fish[i].end());
    for (int j = 1; j < static_cast<int>(fish[i].size()); ++j) {
      fish[i][j].second += fish[i][j - 1].second;
    }
  }
  fish[N + 1] = {{-1, 0LL}};

  auto getRowWeight = [&](int row, int col) {
    return (upper_bound(fish[row].begin(), fish[row].end(), std::make_pair(col, LLONG_MAX)) - 1)->second;
  };

  std::vector<std::vector<int>> indices(N + 2);
  indices[0] = {-1};
  for (int i = 1; i <= N; ++i) {
    for (std::pair<int, int> f : fish[i - 1]) {
      indices[i].push_back(f.first);
    }
    for (std::pair<int, int> f : fish[i + 1]) {
      indices[i].push_back(f.first);
    }
    std::sort(indices[i].begin(), indices[i].end());
    indices[i].erase(std::unique(indices[i].begin(), indices[i].end()), indices[i].end());
  }
  indices[N + 1] = {-1};

  std::vector<long long> dpInc = {0LL}, dpDec = {0LL};
  for (int i = 1; i <= N + 1; ++i) {
    std::vector<long long> ndpInc(indices[i].size());
    long long pref = 0LL;
    int pidx = 0;
    for (int j = 0; j < static_cast<int>(indices[i].size()); ++j) {
      int col = indices[i][j];
      while (pidx < static_cast<int>(indices[i - 1].size()) && indices[i - 1][pidx] <= col) {
        pref = std::max(pref, dpInc[pidx] - getRowWeight(i - 1, indices[i - 1][pidx]));
        ++pidx;
      }
      ndpInc[j] = std::max(pref + getRowWeight(i - 1, col), dpDec[0]);
    }

    std::vector<long long> ndpDec(indices[i].size());
    long long suff = 0;
    int sidx = indices[i - 1].size();
    for (int j = static_cast<int>(indices[i].size()) - 1; j >= 0; --j) {
      int col = indices[i][j];
      while (sidx > 0 && indices[i - 1][sidx - 1] >= col) {
        --sidx;
        suff = std::max(suff, std::max(dpInc[sidx], dpDec[sidx]) + getRowWeight(i, indices[i - 1][sidx]));
      }
      ndpDec[j] = suff - getRowWeight(i, col);
    }

    std::swap(dpInc, ndpInc);
    std::swap(dpDec, ndpDec);
  }
  return std::max(dpInc[0], dpDec[0]);
}
