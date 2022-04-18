// O((N + M)^2 + Q log M)

#include "circuit.h"

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

const int kMod = 1000002022;

int N;

struct SegTree {
 private:
  int n;
  struct Node {
    int val[2];
    bool lazy;
    Node(int v0=0, int v1=0) {
      val[0] = v0;
      val[1] = v1;
      lazy = false;
    }

    Node operator + (const Node &other) {
      return Node((val[0] + other.val[0]) % kMod,
                  (val[1] + other.val[1]) % kMod);
    }
  };
  std::vector<Node> nodes;

  inline void toggle(int idx) {
    std::swap(nodes[idx].val[0], nodes[idx].val[1]);
    nodes[idx].lazy = !nodes[idx].lazy;
  }

  inline void pull(int idx, int l, int r) {
    int mid = (l + r) >> 1;
    nodes[idx] = nodes[idx + 1] + nodes[idx + (mid - l) * 2];
  }

  inline void push(int idx, int l, int r) {
    if (!nodes[idx].lazy) return;
    int mid = (l + r) >> 1;
    toggle(idx + 1);
    toggle(idx + (mid - l) * 2);
    nodes[idx].lazy = false;
  }

  void build(int idx, int l, int r, const std::vector<std::pair<int, int>> &v) {
    if (l + 1 == r) {
      nodes[idx] = Node(v[l].first, v[l].second);
      return;
    }
    int mid = (l + r) >> 1;
    build(idx + 1, l, mid, v);
    build(idx + (mid - l) * 2, mid, r, v);
    pull(idx, l, r);
  }

  void update(int idx, int l, int r, int ll, int rr) {
    if (l >= rr || r <= ll) {
      return;
    }
    if (l >= ll && r <= rr) {
      return toggle(idx);
    }
    push(idx, l, r);
    int mid = (l + r) >> 1;
    update(idx + 1, l, mid, ll, rr);
    update(idx + (mid - l) * 2, mid, r, ll, rr);
    pull(idx, l, r);
  }

 public:
  void init(const std::vector<std::pair<int, int>> &v) {
    n = static_cast<int>(v.size());
    nodes.resize(2 * n - 1);
    build(0, 0, n, v);
  }

  void update(int l, int r) {
    update(0, 0, n, l, r);
  }

  int query() {
    return nodes[0].val[1];
  }
} segtree;

void init(int _N, int M, std::vector<int> P, std::vector<int> A) {
  N = _N;
  std::vector<std::vector<int>> adj(N + M);
  for (int i = 1; i < N + M; ++i) {
    adj[P[i]].push_back(i);
  }

  std::vector<bool> path(N);
  std::vector<int> contribution(M, 1);
  std::function<void(int)> dfs = [&](int u) {
    if (u >= N) {
      for (int i = 0; i < N; ++i) {
        if (path[i]) continue;
        contribution[u - N] = 1LL * contribution[u - N] * adj[i].size() % kMod;
      }
      return;
    }
    path[u] = true;
    for (int v : adj[u]) {
      dfs(v);
    }
    path[u] = false;
  };
  dfs(0);

  std::vector<std::pair<int, int>> v(M);
  for (int i = 0; i < M; ++i) {
    if (A[i]) {
      v[i].second = contribution[i];
    } else {
      v[i].first = contribution[i];
    }
  }
  segtree.init(v);
}

int count_ways(int L, int R) {
  segtree.update(L - N, R - N + 1);
  return segtree.query();
}
