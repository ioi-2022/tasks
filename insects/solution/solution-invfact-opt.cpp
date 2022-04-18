#include "insects.h"
#include <bits/stdc++.h>
using namespace std;

int min_cardinality(int N) {
  int colors = 0;
  vector <bool> mark(N);
  for (int i = N - 1; i >= 0; i--) {
    move_inside(i);
    if (press_button() == 1) {
      colors++;
      mark[i] = true;
    }
    else {
      move_outside(i);
    }
  }
  
  for (int i = 0; i < N; i++) {
    if (mark[i]) {
      move_outside(i);
    }
  }
  
  int ans = N / colors - 1;
  while (true) {
    vector <int> in;
    int B = 0;
    for (int i = 0; i < N; i++) {
      move_inside(i);
      if (press_button() > ans) {
        move_outside(i);
        if (mark[i]) {
          B++;
        }
      }
      else {
        in.push_back(i);
      }
    }
    
    if (B == colors) {
      return ans + 1;
    }
    
    int C = colors - B;
    int M = (int)in.size() - B * ans;
    
    if (C == 1) {
      return M;
    }
    if (M == C * ans) {
      return ans;
    }
    
    for (int x : in) {
      move_outside(x);
    }
    ans = M / C - 1;
  }
}
