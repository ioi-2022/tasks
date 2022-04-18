// Incorrect attempt of O(N) with Monte Carlo
// After remove DAG and handle 2 separate cycles case
// Pick random node with branches, search until 2 "not necessarily valid"
//                                 cycles are found starting from that branch

// Counter to this solution already exist in gen_tricky_3 and gen_circle

// This solution MUST be WA

#include "islands.h"

#include <functional>
#include <queue>
#include <utility>
#include <set>
#include <variant>
#include <vector>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std;

const int max_n = 1e5;

vector<int> adj[max_n+10];
set<int> u_adj[max_n+10];

int vis[max_n+10];  // 0 = unvisited, 1 = visited, 2 = invalid cycle
bool vis_dag[max_n+10];
bool vis_candidate[max_n+10];

vector<int> has_cycle;
bool excluded[max_n+10];
vector<pair<int, int> > candidate;
vector<int> prev_path;

// Find cycle from start of branch
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

// Erase DAG / Erase node with 0 outdegree
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

// Find candidates of node with branch
void find_candidate(int cur, int prev) {
  if (excluded[cur]) return;
  if (vis_candidate[cur]) return;
  vis_candidate[cur] = true;
  int cnt_branch = 0;
  for (auto next: adj[cur]) {
    if (next != prev && !excluded[next]) {
      cnt_branch++;
      find_candidate(next, cur);
    }
  }
  if (cnt_branch >= 2) candidate.push_back({cur, prev});
  return;
}

void reset(int start) {
  memset(vis, 0, sizeof vis);
  vis[start] = 1;
}

std::variant<bool, std::vector<int>> find_journey(
    int N, int M, std::vector<int> U, std::vector<int> V) {

  srand(time(NULL));
  memset(vis, 0, sizeof vis);
  for (int i = 0; i < M; i++) {
    adj[U[i]].push_back(V[i]);
    u_adj[U[i]].insert(V[i]);
  }

  for (int i = 0; i < N; i++) {
    random_shuffle(adj[i].begin(), adj[i].end());
  }

  erase_dag(0);

  // Find 2 cycles with separate outedge from 0
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

  // If 0 only has 1 outedge with cycle(s)
  // 2 cycles must be found here
  assert(has_cycle.size() == 1);

  find_candidate(has_cycle[0], 0);
  random_shuffle(candidate.begin(), candidate.end());

  // Find 2 cycles among candidates
  for (auto [v, prev]: candidate) {
    if (v == 0) continue;
    cnt = 0;
    for (auto start: adj[v]) {
      if (start == prev) continue;
      if (start == 0) continue;
      if (vis[start] == 2) continue;
      if (excluded[start]) continue;
      reset(start);
      vis[v] = 1;
      if (find_cycle(start, true)) {
        cnt++;
      }
      if (cnt >= 2) {
        return true;
      }
    }
    vis[v] = 0;
  }
  return false;
}
