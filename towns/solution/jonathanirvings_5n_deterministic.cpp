#include "towns.h"

#include <bits/stdc++.h>
using namespace std;

int find_town(int N) {
  vector<vector<int>> checked(N, vector<int>(N, -1));

  auto _check_road = [&] (int A, int B) {
    if (checked[A][B] == -1) {
      checked[A][B] = check_road(A, B);
      checked[B][A] = 1 - checked[A][B];
    }
    return checked[A][B];
  };

  int no_out_candidate = 0;
  for (int i = 1; i < N; ++i) {
    if (_check_road(no_out_candidate, i)) {
      no_out_candidate = i;
    }
  }

  vector<int> candidates;
  for (int i = 0; i < N; ++i) {
    if (i != no_out_candidate) {
      candidates.push_back(i);
    }
  }
  while (true) {
    bool found = false;
    for (int i = 0; i < static_cast<int>(candidates.size()) && !found; ++i) {
      for (int j = i + 1;
           j < static_cast<int>(candidates.size()) && !found; ++j) {
        if (checked[candidates[i]][candidates[j]] == -1) {
          if (_check_road(candidates[i], candidates[j])) {
            candidates[i] = candidates.back();
          } else {
            candidates[j] = candidates.back();
          }
          candidates.pop_back();
          found = true;
        }
      }
    }
    if (!found) {
      break;
    }
  }
  candidates.push_back(no_out_candidate);

  for (int u : candidates) {
    int outdeg = 0;
    for (int i = 0; i < N && outdeg <= 1; ++i) {
      if (u != i && _check_road(u, i)) {
        ++outdeg;
      }
    }
    if (outdeg <= 1) {
      return u;
    }
  }

  return -1;
}
