#include "islands.h"

#include <bits/stdc++.h>
using namespace std;

vector<set<pair<int, int>>> adj_list, rev_adj_list;

void RemoveNode(int v) {
  for (auto [u, edge_idx] : adj_list[v]) {
    rev_adj_list[u].erase({v, edge_idx});
  }
  for (auto [u, edge_idx] : rev_adj_list[v]) {
    adj_list[u].erase({v, edge_idx});
  }

  adj_list[v].clear();
  rev_adj_list[v].clear();
}

void FindTour(int v, vector<int> &ans) {
  if (adj_list[v].empty()) {
    ans.push_back(-1);
    return;
  }

  map<int, vector<int>> forward_edge;
  map<int, vector<int>> backward_edge;

  for (auto [u, edge_idx] : adj_list[v]) {
    if (forward_edge[u].empty()) {
      pair<int, int> identifier = {v, -1};
      auto it = adj_list[u].lower_bound(identifier);
      while (static_cast<int>(backward_edge[u].size()) < 2 && it != adj_list[u].end() && it->first == v) {
        backward_edge[u].push_back(it->second);
        it++;
      }
    }

    if (static_cast<int>(forward_edge[u].size()) < 2) {
      forward_edge[u].push_back(edge_idx);
    }
  }

  vector<int> bidirectional_ends;
  pair<int, int> ep1 = {-1, -1}, ep2 = {-1, -1};
  for (auto [u, edges] : forward_edge) {
    if (static_cast<int>(edges.size()) == 2 && static_cast<int>(backward_edge[u].size()) == 2) {
      ep1 = {edges[0], backward_edge[u][0]};
      ep2 = {edges[1], backward_edge[u][1]};
    }

    if (!backward_edge[u].empty()) {
      bidirectional_ends.push_back(u);
    }
  }

  if (static_cast<int>(bidirectional_ends.size()) > 1) {
    int end1 = bidirectional_ends[0], end2 = bidirectional_ends[1];
    ep1 = {forward_edge[end1][0], backward_edge[end1][0]};
    ep2 = {forward_edge[end2][0], backward_edge[end2][0]};
  }

  if (ep1.first != -1) {
    ans.push_back(ep1.first); ans.push_back(ep1.second);
    ans.push_back(ep2.first); ans.push_back(ep2.second);
    ans.push_back(ep1.second); ans.push_back(ep1.first);
    ans.push_back(ep2.second); ans.push_back(ep2.first);
    return;
  }

  // move to any other node, removing current node to ensure termination
  auto [u, edge_idx] = *adj_list[v].begin();
  RemoveNode(v);
  ans.push_back(edge_idx);
  FindTour(u, ans);
  ans.push_back(edge_idx);
}

std::variant<bool, std::vector<int>> find_journey(
  int N, int M, std::vector<int> U, std::vector<int> V) {
  
  adj_list.resize(N);
  rev_adj_list.resize(N);

  for (int i = 0 ; i < M ; i++) {
    adj_list[U[i]].insert({V[i], i});
    rev_adj_list[V[i]].insert({U[i], i});
  }

  vector<int> ret;
  FindTour(0, ret);
  for (int x : ret) {
    if (x == -1) {
      return false;
    }
  }

  return ret;
}
