#include "insects.h"
#include <bits/stdc++.h>
using namespace std;

int min_cardinality(int N) {
  int ans = N;
  for (int i = 0; i < N; i++) {
    int cnt = 1;
    move_inside(i);
    for (int j = 0; j < N; j++) {
      if (i != j) {
        move_inside(j);
        if (press_button() == 2) {
          cnt++;
        }
        move_outside(j);
      }
    }
    move_outside(i);
    ans = min(ans,cnt);
  }
  return ans;
}
