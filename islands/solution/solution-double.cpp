#include "islands.h"

#include <bits/stdc++.h>

#define edge std::pair<int,int>

int ret_paths[111000];
bool in_path[1111];
bool visited[1111];

// we use adj matrix
int path[1111][1111];
int dbl_path[1111][1111];

int path_len;

int dfs(int pos, int N, int id) {
  // std::cout<<"jalan " << pos << std::endl;
  if (visited[pos]) {
    if (in_path[pos]) {
      ret_paths[id] = pos;
      path_len = id;
      return pos;
    }
    else
      return -1;
  }
  
  ret_paths[id] = pos;

  visited[pos] = 1;
  in_path[pos] = 1;

  for (int i = 0; i < N; i++) {
    if (path[pos][i] != -1) {
      int ret = dfs(i, N, id + 1);
      if (ret != -1)
        return ret;
    }
  }

  // std::cout<<"backtrack " << pos << std::endl;
  in_path[pos] = 0;
  return -1;
}

std::variant<bool, std::vector<int>> find_journey(int N, int M, std::vector<int> U, std::vector<int> V) {
  // construct the graph

  memset(path, -1, sizeof(path));
  memset(dbl_path, -1, sizeof(dbl_path));
  memset(visited, 0, sizeof(visited));
  memset(in_path, 0, sizeof(in_path));


  for (int i = 0; i < M; i++) {
    int u = U[i];
    int v = V[i];

    if (path[u][v] == -1) path[u][v] = i;
    else dbl_path[u][v] = i;
  }

  // find a circle
  int circle_start = dfs(0, N, 0);
  // std::cout<< "CIRCLE " << circle_start << std::endl;

  if (circle_start == -1) return false;

  // buildpath
  std::vector<int> sol;
  int circle_id = -1;
  for (int i = 0; i < path_len; i++) {
    if (ret_paths[i] == circle_start) {
      circle_id = i;
      break;
    }
  }
  // 0 -> 1 -> ... -> circle -> ... -> circle
  for (int i = 0; i < path_len; i++)
    sol.push_back(path[ret_paths[i]][ret_paths[i + 1]]);

  // 2nd loop
  // circle -> ... -> circle
  for (int i = circle_id; i < path_len; i++)
    sol.push_back(dbl_path[ret_paths[i]][ret_paths[i + 1]]);

  // inverse circle 1st
  for (int i = path_len; i > circle_id; i--)
    sol.push_back(path[ret_paths[i - 1]][ret_paths[i]]);

  // inverse circle 2nd
  for (int i = path_len; i > circle_id; i--)
    sol.push_back(dbl_path[ret_paths[i - 1]][ret_paths[i]]);

  // go home
  for (int i = circle_id; i > 0; i--)
    sol.push_back(path[ret_paths[i - 1]][ret_paths[i]]);

  return sol;
}
