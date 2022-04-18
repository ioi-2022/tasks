#include "fish.h"

#include <bits/stdc++.h>
using namespace std;

const int kMaxN = 100'000;
const int kMaxFishInColumn = 3;

vector<pair<int, long long>> compressed_cover[kMaxN];
vector<pair<int, long long>> compressed_column[kMaxN];
long long dp[kMaxN][2*kMaxFishInColumn+2][2*kMaxFishInColumn+2];
int N;

int getColumnIdx(int col, int y) {
  pair<int, long long> temp = {y+1, 0};
  auto it = lower_bound(compressed_column[col].begin(), compressed_column[col].end(), temp);
  return it - compressed_column[col].begin() - 1;
}

long long solve(int col, int bef2, int bef) {
  if (col == N) {
    return 0;
  }

  long long &ret = dp[col][bef2][bef];
  if (ret != -1) {
    return ret;
  }

  for (int i = 0 ; i < static_cast<int>(compressed_cover[col].size()) ; i++) {
    long long add = compressed_cover[col][i].second;
    if (col > 0) {
      int y_current_previously_covered = compressed_cover[col][i].first;
      y_current_previously_covered = min(y_current_previously_covered, compressed_cover[col-1][bef].first);
      int current_previously_covered = getColumnIdx(col, y_current_previously_covered);
      add -= compressed_column[col][current_previously_covered].second;

      int y_bef_already_covered = compressed_cover[col-1][bef].first;
      if (bef2 > 0) y_bef_already_covered = max(y_bef_already_covered, compressed_cover[col-2][bef2].first);
      y_bef_already_covered = min(y_bef_already_covered, compressed_cover[col][i].first);
      int bef_already_covered = getColumnIdx(col-1, y_bef_already_covered);
      add -= compressed_column[col-1][bef_already_covered].second;
    }
    ret = max(ret, add + solve(col+1, bef, i));
  }

  return ret;
}

long long max_weights(int _N, int M, std::vector<int> X, std::vector<int> Y,
                      std::vector<int> W) {
  N = _N;
  for (int i = 0 ; i < M ; i++) {
    if (X[i] > 0) compressed_cover[X[i]-1].push_back({Y[i]+1, W[i]});
    if (X[i]+1 < N) compressed_cover[X[i]+1].push_back({Y[i]+1, W[i]});
    compressed_column[X[i]].push_back({Y[i]+1, W[i]});
  }


  for (int i = 0 ; i < N ; i++) {
    auto preparePrefixSum = [](vector<pair<int, long long>> &psum) {
      psum.push_back({0, 0ll});
      sort(psum.begin(), psum.end());
      for (int j = 1 ; j < static_cast<int>(psum.size()) ; j++) {
        psum[j].second += psum[j-1].second;
      }
    };

    preparePrefixSum(compressed_column[i]);
    preparePrefixSum(compressed_cover[i]);
  }


  memset(dp, -1, sizeof dp);
  long long ret = solve(0, 0, 0);
  return ret;
}
