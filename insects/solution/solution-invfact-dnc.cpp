#include "insects.h"
#include <bits/stdc++.h>
using namespace std;

int min_cardinality(int N) {
  vector <int> color;
  vector <int> insects;
  for (int i = 0; i < N; i++) {
    move_inside(i);
    if (press_button() == 1) {
      color.push_back(i);
    }
    else {
      move_outside(i);
      insects.push_back(i);
    }
  }
  
  int colors = color.size();
  for (int x : color) {
    move_outside(x);
  }
  
  int ans = N / colors - 1;
  while (true) {
    vector <int> in;
    for (int x : insects) {
      move_inside(x);
      if (press_button() > ans) {
        move_outside(x);
      }
      else {
        in.push_back(x);
      }
    }
    
    vector <int> candidate;
    for (int x : color) {
      move_inside(x);
      if (press_button() > ans) {
        move_outside(x);
      }
      else {
        candidate.push_back(x);
      }
    }
    
    int B = color.size();
    int C = candidate.size();
    int M = in.size();
    
    if (C == 0) {
      return ans + 1;
    }
    if (C == 1) {
      return M - ans * (B - C) + C;
    }
    if (M + C == ans * B) {
      return ans;
    }
    
    for (int x : in) {
      move_outside(x);
    }
    
    insects.clear();
    for (int x : in) {
      move_inside(x);
      if (press_button() == 2) {
        insects.push_back(x);
      }
      move_outside(x);
    }
    
    color = candidate;
    for (int x : color) {
      move_outside(x);
    }
    
    M = insects.size();
    ans = M / C;
  }
}
