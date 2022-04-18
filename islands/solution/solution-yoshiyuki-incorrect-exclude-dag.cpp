// Incorrect attempt of O(N^2) with pruning and removing DAG
// Time Complexity might be O(N) (unproven) due to DAG removal
// Has incorrect cycle finding
// Counter of this solution (already generalized in some of gen_solution)
/*
6 8
0 1
1 2
2 3
3 2
2 4
4 2
1 5
5 0
*/

#include "islands.h"

#include <functional>
#include <queue>
#include <utility>
#include <set>
#include <variant>
#include <vector>
#include <cstring>
#include <cassert>

const int max_n = 1e5;

std::vector<int> adj[max_n+10];
std::set<int> u_adj[max_n+10];

int vis[max_n+10];
bool vis_one_deg[max_n+10];
bool vis_dag[max_n+10];

std::vector<int> to_start;
std::vector<int> has_cycle;
bool excluded[max_n+10];

bool find_cycle(int cur, bool init = true) {
  if (excluded[cur]) return false;
  if (!init && vis[cur] == 1) return true;
  if (vis[cur] == 2) return false;
  vis[cur] = 1;
  for (auto nex: u_adj[cur]) {
    if (find_cycle(nex, false)) return true;
  }
  return false;
}

void erase_dag(int cur) {
  if (vis_dag[cur]) return;
  vis_dag[cur] = true;
  bool has_outdegree = false;
  for (auto nex: adj[cur]) {
    erase_dag(nex);
    if (excluded[nex]) continue;
    has_outdegree = true;
  }
  if (!has_outdegree) excluded[cur] = true;
  return;
}

void reset(int start) {
  memset(vis, 0, sizeof vis);
  for (auto u: to_start) vis[u] = 2;
  vis[start] = 1;
}

std::variant<bool, std::vector<int>> find_journey(
    int N, int M, std::vector<int> U, std::vector<int> V) {

  memset(vis, 0, sizeof vis);
  for (int i = 0; i < M; i++) {
    adj[U[i]].push_back(V[i]);
    u_adj[U[i]].insert(V[i]);
  }

  for (int i = 0; i < N; i++) {
    random_shuffle(adj[i].begin(), adj[i].end());
  }

  erase_dag(0);

  int cnt = 0;
  for (auto start: adj[0]) {
    if (excluded[start]) continue;
    reset(start);
    vis[0] = 1;
    if (find_cycle(start)) {
      cnt++;
      has_cycle.push_back(start);
    }
    if (cnt >= 2) return true;
  }

  if (has_cycle.empty()) return false;
  assert(has_cycle.size() == 1);

  // Find the first node with branches
  int u = has_cycle[0];
  int v = u, prev = 0;
  to_start.clear();
  to_start.push_back(0);
  vis_one_deg[0] = true;
  while (true) {
    if (vis_one_deg[v]) return false;
    to_start.push_back(v);
    int cnt_branch = 0;
    vis_one_deg[v] = true;
    for (auto nex: adj[v]) {
      if (nex != prev) cnt_branch++;
      if (cnt_branch >= 2) break;
    }
    if (cnt_branch >= 2) break;
    if (cnt_branch != 1) return false;
    for (auto nex: adj[v]) {
      if (nex != prev) {
        prev = v;
        v = nex;
        break;
      }
    }
  }
  if (adj[v].empty() || to_start.empty()) return false;

  // 2 cycles must be found here -> incorrect assumption
  cnt = 0;
  for (auto start: adj[v]) {
    if (start == prev) continue;
    if (excluded[start]) continue;
    reset(start);
    vis[v] = 1;
    if (find_cycle(start)) {
      cnt++;
    }
    if (cnt >= 2) return true;
  }
  vis[v] = 0;
  return false;
}
