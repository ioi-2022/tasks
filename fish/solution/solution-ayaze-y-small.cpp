#include "fish.h"

#include <bits/stdc++.h>
using namespace std;

const int kMaxN = 100'000;
const int kMaxY = 8;
const long long kInf = 4e18;

long long psum[kMaxN][kMaxY+1];
long long dp[kMaxN][kMaxY+1][kMaxY+1];
int N;

long long solve(int col, int y_bef2, int y_bef) {
  if (col == N) {
    return 0;
  }
  long long &ret = dp[col][y_bef2][y_bef];
  if (ret != -1) {
    return ret;
  }

  ret = -kInf;
  for (int y = 0 ; y <= kMaxY ; y++) {
    long long add = 0;

    if (col > 0) add -= psum[col][min(y, y_bef)];
    if (col > 0) {
      int covered = max(y_bef2, y_bef);
      covered = min(covered, y);
      add += (psum[col-1][y] - psum[col-1][covered]);
    }
    if (col+1 < N) add += psum[col+1][y];

    ret = max(ret, add + solve(col+1, y_bef, y));
  }

  return ret;
}

long long max_weights(int _N, int M, std::vector<int> X, std::vector<int> Y,
                      std::vector<int> W) {
  N = _N;
  for (int i = 0 ; i < M ; i++) {
    psum[X[i]][Y[i]+1] += W[i]; // increment y by one to ease our life
  }
  for (int i = 0 ; i < N ; i++) {
    for (int j = 1 ; j <= kMaxY ; j++) {
      psum[i][j] += psum[i][j-1];
    }
  }

  memset(dp, -1, sizeof dp);
  long long ret = solve(0, 0, 0);
  return ret;
}
