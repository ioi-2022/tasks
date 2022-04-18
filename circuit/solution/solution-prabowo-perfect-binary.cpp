// O(N + M + Q log M)

#include "circuit.h"

#include <algorithm>
#include <utility>
#include <vector>

const int kMod = 1000002022;

int N, M;
std::vector<int> dp0, dp1;
std::vector<bool> flip;

void compute(int u) {
  int l = u * 2 + 1, r = u * 2 + 2;
  int diff = (1LL * dp0[l] * dp1[r] + 1LL * dp1[l] * dp0[r]) % kMod;
  dp0[u] = (2LL * dp0[l] * dp0[r] + diff) % kMod;
  dp1[u] = (2LL * dp1[l] * dp1[r] + diff) % kMod;
}

void init(int _N, int _M, std::vector<int>, std::vector<int> A) {
  N = _N; M = _M;
  dp0.resize(N + M);
  dp1.resize(N + M);
  flip.resize(N + M);
  for (int i = N; i < N + M; ++i) {
    if (A[i - N]) {
      dp1[i] = 1;
    } else {
      dp0[i] = 1;
    }
  }

  for (int i = N - 1; i >= 0; --i) {
    compute(i);
  }
}

void update(int node, int l, int r, int ll, int rr) {
  if (l >= rr || r <= ll) return;
  if (l >= ll && r <= rr) {
    flip[node] = !flip[node];
    std::swap(dp0[node], dp1[node]);
    return;
  }
  int lnode = node * 2 + 1;
  int rnode = node * 2 + 2;
  if (flip[node]) {
    flip[node] = false;
    std::swap(dp0[lnode], dp1[lnode]);
    flip[lnode] = !flip[lnode];
    std::swap(dp0[rnode], dp1[rnode]);
    flip[rnode] = !flip[rnode];
  }

  int mid = (l + r) / 2;
  update(lnode, l, mid, ll, rr);
  update(rnode, mid, r, ll, rr);
  compute(node);
}

void update(int l, int r) {
  update(0, 0, M, l, r);
}

int count_ways(int L, int R) {
  L -= N; R -= N;
  update(L, R + 1);
  return dp1[0];
}
