#include "towers.h"

#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> H;
int max_idx;

void init(int _N, std::vector<int> _H) {
  N = _N;
  H = _H;
  for (int i = 0 ; i < N ; i++) {
    if (H[i] > H[max_idx]) {
      max_idx = i;
    } 
  }
}

int max_towers(int L, int R, int D) {
  if (R <= max_idx || L >= max_idx || max(H[L], H[R])+D > H[max_idx]) return 1;
  return 2;
}
