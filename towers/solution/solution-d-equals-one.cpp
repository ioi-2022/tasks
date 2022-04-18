#include "towers.h"

#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> H;
vector<int> pre_sum;

void init(int _N, std::vector<int> _H) {
  N = _N;
  H = _H;
  pre_sum.resize(N);
  for (int i = 1; i < N - 1; ++i) {
    pre_sum[i] = pre_sum[i - 1];
    if (H[i] < min(H[i - 1], H[i + 1])) {
      pre_sum[i] += 1;
    }
  }
}

int max_towers(int L, int R, int D) {
  if (L == R) {
    return 1;
  }
  int answer = pre_sum[R - 1] - pre_sum[L];
  if (H[L] < H[L + 1]) {
    ++answer;
  }
  if (H[R] < H[R - 1]) {
    ++answer;
  }
  return answer;
}
