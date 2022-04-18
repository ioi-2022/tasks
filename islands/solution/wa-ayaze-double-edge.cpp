// WA: only consider solution where we have a cycle that starts and ends in 0
#include "islands.h"

#include <bits/stdc++.h>
using namespace std;

std::variant<bool, std::vector<int>> find_journey(
  int N, int M, std::vector<int> U, std::vector<int> V) {
  
  vector<vector<pair<int, int>>> adj_list(N);
  for (int i = 0 ; i < M ; i++) {
    adj_list[U[i]].push_back({V[i], i});
  }

  vector<int> parent(N, -1);
  parent[0] = 0;

  function<void(int)> dfs = [&](int now) {
    for (auto [nex, _] : adj_list[now]) {
      if (parent[nex] == -1) {
        parent[nex] = now;
        dfs(nex);
      }
    }
  };
  dfs(0);

  vector<int> path;
  int end_point = -1;
  for (int i = 1 ; i < N ; i++) {
    if (parent[i] == -1) continue;
    for (auto [nex, _] : adj_list[i]) {
      if (nex == 0) {
        end_point = i;
      }
    }
  }
  if (end_point == -1) {
    return false;
  }

  path = {0};
  for (int v = end_point ; v != 0 ; v = parent[v]) {
    path.push_back(v);
  }
  path.push_back(0);
  reverse(path.begin(), path.end());

  vector<vector<int>> path_edges;
  for (int i = 0 ; i+1 < static_cast<int>(path.size()) ; i++) {
    int j = i+1;

    vector<int> edges;
    for (auto [v, edge_idx] : adj_list[path[i]]) {
      if (v == path[j]) {
        edges.push_back(edge_idx);
      }
    }

    path_edges.push_back(edges);
  }

  vector<int> ret;
  int path_edges_size = path_edges.size();

  for (int i = 0 ; i < path_edges_size ; i++) {
    ret.push_back(path_edges[i][0]);
  }
  for (int i = 0 ; i < path_edges_size ; i++) {
    ret.push_back(path_edges[i][1]);
  }
  for (int i = path_edges_size-1 ; i >= 0 ; i--) {
    ret.push_back(path_edges[i][0]);
  }
  for (int i = path_edges_size-1 ; i >= 0 ; i--) {
    ret.push_back(path_edges[i][1]);
  }

  return ret;
}
