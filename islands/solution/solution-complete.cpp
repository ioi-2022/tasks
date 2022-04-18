#include "islands.h"

#include <bits/stdc++.h>

#define edge std::pair<int,int>

std::variant<bool, std::vector<int>> find_journey(int N, int M, std::vector<int> U, std::vector<int> V) {
  // construct the graph
  if (N <= 2){
    return false;
  }

  int edge_map[444][444];

  for (int i=0;i<M;i++) {
    edge_map[U[i]][V[i]] = i;
  }
  
  std::vector<int> sol;

  // 0 -> 1 -> 0 -> 2 -> 0
  sol.push_back(edge_map[0][1]);
  sol.push_back(edge_map[1][0]);
  sol.push_back(edge_map[0][2]);
  sol.push_back(edge_map[2][0]);

  // return
  sol.push_back(edge_map[1][0]);
  sol.push_back(edge_map[0][1]);
  sol.push_back(edge_map[2][0]);
  sol.push_back(edge_map[0][2]);

  return sol;
}
