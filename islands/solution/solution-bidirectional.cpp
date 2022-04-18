#include "islands.h"

#include <bits/stdc++.h>

#define edge std::pair<int,int>

std::variant<bool, std::vector<int>> find_journey(int, int M, std::vector<int> U, std::vector<int> V) {
  // adj vector
  std::vector<edge> path[1111], path_rev[1111];
  for (int i = 0; i < M; i++){
    path[U[i]].push_back({V[i], i});
    path_rev[V[i]].push_back({U[i], i});
  }

  // move forward until we find a node with > 1 new outdegree (not including the one we come from)
  int curr_pos = 0;
  int prev = -1;
  std::vector<int> sol, tmp;

  while (true) {
    if (curr_pos == 0 && path[curr_pos].size() == 0) // dead-end
      return false;
    else if (curr_pos != 0 && path[curr_pos].size() == 1) // dead-end
      return false;
    // we're at the 0, and and only have 1 path forward
    else if (curr_pos == 0 && path[curr_pos].size() == 1) {
      int idx = 0;

      prev = curr_pos;
      tmp.push_back(path[curr_pos][idx].second);
      curr_pos = path[curr_pos][idx].first;
    }
    // we're somewhere in the middle, and only have 1 new path forward
    else if (curr_pos != 0 && path[curr_pos].size() == 2) {
      int idx = 0;
      // make sure we're not going back again
      if (path[curr_pos][idx].first == prev) idx = 1;

      prev = curr_pos;
      tmp.push_back(path[curr_pos][idx].second);
      curr_pos = path[curr_pos][idx].first;

    }
    else {
      // branch found.
      // to to left, then right, then do that again.
      int Lidx, Ridx;

      // find the left and the right onen.
      if (path[curr_pos][0].first == prev) {
        Lidx = 1;
        Ridx = 2;
      } else if (path[curr_pos][1].first == prev) {
        Lidx = 0;
        Ridx = 2;
      } else {
        Lidx = 0;
        Ridx = 1;
      }
      int L = path[curr_pos][Lidx].second;
      int R = path[curr_pos][Ridx].second;

      int toL = path[curr_pos][Lidx].first;
      int toR = path[curr_pos][Ridx].first;
      
      // find an edge back
      int Lrev, Rrev;
      for (auto e: path[toL]) {
        if (e.first == curr_pos) Lrev = e.second;
      }
      for (auto e: path[toR]) {
        if (e.first == curr_pos && e.second != Lrev) Rrev = e.second;
      }

      // <path from 0 to X> - L - Lrev - R - Rrev - Lrev - L Rrev - R - <back to 0>
      int TS = tmp.size();
      for (int i = 0; i < TS; i++)
        sol.push_back(tmp[i]);

      sol.push_back(L);
      sol.push_back(Lrev);
      sol.push_back(R);
      sol.push_back(Rrev);

      sol.push_back(Lrev);
      sol.push_back(L);
      sol.push_back(Rrev);
      sol.push_back(R);
      
      for (int i = TS - 1; i >= 0; i--)
        sol.push_back(tmp[i]);

      return sol;
    }

  }
  return false;
}
