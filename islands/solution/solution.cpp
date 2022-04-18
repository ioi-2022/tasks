#include "islands.h"

#include <bits/stdc++.h>

#define edge std::pair<int,int>


int mode[111000];

std::set<edge> back_path[222000];
std::set<edge> path[222000];
std::queue<int> remove_q;

bool must_flip = false;

int path_1_loop, path_2_loop;


void put_path(std::vector<int> &sol, std::vector<int> &mirror_sol, std::vector<std::pair<int, int>> &path_1, int loop_id) {
  bool start = false;
  for (int i = 1; i < (int) path_1.size(); i++) {
      int u = path_1[i - 1].first;
      int v = path_1[i].first;

      if ((!start && u == loop_id) || (start && v == loop_id)) {
        start = 1;
        mirror_sol.push_back(1);
      }
      else
        mirror_sol.push_back(0);

      if (path_1[i].second == 1)
        sol.push_back(path[u].begin()->first);
      else
        sol.push_back(path[v].begin()->first);  
    }
}

// remove a node X
void remove_node(int X) {
  // remove node 'curr', and all paths from/to them
    for (auto e: back_path[X]) {
      auto e_id = e.first;
      auto v = e.second;
      path[v].erase({e_id, X});
      if ((int) path[v].size() == 0) remove_q.push(v);
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
    if ((int) path[i].size() == 0) remove_q.push(i);
  
  int START = 0;
  std::vector<int> backtrack;
  std::vector<int> init_sol;
  std::vector<int> back_sol;

  while (true) {
    clear_unused_nodes();
    // if START is removed, then no solution
    if ((int) path[START].size() == 0) return false;

    // if START outdeg is 1, move forward and start from there.



    if ((int) path[START].size() == 1) {
      backtrack.push_back(START);
      auto prev_start = START;
      init_sol.push_back(path[START].begin()->first);
      back_sol.push_back(path[START].begin()->first);

      START = path[START].begin()->second;
      
      remove_node(prev_start);
      continue;
    }

 
    // now, we start at START, which has >= 2 outdeg.
    // find a solution.

    // step 1: find a loop
    bool seen[111000];
    memset(seen, 0, sizeof(seen));

    // path notation: second denotes direction
    std::vector<std::pair<int, int>> path_1;
    std::vector<int> first_loop;


    int curr = START;

    while (true) {
      path_1.push_back({curr, 1});
      mode[curr] = 1;
      // loop found
      if (seen[curr]) {
        path_1_loop = curr;
        int X = path_1.size();
        
        bool reverse_mode = false;
        for (int i = X - 1; i >= 0; i--) {
          if (reverse_mode) {
            path_1.push_back({path_1[i].first, 0});
          }
          else {
            // the loop
            first_loop.push_back(path_1[i].first);
            mode[path_1[i].first] = 2;
          }
          if (i != X - 1 && path_1[i].first == curr) reverse_mode = true;
        }

        break;
      } 

      seen[curr] = 1;
      int next = path[curr].begin()->second;  /// guaranteed exists
      curr = next;
    }

    // step 2: find another loop
    // path notation: second denotes direction
    std::vector<std::pair<int, int>> path_2;

    curr = path[START].rbegin()->second;

    while (true) {

      path_2_loop = curr;
      path_2.push_back({curr, 1});
      if (seen[curr]) {
        // 3 cases:
        // 1st cases, found a straight path from path_1.
        if (mode[curr] == 1) {
          int curr2 = path[curr].begin()->second;

          while (mode[curr2] == 1) {
            path_2.push_back({curr2, 1});
            curr2 = path[curr2].begin()->second;
          }
          //backloop
          path_2.push_back({first_loop[0], 1});
          for (int i = 1; i < (int) first_loop.size(); i++) {
            path_2.push_back({first_loop[i], 0});
          }
          // set for trackback
          curr = first_loop[0];
        }

        else if (mode[curr] == 2) {
          // go back reverse circle
          // find the starting point
          int st = 0;
          for (int i = 0; i < (int) first_loop.size(); i++) {
            if (first_loop[i] == curr) {
              st = i + 1;
              break;
            }
          }

          // from st to end
          for (int i = st; i < (int) first_loop.size(); i++) {
            path_2.push_back({first_loop[i], 0});
          }
          // from begin to st
          for (int i = 1; i < st; i++) {
              path_2.push_back({first_loop[i], 0});
          }
        } else if (mode[curr] == 3) {
          must_flip = true;
        }

        int X = path_2.size();
        
        bool reverse_mode = false;
        for (int i = X - 1; i >= 0; i--) {
          if (reverse_mode) {
            path_2.push_back({path_2[i].first, 0});
          }
          if (i != X - 1 && path_2[i].first == curr) reverse_mode = true;
        }
        break;
      }
      mode[curr] = 3;
      seen[curr] = 1;
      curr = path[curr].begin()->second;
    }

    // has a solution
    // convert paths to edges
    std::vector<int> sol;
    std::vector<int> mirror_sol;


    put_path(sol, mirror_sol, path_1, path_1_loop);
    mirror_sol.push_back(0);
    sol.push_back(path[START].rbegin()->first);

    put_path(sol, mirror_sol, path_2, path_2_loop);
    
    mirror_sol.push_back(0);
    sol.push_back(path[START].rbegin()->first);

    
    if (must_flip) {
      int X = sol.size();
      std::stack<int> stack_path;
      bool stack_fill_mode = 0;

      for (int i = 0; i < X; i++) {
        while (!stack_fill_mode && stack_path.size()) {
          int tmp = stack_path.top();
          stack_path.pop();
          sol.push_back(tmp);
        }
        if (mirror_sol[i] == 1) {
          stack_fill_mode ^= 1;
        }

        if (stack_fill_mode)
          stack_path.push(sol[i]);
        else
          sol.push_back(sol[i]);
      }
    }


    for (auto x: sol){
      init_sol.push_back(x);
    }

    std::reverse(back_sol.begin(), back_sol.end());
    for (auto x: back_sol){
      init_sol.push_back(x);
    }

    return init_sol;
  }
}
