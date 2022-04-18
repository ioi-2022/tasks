#include "towers.h"

#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> H;

void init(int _N, std::vector<int> _H) {
  N = _N;
  H = _H;
}

int max_towers(int L, int R, int D) {
  set<pair<int, int>> height_idxs, idx_heights;
  set<int> taken;

  vector<pair<int, int>> vec;
  for (int i = L ; i <= R ; i++) {
    vec.push_back({H[i], i});
    height_idxs.insert({H[i], i});
    idx_heights.insert({i, H[i]});
  }

  sort(vec.begin(), vec.end());
  for (auto [h, i] : vec) {
    while (!height_idxs.empty() && height_idxs.begin()->first < h+D) {
      auto [height, idx] = *height_idxs.begin();
      height_idxs.erase({height, idx});
      idx_heights.erase({idx, height});
    }

    bool can_take = true;

    { // left side
      auto it = taken.lower_bound(i);
      if (it != taken.begin()) {
        it--;
        auto it2 = idx_heights.lower_bound({*it, -1});
        if (it2 == idx_heights.end() || it2->first > i) {
          can_take = false;
        }
      }
    }

    { // right side
      auto it = taken.upper_bound(i);
      if (it != taken.end()) {
        auto it2 = idx_heights.lower_bound({*it, -1});
        if (it2 == idx_heights.begin() || (--it2)->first < i) {
          can_take = false;
        }
      }
    }

    if (can_take) {
      taken.insert(i);
    }
  }

  return taken.size();
}
