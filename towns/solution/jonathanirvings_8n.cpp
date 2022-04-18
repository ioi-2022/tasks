#include "towns.h"

#include <bits/stdc++.h>
using namespace std;

int find_town(int N) {
  vector<int> candidates(N);
  iota(candidates.begin(), candidates.end(), 0);

  mt19937 rng;
  while (candidates.size() >= 4) {
    shuffle(candidates.begin(), candidates.end(), rng);
    vector<int> nodes;
    vector<int> outdeg;
    for (int i = 0; i < 4; ++i) {
      nodes.push_back(candidates.back());
      candidates.pop_back();
      outdeg.push_back(0);
    }
    int remove = -1;
    for (int d = 1; d <= 2 && remove == -1; ++d) {
      for (int i = 0; i < 4 && remove == -1; ++i) {
        if (check_road(nodes[i], nodes[(i + d) % 4])) {
          ++outdeg[i];
        } else {
          ++outdeg[(i + d) % 4];
        }

        for (int j = 0; j < 4 && remove == -1; ++j) {
          if (outdeg[j] == 2) {
            remove = j;
          }
        }
      }
    }
    assert(remove != -1);
    for (int i = 0; i < 4; ++i) {
      if (remove != i) {
        candidates.push_back(nodes[i]);
      }
    }
  }

  for (int u : candidates) {
    int outdeg = 0;
    for (int i = 0; i < N && outdeg <= 1; ++i) {
      if (u != i && check_road(u, i)) {
        ++outdeg;
      }
    }
    if (outdeg <= 1) {
      return u;
    }
  }

  return -1;
}
