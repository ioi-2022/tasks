#include "fish.h"

#include <bits/stdc++.h>
using namespace std;

const int kMaxN = 100'000;

long long dp[kMaxN][2][2];
int arr[kMaxN];
int N;

long long solve(int col, bool bef2, bool bef) {
  if (col == N) {
    return 0;
  }
  long long &ret = dp[col][bef2][bef];
  if (ret != -1) {
    return ret;
  }

  // skip
  ret = solve(col+1, bef, false);

  // take
  long long add = 0;
  if (col > 0 && bef) add -= arr[col];
  if (col > 0 && !bef2 && !bef) add += arr[col-1];
  if (col+1 < N) add += arr[col+1];
  ret = max(ret, add + solve(col+1, bef, true));

  return ret;
}

long long max_weights(int _N, int M, std::vector<int> X, std::vector<int>,
                      std::vector<int> W) {
  N = _N;
  for (int i = 0 ; i < M ; i++) {
    arr[X[i]] = W[i];
  }

  memset(dp, -1, sizeof dp);
  long long ret = solve(0, false, false);
  return ret;
}
