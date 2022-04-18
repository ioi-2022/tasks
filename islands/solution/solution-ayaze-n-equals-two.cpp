#include "islands.h"

#include <bits/stdc++.h>
using namespace std;

std::variant<bool, std::vector<int>> find_journey(
  int N, int M, std::vector<int> U, std::vector<int> V) {
  
  vector<vector<int>> out(N), in(N);
  for (int i = 0 ; i < M ; i++) {
    if (U[i] == 0) out[V[i]].push_back(i);
    if (V[i] == 0) in[U[i]].push_back(i);
  }

  for (int i = 0 ; i < N ; i++) {
    if (static_cast<int>(out[i].size()) >= 2 && static_cast<int>(in[i].size()) >= 1) {
      vector<int> ret = {
        out[i][0], in[i][0], out[i][1], out[i][0], in[i][0], out[i][1] 
      };

      return ret;
    }
  }

  return false;
}
