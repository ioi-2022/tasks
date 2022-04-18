// O(Q(N + M)^2)

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

  std::vector<int> dp = {1};
  for (int v : adj[u]) {
    dfs(v);
    std::vector<int> prod0(dp.size()), prod1(dp.size());
    for (int i = 0; i < static_cast<int>(dp.size()); ++i) {
      prod0[i] = 1LL * dp[i] * dp0[v] % kMod;
      prod1[i] = 1LL * dp[i] * dp1[v] % kMod;
    }
    dp[0] = prod0[0];
    for (int i = 1; i < static_cast<int>(dp.size()); ++i) {
      dp[i] = (prod0[i] + prod1[i - 1]) % kMod;
    }
    dp.push_back(prod1.back());
  }

  dp0[u] = 0;
  for (int i = 0, sum = 0; i + 1 < static_cast<int>(dp.size()); ++i) {
    sum = (sum + dp[i]) % kMod;
    dp0[u] = (dp0[u] + sum) % kMod;
  }

  dp1[u] = 0;
  for (int i = static_cast<int>(dp.size()) - 1, sum = 0; i >= 1; --i) {
    sum = (sum + dp[i]) % kMod;
    dp1[u] = (dp1[u] + sum) % kMod;
  }
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
