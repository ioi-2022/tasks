#include "insects.h"
#include <bits/stdc++.h>
using namespace std;

int min_cardinality(int N) {
  int colors = 0;
  vector <int> insects;
  for (int i = 0; i < N; i++) {
    move_inside(i);
    if (press_button() == 1) {
      colors++;
    }
    else {
      move_outside(i);
      insects.push_back(i);
    }
  }
  
  int ans = 1;
  int L = 2, R = N/colors;
  while (L <= R) {
    int mid = (L + R)/2;
    vector <int> in, out;
    
    for (int x : insects) {
      move_inside(x);
      if (press_button() > mid) {
        move_outside(x);
        out.push_back(x);
      }
      else {
        in.push_back(x);
      }
    }
    
    if ((int)in.size() == colors * (mid - ans)) {
      L = mid + 1;
      ans = mid;
      insects = out;
    }
    else {
      R = mid - 1;
      insects = in;
      for (int x : insects) {
        move_outside(x);
      }
    }
  }
  
  return ans;
}
