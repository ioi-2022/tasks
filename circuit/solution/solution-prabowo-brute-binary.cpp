// O(Q(N + M))

#include "circuit.h"

#include <algorithm>
#include <vector>

const int kMod = 1000002022;

std::vector<std::vector<int>> adj;
std::vector<int> dp0, dp1;

void dfs(int u) {
  if (adj[u].empty()) {
    return;
  }

  for (int v : adj[u]) {
    dfs(v);
  }

  int l = adj[u][0], r = adj[u][1];
  int diff = (1LL * dp0[l] * dp1[r] + 1LL * dp1[l] * dp0[r]) % kMod;
  dp0[u] = (2LL * dp0[l] * dp0[r] + diff) % kMod;
  dp1[u] = (2LL * dp1[l] * dp1[r] + diff) % kMod;
}

void init(int N, int M, std::vector<int> P, std::vector<int> A) {
  adj.resize(N + M);
  for (int i = 1; i < N + M; ++i) {
    adj[P[i]].push_back(i);
  }

  dp0.resize(N + M);
  dp1.resize(N + M);

  for (int i = N; i < N + M; ++i) {
    if (A[i - N]) {
      dp1[i] = 1;
    } else {
      dp0[i] = 1;
    }
  }
}

int count_ways(int L, int R) {
  for (int i = L; i <= R; ++i) {
    std::swap(dp0[i], dp1[i]);
  }
  dfs(0);
  return dp1[0];
}
