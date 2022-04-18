#include "towers.h"

#include <bits/stdc++.h>
using namespace std;

const int kLogN = 17;
const int kInf = 2'000'000'000;

struct Fenwick {
  int n;
  vector<vector<int>> tree;
  vector<vector<int>> nums;

  void init(int _n, vector<tuple<int, int, int>> vec) {
    n = _n;
    tree.clear();
    nums.clear();
    tree.resize(n+5);
    nums.resize(n+5);

    for (auto &[row, col, _] : vec) { // normalize
      row++; col++;
    }
    for (auto [row, col, _] : vec) {
      for (int i = row ; i < n+5 ; i += i & -i) {
        nums[i].push_back(col);
      }
    }
    for (int i = 0 ; i < n+5 ; i++) {
      nums[i].push_back(-1);
      sort(nums[i].begin(), nums[i].end());
      nums[i].erase(unique(nums[i].begin(), nums[i].end()), nums[i].end());
      tree[i].resize(nums[i].size());
    }
    for (auto [row, col, val] : vec) {
      update(row, col, val);
    }
  }

  int getIdx(int r, int val) {
    return upper_bound(nums[r].begin(), nums[r].end(), val) - nums[r].begin() - 1;
  }

  void update(int r, int c, int val) {
    for (int i = r ; i < n+5 ; i += i & -i) {
      int idx = getIdx(i, c);
      for (int j = idx ; j < static_cast<int>(tree[i].size()) ; j += j & -j) {
        tree[i][j] += val;
      }
    }
  }

  int query(int l, int r) {
    l++; r++;
    int ret = 0;
    for (int x = l ; x > 0 ; x -= x & -x) {
      int idx = getIdx(x, r);
      for (int j = idx ; j > 0 ; j -= j & -j) {
        ret += tree[x][j];
      }
    }
    return ret;
  }
};

vector<int> D_bounds_left, D_bounds_right;
vector<int> left_lower, right_lower;
int N;

void init(int _N, std::vector<int> H) {
  N = _N;

  vector<pair<int, int>> ordered_H;
  for (int i = 0 ; i < N ; i++) {
    ordered_H.push_back({H[i], i});
  }
  sort(ordered_H.begin(), ordered_H.end());

  left_lower.resize(N, -1);
  right_lower.resize(N, -1);
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

    D_bounds_left.push_back(D_max_left);
    D_bounds_right.push_back(D_max_right);
  }
}

Fenwick ft;
int last_D = -1;

void build(int D) {
  vector<tuple<int, int, int>> vec;
  for (int i = 0 ; i < N ; i++) {
    int l, r;

    if (left_lower[i] == -1 || D_bounds_left[i] >= D) l = 0; 
    else l = left_lower[i]+1;
    
    if (right_lower[i] == -1 || D_bounds_right[i] >= D) r = N-1;
    else r = right_lower[i]-1;
    // l <= L <= i <= R <= r
    // (l, i) to (i, r)
    // add(l, i)
    // dec(l, r+1)
    // dec(i+1, i)
    // add(i+1, r+1)
    vec.push_back(make_tuple(l, i, 1));
    vec.push_back(make_tuple(l, r+1, -1));
    vec.push_back(make_tuple(i+1, i, -1));
    vec.push_back(make_tuple(i+1, r+1, 1));
  }

  ft.init(N, vec);
}

int max_towers(int L, int R, int D) {
  if (D != last_D) {
    last_D = D;
    build(D);
  }

  return ft.query(L, R);
}
