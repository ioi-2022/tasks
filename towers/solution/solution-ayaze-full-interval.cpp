#include "towers.h"

#include <bits/stdc++.h>
using namespace std;

const int kLogN = 17;
const int kInf = 2'000'000'000;

vector<int> D_bounds;

void init(int N, std::vector<int> H) {
  vector<pair<int, int>> ordered_H;
  for (int i = 0 ; i < N ; i++) {
    ordered_H.push_back({H[i], i});
  }
  sort(ordered_H.begin(), ordered_H.end());

  vector<int> left_lower(N, -1), right_lower(N, -1);
  set<int> active;
  for (auto [_, i] : ordered_H) {
    {
      auto it = active.lower_bound(i);
      if (it != active.begin()) {
        it--;
        left_lower[i] = *it;
      }
    }
    {
      auto it = active.upper_bound(i);
      if (it != active.end()) {
        right_lower[i] = *it;
      }
    }
    active.insert(i);
  }

  vector<vector<int>> sparse_table(N, vector<int>(kLogN, -1));
  for (int i = 0 ; i < N ; i++) {
    sparse_table[i][0] = H[i];
  }
  for (int j = 1 ; j < kLogN ; j++) {
    for (int i = 0 ; i < N ; i++) {
      if (i + (1 << (j-1)) < N) {
        sparse_table[i][j] = max(sparse_table[i][j-1], sparse_table[i + (1 << (j-1))][j-1]);
      }
    }
  }
  function<int(int, int)> queryMax = [&](int l, int r) -> int {
    int lg = 0;
    while (l + (1 << lg) - 1 <= r) lg++;
    lg--;
    return max(sparse_table[l][lg], sparse_table[r - (1 << lg) + 1][lg]);
  };

  for (int i = 0 ; i < N ; i++) {
    int D_max_left = kInf, D_max_right = kInf;
    
    if (left_lower[i] != -1) {
      D_max_left = -1;
      int mx = queryMax(left_lower[i], i);
      D_max_left = mx - H[i];
    }
    if (right_lower[i] != -1) {
      D_max_right = -1;
      int mx = queryMax(i, right_lower[i]);
      D_max_right = mx - H[i];
    }

    int D_max = min(D_max_left, D_max_right);
    if (D_max > 0) {
      D_bounds.push_back(D_max);
    }
  }
  
  sort(D_bounds.begin(), D_bounds.end());
}

int max_towers(int, int, int D) {
  return D_bounds.end() - lower_bound(D_bounds.begin(), D_bounds.end(), D);
}
