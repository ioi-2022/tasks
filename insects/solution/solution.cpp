#include "insects.h"
#include <bits/stdc++.h>
using namespace std;

int min_cardinality(int N) {
  int colors = 0;
  vector <int> insects;
  vector <int> order(N);
  iota(order.begin(),order.end(),0);
  
  mt19937 mt(715082022);
  shuffle(order.begin(),order.end(),mt);
  for (int x : order) {
    move_inside(x);
    if (press_button() == 1) {
      colors++;
    }
    else {
      move_outside(x);
      insects.push_back(x);
    }
  }
  
  int ans = 1;
  int rest = N - colors;
  vector <int> in, out;
  
  while (rest >= colors) {
    int mid = ans + (rest + colors + 1) / (2 * colors);
    
    shuffle(in.begin(),in.end(),mt);
    while ((int)in.size() > mid - ans) {
      int x = in.back();
      in.pop_back();
      insects.push_back(x);
      move_outside(x);
      
      if ((int)in.size() % colors == 0) {
        if (press_button() <= mid) {
          break;
        }
      }
    }
    
    shuffle(insects.begin(),insects.end(),mt);
    for (int x : insects) {
      if ((int)in.size() == colors * (mid - ans)) {
        out.push_back(x);
        continue;
      }
      move_inside(x);
      if ((int)in.size() >= mid - ans && press_button() > mid) {
        move_outside(x);
        out.push_back(x);
      }
      else {
        in.push_back(x);
      }
    }
    
    if ((int)in.size() == colors * (mid - ans)) {
      ans = mid;
      rest = out.size();
      insects = out;
      in.clear();
      out.clear();
    }
    else {
      rest = in.size();
      out.clear();
      insects.clear();
    }
  }
  
  return ans;
}
