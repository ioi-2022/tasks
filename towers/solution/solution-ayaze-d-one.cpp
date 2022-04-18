#include "towers.h"

#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> H;
vector<int> bottoms;

void init(int _N, std::vector<int> _H) {
  N = _N;
  H = _H;
  for (int i = 1 ; i+1 < N ; i++) {
    if (H[i] < H[i+1] && H[i] < H[i-1]) {
      bottoms.push_back(i);
    }
  }
}

int max_towers(int L, int R, int) {
  if (R-L+1 <= 2) return 1;
  
  int bottom_count = upper_bound(bottoms.begin(), bottoms.end(), R-1) - 
                     lower_bound(bottoms.begin(), bottoms.end(), L+1);
  
  if (bottom_count == 0) {
    if (H[L] < H[L+1] && H[R] < H[R-1]) return 2;
    return 1;
  }

  int ret = bottom_count;

  int l_idx = lower_bound(bottoms.begin(), bottoms.end(), L+1) - bottoms.begin();
  int l = bottoms[l_idx];
  if (l > L+1 && H[L] < H[L+1]) ret++;

  int r_idx = upper_bound(bottoms.begin(), bottoms.end(), R-1) - bottoms.begin() - 1;
  int r = bottoms[r_idx];
  if (r < R-1 && H[R] < H[R-1]) ret++;

  return ret;
}
