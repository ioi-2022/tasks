#include "towers.h"
#include <bits/stdc++.h>
using namespace std;

int N;
vector <int> H;

void init(int _N,vector<int> _H) {
  N = _N;
  H = _H;
}

int max_towers(int L,int R,int D) {
  int last = L, ans = 1;
  bool up = true;
  for (int i = L; i <= R; i++) {
    if (up) {
      if (H[i] >= H[last] + D) {
        up = false;
        last = i;
      }
      if (H[i] < H[last]) {
        last = i;
      }
    }
    else {
      if (H[i] <= H[last] - D) {
        up = true;
        last = i;
        ans++;
      }
      if (H[i] > H[last]) {
        last = i;
      }
    }
  }
  return ans;
}
