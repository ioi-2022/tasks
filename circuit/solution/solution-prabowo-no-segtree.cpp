// O(N + QM)
// Note: this solution can pass quadratic subtask because of low constant

#pragma GCC optimize("Ofast,unroll-loops")

#include "circuit.h"

#include <functional>
#include <utility>
#include <vector>

const int kMod = 1000002022;

int N;
std::vector<int> A;
std::vector<int> contribution;
long long total;

void init(int _N, int M, std::vector<int> P, std::vector<int> _A) {
  N = _N; A = _A;
  std::vector<std::vector<int>> adj(N + M);
  for (int i = 1; i < N + M; ++i) {
    adj[P[i]].push_back(i);
  }

  std::vector<int> prod(N + M);
  std::function<int(int)> dfs_prod = [&](int u) {
    if (u >= N) {
      return prod[u] = 1;
    }
    prod[u] = static_cast<int>(adj[u].size());
    for (int v : adj[u]) {
      prod[u] = 1LL * prod[u] * dfs_prod(v) % kMod;
    }
    return prod[u];
  };
  dfs_prod(0);

  contribution.resize(M);
  std::function<void(int, int)> dfs_contrib = [&](int u, int product) {
    if (u >= N) {
      contribution[u - N] = product;
      return;
    }
    std::vector<int> prefix(adj[u].size()), suffix(adj[u].size());
    for (int i = 0; i < static_cast<int>(adj[u].size()); ++i) {
      prefix[i] = prod[adj[u][i]];
      if (i > 0) {
        prefix[i] = 1LL * prefix[i] * prefix[i - 1] % kMod;
      }
    }
    for (int i = static_cast<int>(adj[u].size()) - 1; i >= 0; --i) {
      suffix[i] = prod[adj[u][i]];
      if (i + 1 < static_cast<int>(adj[u].size())) {
        suffix[i] = 1LL * suffix[i] * suffix[i + 1] % kMod;
      }
    }
    for (int i = 0; i < static_cast<int>(adj[u].size()); ++i) {
      int next_product = product;
      if (i > 0) {
        next_product = 1LL * next_product * prefix[i - 1] % kMod;
      }
      if (i + 1 < static_cast<int>(adj[u].size())) {
        next_product = 1LL * next_product * suffix[i + 1] % kMod;
      }
      dfs_contrib(adj[u][i], next_product);
    }
  };
  dfs_contrib(0, 1);

  total = 0;
  for (int j = 0; j < M; ++j) {
    total += contribution[j] * A[j];
  }
}

int count_ways(int L, int R) {
  L -= N; R -= N;
  for (int j = L; j <= R; ++j) {
    total += (2 * (A[j] ^= 1) - 1) * contribution[j];
  }
  return total % kMod;
}
