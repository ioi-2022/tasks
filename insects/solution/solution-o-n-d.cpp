#include "insects.h"
#include <bits/stdc++.h>
using namespace std;

int min_cardinality(int N) {
  int ans = N;
  vector <int> insects(N);
  iota(insects.begin(), insects.end(), 0);
  while (!insects.empty()) {
    int cnt = 1;
    vector <int> next;
    move_inside(insects[0]);
    for (int i = 1; i < (int)insects.size(); i++) {
      move_inside(insects[i]);
      if (press_button() == 2) {
        cnt++;
      }
      else {
        next.push_back(insects[i]);
      }
      move_outside(insects[i]);
    }
    move_outside(insects[0]);
    insects = next;
    ans = min(ans,cnt);
  }
  return ans;
}
