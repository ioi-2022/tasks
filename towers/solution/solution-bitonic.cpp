#include "towers.h"

#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> H;
int peak_index;

void init(int _N, std::vector<int> _H) {
  N = _N;
  H = _H;
  peak_index = N - 1;
  for (int i = 0; i < N - 1; ++i) {
    if (H[i] > H[i + 1]) {
      peak_index = i;
      break;
    }
  }
}

int max_towers(int L, int R, int D) {
  if (L <= peak_index && peak_index <= R) {
    return 1 + (max(H[L], H[R]) <= (H[peak_index] - D) ? 1 : 0);
  }
  return 1;
}
