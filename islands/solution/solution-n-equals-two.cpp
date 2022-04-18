#include "islands.h"

#include <bits/stdc++.h>

std::variant<bool, std::vector<int>> find_journey(
  int, int M, std::vector<int> U, std::vector<int> V) {
  // construct the graph

  std::vector<int> to;
  std::vector<int> back;

  for (int i=0;i<M;i++) {
    if (U[i] == 0 && V[i] == 1)
      to.push_back(i);
    if (U[i] == 1 && V[i] == 0)
      back.push_back(i);
  }
  //cek
  if (to.size() >= 2 && back.size() >= 1) {
    return std::vector<int>({ to[0], back[0], to[1], to[0], back[0], to[1]});
  }
  else 
    return false;
}
