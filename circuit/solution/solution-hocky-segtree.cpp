#include "circuit.h"

#include <bits/stdc++.h>
using namespace std;

const int kMod = (int) 1e9 + 2022;

struct Node {
  int left_bound, right_bound;
  int memo[2] = {0, 0};
};

struct Tree {
  int n;
  vector<Node> segment_tree;
  vector<int> lazy;
  vector<int> is_on;

  Tree(int m_init, vector<int> on_init) :
    n(m_init),
    segment_tree(2 * n - 1),
    lazy (2 * n - 1),
    is_on(on_init) {
    get_bounds(0);
    build(0);
  }

  bool is_leaf(int pos) {
    return (pos >= n - 1);
  }

  void get_bounds(int pos) {
    assert(pos < 2 * n - 1);
    if (is_leaf(pos)) {
      assert(pos - (n - 1) <= n);
      segment_tree[pos].left_bound = segment_tree[pos].right_bound = pos - (n - 1);
      return;
    }

    get_bounds(pos * 2 + 1);
    get_bounds(pos * 2 + 2);

    segment_tree[pos].left_bound = min(segment_tree[pos * 2 + 1].left_bound, segment_tree[pos * 2 + 2].left_bound);
    segment_tree[pos].right_bound = max(segment_tree[pos * 2 + 1].right_bound, segment_tree[pos * 2 + 2].right_bound);

    if(segment_tree[pos].left_bound > segment_tree[pos].right_bound)
      swap(segment_tree[pos].left_bound, segment_tree[pos].right_bound);
    
    assert(segment_tree[pos].left_bound < segment_tree[pos].right_bound);
  }

  void fix_segment_tree(int index) {

    // Fix segment tree
    vector<int> knapsack(3);
    knapsack[0] = 1LL * get_result(index * 2 + 1, 0) * get_result(index * 2 + 2, 0) % kMod;
    knapsack[1] = 1LL * get_result(index * 2 + 1, 1) * get_result(index * 2 + 2, 0) % kMod +
                  1LL * get_result(index * 2 + 1, 0) * get_result(index * 2 + 2, 1) % kMod;
    knapsack[1] %= kMod;
    knapsack[2] = 1LL * get_result(index * 2 + 1, 1) * get_result(index * 2 + 2, 1) % kMod;

    segment_tree[index].memo[0] = segment_tree[index].memo[1] = knapsack[1];
    segment_tree[index].memo[0] = (segment_tree[index].memo[0] + knapsack[0] * 2 % kMod) % kMod;
    segment_tree[index].memo[1] = (segment_tree[index].memo[1] + knapsack[2] * 2 % kMod) % kMod;

  }

  void build(int index) {
    int left_bound = segment_tree[index].left_bound;
    int right_bound = segment_tree[index].right_bound;
    if (is_leaf(index)) {
      segment_tree[index].memo[is_on[left_bound]] = 1;
      assert(left_bound == right_bound);
      return;
    }
    build(index * 2 + 1);
    build(index * 2 + 2);
    fix_segment_tree(index);
  }

  int get_result(int index, bool intend) {
    return segment_tree[index].memo[intend];
  }

  void lazy_propagate(int index) {
    if (lazy[index] == 0) return;
    lazy[index] = 0;
    swap(segment_tree[index].memo[0], segment_tree[index].memo[1]);
    if (!is_leaf(index)) {
      lazy[index * 2 + 1] ^= 1;
      lazy[index * 2 + 2] ^= 1;
    }
  }

  int toggle(int left_target, int right_target) {
    toggle(0, left_target - (n - 1), right_target - (n - 1));
    return get_result(0, 1);
  }

  void toggle(int index,
              int left_target, int right_target) {
    lazy_propagate(index);
    int left_bound = segment_tree[index].left_bound;
    int right_bound = segment_tree[index].right_bound;
    if (left_target > right_bound || right_target < left_bound) return;
    if (left_target <= left_bound && right_bound <= right_target) {
      // Toggle this tree, enable lazy;
      lazy[index] ^= 1;
      lazy_propagate(index);
      return;
    }
    toggle(index * 2 + 1, left_target, right_target);
    toggle(index * 2 + 2, left_target, right_target);
    fix_segment_tree(index);
    return;
  }
};

Tree *solution;

void init(int N, int M, vector<int>, vector<int> A) {
  assert(N == M - 1);
  solution = new Tree(M, A);
}

int count_ways(int L, int R) {
  return solution->toggle(L, R);
}
