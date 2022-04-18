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

  vector<int> path = {0};
  for (int i = 1; i < N; ++i) {
    if (_check_road(i, path[0])) {
      path.insert(path.begin(), i);
    } else {
      int L = 0;
      int R = path.size() - 1;
      int res = 0;
      while (L <= R) {
        int M = (L + R) >> 1;
        if (_check_road(path[M], i)) {
          L = M + 1;
          res = M;
        } else {
          R = M - 1;
        }
      }
      path.insert(path.begin() + res + 1, i);
    }
  }
  
  while (path.size() > 3 && _check_road(path[0], path.back())) {
    path.erase(path.begin());
  }

  vector<vector<bool>> adj(N, vector<bool>(N, false));
  for (int i = 0; i < static_cast<int>(path.size()); ++i) {
    adj[path[i]][path[(i + 1) % path.size()]] = 1;
    adj[path[(i + 1) % path.size()]][path[i]] = 1;
  }

  while (path.size() > 3) {
    for (int i = 1; i < static_cast<int>(path.size()); ++i) {
      if (!adj[path[0]][path[i]]) {
        if (_check_road(path[0], path[i])) {
          path.erase(path.begin());
        } else {
          path.erase(path.begin() + i);
        }
        break;
      }
    }
  }

  for (int u : path) {
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
