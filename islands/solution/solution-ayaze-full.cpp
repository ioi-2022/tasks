#include "islands.h"

#include <bits/stdc++.h>
using namespace std;

vector<set<pair<int, int>>> adj_list, rev_adj_list;
queue<int> unprocessed_zero_outdeg;

void RemoveNode(int v) {
  for (auto [u, edge_idx] : adj_list[v]) {
    rev_adj_list[u].erase({v, edge_idx});
  }

  for (auto [u, edge_idx] : rev_adj_list[v]) {
    adj_list[u].erase({v, edge_idx});

    if (adj_list[u].empty()) {
      unprocessed_zero_outdeg.push(u);
    }
  }

  adj_list[v].clear();
  rev_adj_list[v].clear();
}

// nodes, edge idxs
// last node is the first cyclic node
pair<vector<int>, vector<int>> FindCycle(int u, bool take_second) {
  auto it = adj_list[u].begin();
  if (take_second) ++it;

  set<int> seen = {u};
  vector<int> nodes = {u};
  vector<int> edge_idxs = {};

  auto [v, edge_idx] = *it;
  while (!seen.count(v)) {
    nodes.push_back(v);
    edge_idxs.push_back(edge_idx);
    seen.insert(v);

    edge_idx = adj_list[v].begin()->second;
    v = adj_list[v].begin()->first;
  }

  nodes.push_back(v);
  edge_idxs.push_back(edge_idx);
  return {nodes, edge_idxs};
}

pair<vector<int>, vector<int>> ConstructSimpleCycle(vector<int> nodes, vector<int> edge_idxs) {
  int first_occ = -1;
  for (int i = 0 ; i+1 < static_cast<int>(nodes.size()) ; i++) {
    if (nodes[i] == nodes.back()) {
      first_occ = i;
      break;
    }
  }

  vector<int> first_path;
  for (int i = 0 ; i < static_cast<int>(edge_idxs.size()) ; i++) {
    first_path.push_back(edge_idxs[i]);
  }
  for (int i = first_occ-1 ; i >= 0 ; i--) {
    first_path.push_back(edge_idxs[i]);
  }

  vector<int> second_path;
  for (int i = 0 ; i < first_occ ; i++) {
    second_path.push_back(edge_idxs[i]);
  }
  for (int i = static_cast<int>(edge_idxs.size())-1 ; i >= 0 ; i--) {
    second_path.push_back(edge_idxs[i]);
  }

  return {first_path, second_path};
}

void FindTour(int u, vector<int> &ans) {
  while (!unprocessed_zero_outdeg.empty()) {
    int v = unprocessed_zero_outdeg.front();
    unprocessed_zero_outdeg.pop();
    RemoveNode(v);
  }

  if (adj_list[u].empty()) {
    ans.push_back(-1);
    return;
  }

  if (static_cast<int>(adj_list[u].size()) == 1) {
    auto [v, edge_idx] = *adj_list[u].begin();
    ans.push_back(edge_idx);
    RemoveNode(u);
    FindTour(v, ans);
    ans.push_back(edge_idx);
    return;
  }

  auto [first_cycle, first_cycle_edges] = FindCycle(u, false);
  auto [second_cycle, second_cycle_edges] = FindCycle(u, true);

  map<int, int> first_occ;
  for (int i = 0 ; i+1 < static_cast<int>(first_cycle.size()) ; i++) {
    first_occ[first_cycle[i]] = i;
  }
  int common_node = -1;
  int first_common_occ = -1;
  int second_common_occ = -1;
  for (int i = 1 ; i+1 < static_cast<int>(second_cycle.size()) ; i++) {
    if (first_occ.count(second_cycle[i])) {
      common_node = second_cycle[i];
      second_common_occ = i;
      first_common_occ = first_occ[common_node];
      break;
    }
  }

  if (common_node == -1) { // two disjoint cycles
    auto cycle1 = ConstructSimpleCycle(first_cycle, first_cycle_edges);
    auto cycle2 = ConstructSimpleCycle(second_cycle, second_cycle_edges);

    ans.insert(ans.end(), cycle1.first.begin(), cycle1.first.end());
    ans.insert(ans.end(), cycle2.first.begin(), cycle2.first.end());
    ans.insert(ans.end(), cycle1.second.begin(), cycle1.second.end());
    ans.insert(ans.end(), cycle2.second.begin(), cycle2.second.end());
    return;
  }

  int first_cyclic_occ = -1;
  for (int i = 0 ; i+1 < static_cast<int>(first_cycle.size()) ; i++) {
    if (first_cycle[i] == first_cycle.back()) {
      first_cyclic_occ = i;
      break;
    }
  }
  
  // loop the first path
  for (int i = 0 ; i < static_cast<int>(first_cycle_edges.size()) ; i++) {
    ans.push_back(first_cycle_edges[i]);
  }
  for (int i = first_cyclic_occ-1 ; i >= 0 ; i--) {
    ans.push_back(first_cycle_edges[i]);
  }

  // enter first path through second path
  for (int i = 0 ; i < second_common_occ ; i++) {
    ans.push_back(second_cycle_edges[i]);
  }

  // only one of the following loops will be executed; we want
  // to reach the start of the cycle
  for (int i = first_common_occ-1 ; i >= first_cyclic_occ ; i--) {
    ans.push_back(first_cycle_edges[i]);
  }
  for (int i = first_common_occ ; i < first_cyclic_occ ; i++) {
    ans.push_back(first_cycle_edges[i]);
  }

  // go back through the latter part of first path and second path
  for (int i = static_cast<int>(first_cycle_edges.size())-1 ; i >= first_common_occ ; i--) {
    ans.push_back(first_cycle_edges[i]);
  }
  for (int i = second_common_occ-1 ; i >= 0 ; i--) {
    ans.push_back(second_cycle_edges[i]);
  }
}

std::variant<bool, std::vector<int>> find_journey(
    int N, int M, std::vector<int> U, std::vector<int> V) {
  
  adj_list.resize(N);
  rev_adj_list.resize(N);
  for (int i = 0 ; i < M ; i++) {
    adj_list[U[i]].insert({V[i], i});
    rev_adj_list[V[i]].insert({U[i], i});
  }
  for (int i = 0 ; i < N ; i++) {
    if (adj_list[i].empty()) {
      unprocessed_zero_outdeg.push(i);
    }
  }

  vector<int> ret;
  FindTour(0, ret);

  for (int e : ret) {
    if (e == -1) {
      return false;
    }
  }

  return ret;
}
