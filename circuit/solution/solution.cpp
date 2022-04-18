// O(N + M + Q log M)

#include "circuit.h"

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

  std::vector<int> contribution(M);
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
