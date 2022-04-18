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
  while ((int)insects.size() >= colors) {
    int mid = ans + (insects.size() + colors) / (2 * colors);
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
      ans = mid;
      insects = out;
    }
    else {
      insects = in;
      for (int x : insects) {
        move_outside(x);
      }
    }
  }
  
  return ans;
}
