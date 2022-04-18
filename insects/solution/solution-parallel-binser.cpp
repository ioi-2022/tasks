#include "insects.h"
#include <bits/stdc++.h>
using namespace std;

int min_cardinality(int N) {
  vector <int> colors;
  vector <int> L(N), R(N);
  vector <int> cnt;
  vector <vector<int>> mid;
  
  int rest = 0;
  for (int i = 0; i < N; i++) {
    move_inside(i);
    if (press_button() == 1) {
      R[i] = L[i] = colors.size();
      colors.push_back(i);
      cnt.push_back(1);
      mid.push_back(vector<int>(0));
    }
    else {
      L[i] = 0;
      R[i] = (int)colors.size() - 1;
      move_outside(i);
      mid[(L[i] + R[i]) / 2].push_back(i);
      rest++;
    }
  }
  
  while (rest > 0) {
    for (int i = 0; i < (int)colors.size(); i++) {
      move_outside(colors[i]);
      for (int x : mid[i]) {
        move_inside(x);
        if (press_button() == 1) {
          R[x] = i;
        }
        else {
          L[x] = i + 1;
        }
        move_outside(x);
        
        if (L[x] < R[x]) {
          mid[(L[x] + R[x]) / 2].push_back(x);
        }
        else {
          cnt[L[x]]++;
          rest--;
        }
      }
      mid[i].clear();
    }
    
    for (int i = 0; i < (int)colors.size(); i++) {
      move_inside(colors[i]);
      for (int x : mid[i]) {
        move_inside(x);
        if (press_button() == 2) {
          R[x] = i;
        }
        else {
          L[x] = i + 1;
        }
        move_outside(x);
        
        if (L[x] < R[x]) {
          mid[(L[x] + R[x]) / 2].push_back(x);
        }
        else {
          cnt[L[x]]++;
          rest--;
        }
      }
      mid[i].clear();
    }
  }
  
  int ans = N;
  for (int x : cnt) {
    ans = min(ans,x);
  }
  return ans;
}
