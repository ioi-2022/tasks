#include "fish.h"

#include <bits/stdc++.h>
using namespace std;

const int kMaxN = 300;
const int kUp = 0, kDown = 1;
const long long kInf = 4e18;

long long psum[kMaxN][kMaxN+1];
long long dp[kMaxN][kMaxN+1][2];
int N;

long long solve(int col, int y_bef, int mode) {
  if (col >= N) {
    return 0;
  }
  long long &ret = dp[col][y_bef][mode];
  if (ret != -1) {
    return ret;
  }

  ret = -kInf;
  
  for (int i = 1 ; i <= N ; i++) {
    long long add = 0;
    if (col+1 < N) add += psum[col+1][i];
    if (col > 0) add -= psum[col][min(i, y_bef)];
    if (col > 0 && i > y_bef) add += (psum[col-1][i] - psum[col-1][y_bef]);

    if (mode == kUp) {
      ret = max(ret, add + solve(col+1, i, i >= y_bef ? kUp : kDown));
    }
    if (mode == kDown) {
      if (i <= y_bef) ret = max(ret, add + solve(col+1, i, kDown));
      else break;
    }
  }

  // if we turn this into 0
  ret = max(ret, solve(col+2, 0, kUp));
  if (y_bef == 0) {
    ret = max(ret, solve(col+1, 0, kUp));
  }
  if (col+2 <= N) {
    // set for col+1
    for (int i = 1 ; i <= N ; i++) {
      long long add = 0;
      if (col+2 < N) add += psum[col+2][i];
      if (col+1 < N) add += (psum[col][max(i, y_bef)] - psum[col][y_bef]);
      ret = max(ret, add + solve(col+2, i, kUp));
    }
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
    for (int j = 1 ; j <= N ; j++) {
      psum[i][j] += psum[i][j-1];
    }
  }

  memset(dp, -1, sizeof dp);
  long long ret = solve(0, 0, kUp);
  return ret;
}
