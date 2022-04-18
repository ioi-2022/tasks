// Incorrect attempt of O(N^2) with pruning
// Counter of this solution:
// * gen_dag: WA
// * gen_tricky2: TLE

// A fix to this incorrect solution is to remove DAG,
// which lead to the lemma 1 (remove nodes with 0 out-degree)

#include "islands.h"

#include <functional>
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

std::vector<int> visited, to_start;
std::vector<int> has_cycle;

bool find_cycle(int cur, bool init = true) {
  if (!init && vis[cur] == 1) return true;
  if (vis[cur] == 2) return false;
  vis[cur] = 1;
  visited.push_back(cur);
  for (auto nex: u_adj[cur]) {
    if (find_cycle(nex, false)) return true;
  }
  return false;
}

void reset(int start) {
  for (auto u: visited) vis[u] = 0;
  visited.clear();
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

  int cnt = 0;
  for (auto start: adj[0]) {
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

  for (auto u: has_cycle) {
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
    if (adj[v].empty() || to_start.empty()) continue;
    cnt = 0;
    for (auto start: adj[v]) {
      if (start == prev) continue;
      reset(start);
      vis[v] = 1;
      cnt += find_cycle(start);
      if (cnt >= 2) return true;
    }
    vis[v] = 0;
  }
  return false;
}
