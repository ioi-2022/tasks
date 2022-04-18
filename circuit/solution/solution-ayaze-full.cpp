// O(N + M + Q log M)
#include "circuit.h"
#include <bits/stdc++.h>
using namespace std;

const int kMod = 1'000'002'022;

struct Node {
  bool lazy;
  int val[2];

  Node(int v0 = 0, int v1 = 0) {
    lazy = false;
    val[0] = v0;
    val[1] = v1;
  }

  void flip() {
    lazy = !lazy;
    swap(val[0], val[1]);
  }

  Node operator +(Node other) const {
    Node ret;
    for (int i = 0 ; i < 2 ; i++) {
      ret.val[i] = (val[i] + other.val[i]) % kMod;
    }
    return ret;
  }
};

struct SegmentTree {
  vector<Node> tree;
  int n;

  SegmentTree() {}
  SegmentTree(vector<pair<int, int>> initial_values) {
    n = initial_values.size();
    tree.resize(4 * n + 5);
    build(1, 0, n-1, initial_values);
  }

  void build(int id, int l, int r, vector<pair<int, int>> &initial_values) {
    if (l == r) {
      pair<int, int> val = initial_values[l];
      tree[id] = Node(val.first, val.second);
    } else {
      int m = (l + r) / 2;
      int chld = id << 1;

      build(chld, l, m, initial_values);
      build(chld+1, m+1, r, initial_values);
      tree[id] = tree[chld] + tree[chld+1];
    }
  }

  void propagate(int id) {
    if (!tree[id].lazy) return;
    for (int i = 0 ; i < 2 ; i++) {
      tree[2*id + i].flip();
    }
    tree[id].lazy = false;
  }

  void update(int l, int r) {
    update(1, 0, n-1, l, r);
  }

  void update(int id, int l, int r, int x, int y) {
    if (x <= l && r <= y) {
      tree[id].flip();
    } else {
      int m = (l + r) / 2;
      int chld = id << 1;

      propagate(id);
      if (x <= m) update(chld, l, m, x, y);
      if (y > m)  update(chld+1, m+1, r, x, y);
      tree[id] = tree[chld] + tree[chld+1];
    }
  }

  int query() {
    return tree[1].val[1];
  }
};

SegmentTree segment_tree;
vector<vector<int>> children;
vector<int> child_muls;
vector<int> contribs;
int N;

void DfsMuls(int v) {
  if (v >= N) {
    child_muls[v] = 1;
    return;
  }

  child_muls[v] = children[v].size();
  for (int nex : children[v]) {
    DfsMuls(nex);
    child_muls[v] = 1ll * child_muls[v] * child_muls[nex] % kMod;
  }
}

void DfsContrib(int v, int current_contrib) {
  if (v >= N) {
    contribs[v-N] = current_contrib;
    return;
  }

  vector<int> prefix_contrib(children[v].size()), suffix_contrib(children[v].size());
  for (int i = 0 ; i < static_cast<int>(children[v].size()) ; i++) {
    prefix_contrib[i] = child_muls[children[v][i]];
    if (i > 0) {
      prefix_contrib[i] = 1ll * prefix_contrib[i] * prefix_contrib[i-1] % kMod;
    }
  }
  for (int i = static_cast<int>(children[v].size())-1 ; i >= 0 ; i--) {
    suffix_contrib[i] = child_muls[children[v][i]];
    if (i+1 < static_cast<int>(children[v].size())) { 
      suffix_contrib[i] = 1ll * suffix_contrib[i] * suffix_contrib[i+1] % kMod;
    }
  }

  for (int i = 0 ; i < static_cast<int>(children[v].size()) ; i++) {
    int nex = children[v][i];
    int new_contrib = current_contrib;
    if (i > 0) {
      new_contrib = 1ll * new_contrib * prefix_contrib[i-1] % kMod;
    }
    if (i+1 < static_cast<int>(children[v].size())) { 
      new_contrib = 1ll * new_contrib * suffix_contrib[i+1] % kMod;
    }

    DfsContrib(nex, new_contrib);
  }
}

void init(int _N, int M, std::vector<int> P, std::vector<int> A) {
  N = _N;
  children.resize(N);
  contribs.resize(M);
  child_muls.resize(N+M);

  for (int i = 1 ; i < static_cast<int>(P.size()) ; i++) {
    children[P[i]].push_back(i);
  }
  DfsMuls(0);
  DfsContrib(0, 1);

  vector<pair<int, int>> initial_values;
  for (int i = 0 ; i < M ; i++) {
    pair<int, int> c = {0, contribs[i]};
    if (A[i] == 0) swap(c.first, c.second);
    initial_values.push_back(c);
  }

  segment_tree = SegmentTree(initial_values);
}

int count_ways(int L, int R) {
  segment_tree.update(L-N, R-N);
  return segment_tree.query();
}
