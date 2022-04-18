#include "fish.h"

#include <bits/stdc++.h>
using namespace std;

long long max_weights(int N, int M, std::vector<int> X, std::vector<int> Y,
                      std::vector<int> W) {
  vector<long long> psum_0(N+1, 0), psum_1(N+1, 0);

  for (int i = 0 ; i < M ; i++) {
    if (X[i] == 0) psum_0[Y[i]+1] += W[i];
    if (X[i] == 1) psum_1[Y[i]+1] += W[i];
  }
  for (int i = 1 ; i <= N ; i++) {
    psum_0[i] += psum_0[i-1];
    psum_1[i] += psum_1[i-1];
  }

  long long ret = psum_1[N];
  for (int i = 0 ; i <= N ; i++) {
    long long val = psum_0[i];
    if (N > 2) val += (psum_1[N] - psum_1[i]);
    ret = max(ret, val);
  }

  return ret;
}
