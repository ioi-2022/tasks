#include "hoax.h"

#include <bits/stdc++.h>
using namespace std;

int n, s, q;
vector<vector<pair<int, int>>> onlines;
vector<int> queries;

vector<vector<int>> usersOnlineAtHour;
void PrepareSolver() {
  int totalHours = s * n;
  usersOnlineAtHour.clear();
  usersOnlineAtHour.resize(totalHours);

  for (int user = 0; user < n; ++user) {
    for (pair<int, int> online: onlines[user]) {
      for (int day = 0; day < n; ++day) {
        for (int hour = online.first; hour <= online.second; ++hour) {
          usersOnlineAtHour[day*s + hour].push_back(user);
        }
      }
    }
  }
}

int CountHoaxesWithInitialUser(int initialUser) {
  vector<bool> userHasHoax(n, false);
  userHasHoax[initialUser] = true;

  int totalHours = s * n;
  for (int hour; hour < totalHours; ++hour) {
    bool someoneHasHoaxAtThisHour = false;
    for (int user: usersOnlineAtHour[hour]) {
      if (userHasHoax[user]) {
        someoneHasHoaxAtThisHour = true;
        break;
      }
    }

    if (someoneHasHoaxAtThisHour) {
      // Spread hoaxes to everyone
      for (int user: usersOnlineAtHour[hour]) {
        userHasHoax[user] = true;
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
