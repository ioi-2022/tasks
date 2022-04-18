#include "hoax.h"

#include <bits/stdc++.h>
using namespace std;

int n, s, q;
vector<vector<pair<int, int>>> onlines;
vector<int> queries;

vector<pair<pair<int, int>, int>> sortedOnlineTime;
void PrepareSolver() {
  for (int user = 0; user < n; ++user) {
    for (pair<int, int> online: onlines[user]) {
      sortedOnlineTime.push_back({online, user});
    }
  }

  sort(sortedOnlineTime.begin(), sortedOnlineTime.end());
}

int CountHoaxesWithInitialUser(int initialUser) {
  vector<bool> userHasHoax(n, false);
  userHasHoax[initialUser] = true;

  for (int day = 0; day < n; ++day) {
    for (int i = 0; i < sortedOnlineTime.size(); ++i) {
      pair<int, int> online = sortedOnlineTime[i].first;
      int user = sortedOnlineTime[i].second;
      if (!userHasHoax[user]) continue;

      for (int j = 0; j < sortedOnlineTime.size(); ++j) {
        // Not yet intersect
        if (sortedOnlineTime[j].first.second < online.first) continue;

        // No longer intersect
        if (sortedOnlineTime[j].first.first > online.second) break;

        userHasHoax[sortedOnlineTime[j].second] = true;
      }
    }
  }

  return count(userHasHoax.begin(), userHasHoax.end(), true);
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
