#include "insects.h"
#include <bits/stdc++.h>
using namespace std;

int min_cardinality(int N) {
  int ans = N;
  int L = 1, R = N - 1;
  while (L <= R) {
    int mid = (L + R)/2;
    vector <int> in, out, rest;
    
    for (int i = 0; i < N; i++) {
      move_inside(i);
      if (press_button() > mid) {
        move_outside(i);
        out.push_back(i);
      }
      else {
        in.push_back(i);
      }
    }
    
    for (int x : in) {
      move_outside(x);
    }
    
    for (int x : out) {
      move_inside(x);
      if (press_button() == 2) {
        move_outside(x);
      }
      else {
        rest.push_back(x);
      }
    }
    
    bool full = true;
    for (int x : in) {
      move_inside(x);
      if (press_button() == 1) {
        move_outside(x);
        full = false;
        break;
      }
      move_outside(x);
    }
    
    for (int x : rest) {
      move_outside(x);
    }
    
    if (full) {
      L = mid + 1;
    }
    else {
      R = mid - 1;
      ans = mid;
    }
  }
  
  return ans;
}
