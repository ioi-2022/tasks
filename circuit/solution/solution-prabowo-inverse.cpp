// O(N + M log MOD + Q log M)

#include "circuit.h"

#include <functional>
#include <utility>
#include <vector>

const int kMod = 1000002022;
const int kPhiMod = kMod / 2 / 223 * 222 / 2242157 * 2242156;

int power(int x, int y) {
  int result = 1;
  for (; y > 0; y >>= 1) {
    if (y & 1) {
      result = static_cast<long long>(result) * x % kMod;
    }
    x = static_cast<long long>(x) * x % kMod;
  }
  return result;
}

struct Z {
  int c, e2, e223;
  Z(int val=1) {
    e2 = 0, e223 = 0, c = val;
    while (c % 2 == 0) {
      ++e2;
      c /= 2;
    }
    while (c % 223 == 0) {
      ++e223;
      c /= 223;
    }
  }
  Z& operator *= (const Z &rhs) {
    c = static_cast<long long>(c) * rhs.c % kMod;
    e2 += rhs.e2;
    e223 += rhs.e223;
    return *this;
  }
  friend Z operator / (Z lhs, Z rhs) {
    lhs.c = static_cast<long long>(lhs.c) * power(rhs.c, kPhiMod - 1) % kMod;
    lhs.e2 -= rhs.e2;
    lhs.e223 -= rhs.e223;
    return lhs;
  }
  int value() {
    return static_cast<long long>(c) *
        power(2, e2) % kMod * power(223, e223) % kMod;
  }
};

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

  Z product;
  for (int i = 0; i < N; ++i) {
    product *= Z(static_cast<int>(adj[i].size()));
  }

  std::vector<int> contribution(M);
  std::function<void(int, Z)> dfs = [&](int u, Z path) {
    if (u >= N) {
      contribution[u - N] = (product / path).value();
      return;
    }
    path *= Z(static_cast<int>(adj[u].size()));
    for (int v : adj[u]) {
      dfs(v, path);
    }
  };
  dfs(0, Z());

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
