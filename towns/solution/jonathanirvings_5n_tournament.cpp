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
  {
    vector<int> candidates(N);
    iota(candidates.begin(), candidates.end(), 0);
    mt19937 rng;
    while (candidates.size() > 1) {
      vector<int> new_candidates;
      for (int i = 0; i < candidates.size(); i += 2) {
        if (i + 1 == candidates.size()) {
          new_candidates.push_back(candidates[i]);
        } else {
          if (_check_road(candidates[i], candidates[i + 1])) {
            new_candidates.push_back(candidates[i + 1]);
          } else {
            new_candidates.push_back(candidates[i]);
          }
        }
      }
      candidates = new_candidates;
    }
    no_out_candidate = candidates[0];
  }

  vector<int> candidates;
  for (int i = 0; i < N; ++i) {
    if (i != no_out_candidate) {
      candidates.push_back(i);
    }
  }
  mt19937 rng;
  while (true) {
    shuffle(candidates.begin(), candidates.end(), rng);
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
