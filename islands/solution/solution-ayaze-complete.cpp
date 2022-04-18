#include "islands.h"

#include <bits/stdc++.h>
using namespace std;

std::variant<bool, std::vector<int>> find_journey(
  int N, int M, std::vector<int> U, std::vector<int> V) {
  
  map<pair<int, int>, int> edge_to_idx;
  for (int i = 0 ; i < M ; i++) {
    edge_to_idx[{U[i], V[i]}] = i;
  }

  function<bool(int, int)> hasBidirectionalEdge = [&](int u, int v) -> bool {
    return edge_to_idx.count({u, v}) > 0 && edge_to_idx.count({v, u}) > 0;
  };
  for (int i = 1 ; i < N ; i++) {
    for (int j = i+1 ; j < N ; j++) {
      if (hasBidirectionalEdge(0, i) && hasBidirectionalEdge(0, j) && hasBidirectionalEdge(i, j)) {
        vector<int> ret = {
          edge_to_idx[{0, i}], edge_to_idx[{i, j}], edge_to_idx[{j, 0}],
          edge_to_idx[{0, j}], edge_to_idx[{j, i}], edge_to_idx[{i, 0}],
          edge_to_idx[{j, 0}], edge_to_idx[{i, j}], edge_to_idx[{0, i}],
          edge_to_idx[{i, 0}], edge_to_idx[{j, i}], edge_to_idx[{0, j}]
        };
        
        return ret;
      }
    }
  }

  return false;
}
