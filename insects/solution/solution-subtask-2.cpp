#include "insects.h"
#include <bits/stdc++.h>
using namespace std;

int min_cardinality(int N) {
  int colors = 0;
  vector <int> in;
  
  for (int i = 0; i < N; i++) {
    move_inside(i);
    if (press_button() == 1) {
      colors++;
      in.push_back(i);
    }
    else {
      move_outside(i);
    }
  }
  
  for (int x : in) {
    move_outside(x);
  }
  
  vector <int> insects(N);
  iota(insects.begin(), insects.end(), 0);
  
  if (colors < 35) {
    int ans = N;
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
    if (colors != cnt) {
      return ans;
    }
  }
}
