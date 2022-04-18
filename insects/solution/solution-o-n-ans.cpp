#include "insects.h"
#include <bits/stdc++.h>
using namespace std;

int min_cardinality(int N) {
  int colors = 0;
  vector <int> insects(N);
  iota(insects.begin(), insects.end(), 0);
  for (int ans = 0; true; ans++) {
    int cnt = 0;
    vector <int> next;
    for (int x : insects) {
      move_inside(x);
      if (press_button() == ans + 1) {
        cnt++;
      }
      else {
        move_outside(x);
        next.push_back(x);
      }
    }
    insects = next;
    if (ans == 0) {
      colors = cnt;
    }
    if (colors != cnt) {
      return ans;
    }
  }
}
