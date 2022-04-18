#include "islands.h"

#include <bits/stdc++.h>

#define edge std::pair<int,int>

#define DEBUG false

int mode[111000];

int next_s[111000];
int prev_s[111000];

bool is_removed[222000];
std::set<edge> back_path[222000];
std::set<edge> path[222000];
std::queue<int> remove_q;

// remove a node X
void remove_node(int X) {
  // remove node 'curr', and all paths from/to them
    for (auto e: back_path[X]) {
      auto e_id = e.first;
      auto v = e.second;
      path[v].erase({e_id, X});
      
      // forgot to do this
      // if (path[v].size() == 0) remove_q.push(v);
    }

    for (auto e: path[X]) {
      auto e_id = e.first;
      auto v = e.second;
      back_path[v].erase({e_id, X});
    } 
}

// chain-remove all 0 outdegrees
void clear_unused_nodes() {
  while (remove_q.size()) {
    int curr = remove_q.front();
    remove_q.pop();

    remove_node(curr);  
  }
}

std::variant<bool, std::vector<int>> find_journey(int N, int M, std::vector<int> U, std::vector<int> V) {
  // construct the graph
  for (int i = 0; i < M; i++) {
    path[U[i]].insert({i, V[i]});
    back_path[V[i]].insert({i, U[i]});
  }

  for (int i = 0; i < N; i++)
    if (path[i].size() == 0) remove_q.push(i);
  
  int START = 0;
  std::vector<int> backtrack;
  std::vector<int> init_sol;
  std::vector<int> back_sol;

  while (true) {
    clear_unused_nodes();
    // if START is removed, then no solution
    if (path[START].size() == 0) return false;

    // if START outdeg is 1, move forward and start from there.

    if (path[START].size() == 1) {
      backtrack.push_back(START);
      auto prev_start = START;
      init_sol.push_back(path[START].begin()->first);
      back_sol.push_back(path[START].begin()->first);

      START = path[START].begin()->second;
      
      remove_node(prev_start);
      continue;
    }

    return true;
  }
}
