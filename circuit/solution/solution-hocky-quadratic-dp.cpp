#include "circuit.h"

#include <bits/stdc++.h>
using namespace std;

const int kMod = (int) 1e9 + 2022;

struct Tree {
  int n, m;
  vector<int> parent, is_on;
  vector<vector<int>> edge;
  vector<int> memo[2];

  Tree(int n_init, int m_init, vector<int> parent_init, vector<int> on_init) :
    n(n_init),
    m(m_init),
    parent(parent_init),
    is_on(on_init),
    edge(n + m) {

    reverse(is_on.begin(), is_on.end());
    is_on.resize(n + m);
    reverse(is_on.begin(), is_on.end());

    for (int i = 1; i < n + m; i++) {
      edge[parent[i]].push_back(i);
    }

    memo[0].resize(n + m, -1);
    memo[1].resize(n + m, -1);

  }

  bool is_leaf(int pos) {
    return pos >= n;
  }

  void toggle(pair<int, int> toggle_bounds) {
    for (int i = toggle_bounds.first; i <= toggle_bounds.second; i++) {
      is_on[i] ^= 1;
    }
  }

  void reset_memo() {
    memo[0].assign(n + m, 0);
    memo[1].assign(n + m, 0);
  }

  int dp(int pos, int intend) {
    return memo[intend][pos];
  }

  void dp(int pos) {

    if (is_leaf(pos)) {
      memo[is_on[pos]][pos] = 1;
      memo[!is_on[pos]][pos] = 0;
      return;
    }

    for (const auto &nx : edge[pos]) dp(nx);

    int child_size = edge[pos].size();
    vector<int> knapsack(1, 1);


    for (const auto &nx : edge[pos]) {

      int current_size = knapsack.size();
      knapsack.push_back(0);


      for (int i = current_size; i >= 0; i--) {
        knapsack[i] = 1LL * knapsack[i] * dp(nx, 0) % kMod;
        if(i - 1 >= 0) knapsack[i] += 1LL * knapsack[i - 1] * dp(nx, 1) % kMod;
        knapsack[i] %= kMod;
      }
    }


    for (int i = 1; i <= child_size; i++) knapsack[i] = (knapsack[i] + knapsack[i - 1]) % kMod;

    auto _get_sum = [&](int l, int r) -> int {
      int return_value = knapsack[r] + kMod;
      if (l > 0) return_value -= knapsack[l - 1];
      if (return_value >= kMod) return_value -= kMod;
      return return_value;
    };

    for (int i = 1; i <= child_size; i++) {
      memo[1][pos] = (memo[1][pos] + _get_sum(i, child_size)) % kMod;
      memo[0][pos] = (memo[0][pos] + _get_sum(0, i - 1)) % kMod;
    }
  }
};

Tree *solution;

void init(int N, int M, vector<int> P, vector<int> A) {
  solution = new Tree(N, M, P, A);
}

int count_ways(int L, int R) {
  solution->reset_memo();
  solution->toggle({L, R});
  solution->dp(0);
  return solution->dp(0, 1);
}
