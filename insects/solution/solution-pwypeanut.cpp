#include "insects.h"
#include <bits/stdc++.h>
using namespace std;

int find_ans(set<int> S, int mx, int tt) {
  if (mx <= 0) return 0;
  if (S.size() == 0) return 0;
  int test = (mx + 1) / 2;
  set<int> lefty = S;
  vector<int> inside;
  for (auto it = (S.begin()); it != S.end(); it++) {
    move_inside(*it);
    if (press_button() > test) {
      move_outside(*it);
    } else {
      inside.push_back(*it);
      lefty.erase(*it);
    }
  }
  for (int i = 0; i < (int)inside.size(); i++) move_outside(inside[i]);
  if ((int)inside.size() == test * tt) {
    return find_ans(lefty, mx - test, tt) + test;
  } else {
    set<int> res;
    for (int i = 0; i < (int)inside.size(); i++) res.insert(inside[i]);
    return find_ans(res, test - 1, tt);
  }
}

int min_cardinality(int N) {
  set<int> lefty;
  for (int i = 1; i < N; i++) lefty.insert(i);
  vector<int> types;
  types.push_back(0);
  move_inside(0);
  for (int i = 1; i < N; i++) {
    move_inside(i);
    if (press_button() == 2) {
      move_outside(i);
    } else {
      types.push_back(i);
      lefty.erase(i);
    }
  }
  for (int i = 0; i < (int)types.size(); i++) move_outside(types[i]);
  if (types.size() == 1) return N;
  return find_ans(lefty, ceil((double)N / types.size()) - 1, types.size()) + 1;
}
