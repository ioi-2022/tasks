#include "hoax.h"

#include <bits/stdc++.h>
using namespace std;

int n, s, q;
vector<vector<pair<int, int>>> onlines;
vector<int> queries;

bool isIntersect(pair<int, int> range1, pair<int, int> range2) {
  function<bool(int, pair<int, int>)> pointInRange = [](int x, pair<int, int> range) {
    return range.first <= x && x <= range.second;
  };

  return pointInRange(range1.first, range2) || pointInRange(range1.second, range2) ||
      pointInRange(range2.first, range1) || pointInRange(range2.second, range1);
}

vector<vector<int>> intersect;
void PrepareSolver() {
  intersect = vector<vector<int>>(n);
  for (int user1 = 0; user1 < n; ++user1) {
    for (pair<int, int> online1: onlines[user1]) {
      for (int user2 = user1; user2 < n; ++user2) {
        bool hasIntersect = false;
        for (pair<int, int> online2: onlines[user2]) {
          if (isIntersect(online1, online2)) {
            hasIntersect = true;
            break;
          }
        }

        if (hasIntersect) {
          intersect[user1].push_back(user2);
          intersect[user2].push_back(user1);
        }
      }
    }
  }
}

int CountHoaxesWithInitialUser(int initialUser) {
  queue<int> bfsQueue;
  vector<bool> hasHoax(n, false);

  bfsQueue.push(initialUser);
  hasHoax[initialUser] = true;

  int hoaxCount = 0;

  while (!bfsQueue.empty()) {
    int user = bfsQueue.front();
    bfsQueue.pop();

    ++hoaxCount;

    for (int intersectingUser: intersect[user]) {
      if (hasHoax[intersectingUser]) continue;
      hasHoax[intersectingUser] = true;

      bfsQueue.push(intersectingUser);
    }
  }

  return hoaxCount;
}

void init(int N, int S, std::vector<int> T,
          std::vector<std::vector<int>> A, std::vector<std::vector<int>> B) {
  n = N;
  s = S;

  onlines.clear();
  onlines.resize(n);

  for (int i = 0; i < n; ++i) {
    int o = T[i];

    for (int j = 0; j < o; ++j) {
      int a = A[i][j], b = B[i][j];
      --a, --b;

      onlines[i].emplace_back(a, b);
    }
  }
  PrepareSolver();
}

int count_users(int P) {
  return CountHoaxesWithInitialUser(P);
}
