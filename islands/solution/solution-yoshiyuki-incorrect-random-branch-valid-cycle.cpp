// Incorrect attempt of O(N^2) with Monte Carlo
// After remove DAG and handle 2 separate cycles case
// Pick random node with branches, search until 2 valid cycles are found
// It choose a random shortest path between 0 and the start of the cycle

// One counter to this solution is gen_tricky_3.

// This solution MUST be WA and TLE for all values of ATTEMPT.

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

const int max_n = 1e5;
const int ATTEMPT = 200;

std::vector<int> adj[max_n+10];
std::set<int> u_adj[max_n+10];

int vis[max_n+10];  // 0 = unvisited, 1 = visited, 2 = invalid cycle
bool vis_dag[max_n+10];
bool vis_candidate[max_n+10];
bool vis_path[max_n+10];

std::vector<int> has_cycle;
bool excluded[max_n+10];
std::vector<std::pair<int, int> > candidate;
int prev_path[max_n+10];

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

// Find "random" path from 0 to start of branch
void find_path(int start, int finish) {
  memset(vis_path, false, sizeof vis_path);
  memset(prev_path, -1, sizeof prev_path);
  std::queue<int> BFS;
  vis_path[start] = true;
  BFS.push(start);
  while (!BFS.empty()) {
    if (vis_path[finish]) break;
    int cur = BFS.front();
    BFS.pop();
    for (auto nex: adj[cur]) {
      if (vis_path[nex]) continue;
      assert(vis_path[nex] == false);
      vis_path[nex] = true;
      prev_path[nex] = cur;
      BFS.push(nex);
    }
  }
  int cur = finish;
  while (cur != start) {
    cur = prev_path[cur];
    vis[cur] = 2;
  }
}

void reset(int start, bool second = false) {
  memset(vis, 0, sizeof vis);
  if (second) {
    find_path(0, start);
  }
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
    std::random_shuffle(adj[i].begin(), adj[i].end());
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
  int attempt = ATTEMPT;
  for (auto [v, prev]: candidate) {
    if (v == 0) continue;
    cnt = 0;
    for (auto start: adj[v]) {
      if (start == prev) continue;
      if (start == 0) continue;
      if (vis[start] == 2) continue;
      if (excluded[start]) continue;
      attempt--;
      reset(start, true);
      vis[v] = 1;
      if (find_cycle(start, true)) {
        cnt++;
      }
      if (cnt >= 2) {
        return true;
      }
      if (attempt == 0) return false;
    }
    vis[v] = 0;
    if (attempt == 0) return false;
  }
  return false;
}
