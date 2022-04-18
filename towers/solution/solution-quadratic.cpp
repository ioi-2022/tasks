#include "towers.h"

#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> H;
vector<bool> used;

void init(int _N, std::vector<int> _H) {
  N = _N;
  H = _H;
  used.resize(N);
}

int max_towers(int L, int R, int D) {
  vector<int> towers(R - L + 1);
  iota(towers.begin(), towers.end(), L);
  sort(towers.begin(), towers.end(), [&] (int i, int j) {
    return H[i] < H[j];
  });
  fill(used.begin() + L, used.begin() + R + 1, false);
  for (int i : towers) {
    bool can_use = true;
    for (int dir : {-1, 1}) {
      for (int j = i + dir; L <= j && j <= R && can_use; j += dir) {
        if (H[i] <= H[j] - D) {
          break;
        }
        if (used[j]) {
          can_use = false;
        }
      }
    }
    if (can_use) {
      used[i] = true;
    }
  }
  return count(used.begin() + L, used.begin() + R + 1, true);
}
