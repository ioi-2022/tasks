#include "hoax.h"

#include <bits/stdc++.h>
using namespace std;

int n, s, q;
vector<vector<pair<int, int>>> onlines;
vector<int> queries;

vector<pair<int, int>> timelineOfEvent;
void PrepareSolver() {
  timelineOfEvent.clear();

  for (int user = 0; user < n; ++user) {
    for (pair<int, int> online: onlines[user]) {
      timelineOfEvent.emplace_back(online.first, -user - 1);
      timelineOfEvent.emplace_back(online.second, user + 1);
    }
  }

  sort(timelineOfEvent.begin(), timelineOfEvent.end());
}

namespace DSU {
  vector<int> par;
  vector<int> size;

  void init(int n) {
    par = size = vector<int>(n);

    for (int i = 0; i < n; ++i) {
      par[i] = i;
      size[i] = 1;
    }
  }

  int getRoot(int u) {
    if (u != par[u]) return par[u] = getRoot(par[u]);
    return u;
  }

  void merge(int u, int v) {
    if (getRoot(u) == getRoot(v)) return;
    size[getRoot(v)] += size[getRoot(u)];
    par[getRoot(u)] = getRoot(v);
  }

  int getSize(int u) {
    return size[getRoot(u)];
  }
};

int CountHoaxesWithInitialUser(int initialUser) {
  DSU::init(n);

  int onlineUserCount = 0;
  vector<int> connectedUsers;

  for (pair<int, int> event: timelineOfEvent) {
    int user = abs(event.second) - 1;

    // user become online
    if (event.second < 0) {
      ++onlineUserCount;
      connectedUsers.push_back(user);
    }

    // user become offline
    if (event.second > 0) {
      --onlineUserCount;

      if (onlineUserCount == 0) {
        for (int i = 1; i < connectedUsers.size(); ++i) {
          DSU::merge(connectedUsers[0], connectedUsers[i]);
        }
        connectedUsers.clear();
      }
    }
  }

  return DSU::getSize(initialUser);
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
