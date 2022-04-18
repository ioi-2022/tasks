#include "islands.h"

#include <algorithm>
#include <functional>
#include <iterator>
#include <queue>
#include <utility>
#include <set>
#include <variant>
#include <vector>

std::variant<bool, std::vector<int>> find_journey(
    int N, int M, std::vector<int> U, std::vector<int> V) {
  std::vector<std::set<std::pair<int, int>>> adj(N), radj(N);
  for (int j = 0; j < M; ++j) {
    adj[U[j]].emplace(V[j], j);
    radj[V[j]].emplace(U[j], j);
  }

  std::queue<int> q;
  for (int i = 0; i < N; ++i) {
    if (adj[i].empty()) {
      q.push(i);
    }
  }

  auto clearQueue = [&]() {
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (auto [v, j] : radj[u]) {
        adj[v].erase({u, j});
        if (adj[v].empty()) {
          q.push(v);
        }
      }
      radj[u].clear();
    }
  };

  std::vector<int> ans, beforeCycle;
  for (int u = 0;;) {
    clearQueue();
    if (adj[u].empty()) {
      return false;
    }

    if (static_cast<int>(adj[u].size()) == 1) {
      auto [v, j] = *adj[u].begin();
      ans.push_back(j);
      beforeCycle.push_back(j);
      radj[v].erase({u, j});
      adj[u].clear();
      q.push(u);
      u = v;
      continue;
    }

    for (int i = 0; i < N; ++i) {
      if (i != u && static_cast<int>(adj[i].size()) > 1) {
        adj[i].erase(std::next(adj[i].begin()), adj[i].end());
      }
    }
    adj[u].erase(std::next(adj[u].begin(), 2), adj[u].end());

    int invertCount = 0;
    std::vector<int> inverted(M);
    int lstEdge = -1;
    do {
      auto [v, j] = *adj[u].begin();
      if (lstEdge != -1) {
        adj[U[lstEdge]].emplace(V[lstEdge], lstEdge);
      }

      ans.push_back(j);

      lstEdge = j;
      adj[U[j]].erase({V[j], j});
      std::swap(U[j], V[j]);
      if (inverted[j] ^= 1) {
        ++invertCount;
      } else {
        --invertCount;
      }

      u = v;
    } while (invertCount != 0);
    break;
  }

  while (!beforeCycle.empty()) {
    ans.push_back(beforeCycle.back());
    beforeCycle.pop_back();
  }

  return ans;
}
