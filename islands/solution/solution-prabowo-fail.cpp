#include "islands.h"

#include <functional>
#include <queue>
#include <utility>
#include <variant>
#include <vector>

std::vector<std::vector<std::pair<int, int>>> pruneNoOutgoing(
    int N, int M, std::vector<int> &U, std::vector<int> &V) {
  std::vector<int> deg(N);
  std::vector<std::vector<std::pair<int, int>>> radj(N);
  for (int i = 0; i < M; ++i) {
    if (U[i] < 0) {
      continue;
    }
    radj[V[i]].emplace_back(U[i], i);
    ++deg[U[i]];
  }

  std::queue<int> q;
  for (int i = 0; i < N; ++i) {
    if (deg[i] == 0) {
      q.push(i);
    }
  }

  std::vector<bool> removed(N, false);
  while (!q.empty()) {
    int u = q.front();
    q.pop();

    removed[u] = true;

    for (auto [v, i] : radj[u]) {
      if (--deg[v] == 0) {
        q.push(v);
      }
    }
  }

  std::vector<std::vector<std::pair<int, int>>> adj(N);
  for (int i = 0; i < M; ++i) {
    if (U[i] < 0 || removed[U[i]] || removed[V[i]]) {
      U[i] = V[i] = -1;
    } else {
      adj[U[i]].emplace_back(V[i], i);
    }
  }

  return adj;
}

std::variant<bool, std::vector<int>> find_journey(
    int N, int M, std::vector<int> U, std::vector<int> V) {
  std::vector<std::vector<std::pair<int, int>>> adj = pruneNoOutgoing(N, M, U, V);

  std::vector<int> inverted(M);
  int invertedCnt = 0;

  std::vector<int> ans;
  int lstEdge = -1;
  std::function<int(int)> step = [&](int edge) {
    int u = U[edge], v = V[edge];

    if (lstEdge != -1) {
      adj[U[lstEdge]].emplace_back(V[lstEdge], lstEdge);
    }

    lstEdge = edge;
    for (int i = 0; i < static_cast<int>(adj[u].size()); ++i) {
      if (adj[u][i].second == edge) {
        adj[u][i] = adj[u].back();
        adj[u].pop_back();
        break;
      }
    }
    std::swap(U[lstEdge], V[lstEdge]);
    if (inverted[lstEdge] ^= 1) {
      ++invertedCnt;
    } else {
      --invertedCnt;
    }

    ans.push_back(edge);
    return v;
  };

  // The WA part is because the next section is not made into a loop
  //   I only step out of the 1-outdegree and prune once
  //   And even if it was made into a loop, the complexity would be O(N^2)
  // WA case is very easy: a bidirectional line graph of size > 3

  int u = 0;
  while (static_cast<int>(adj[u].size()) == 1) {
    u = step(adj[u][0].second);
  }
  std::vector<int> pathToTwoDeg = ans;

  for (int j : ans) {
    U[j] = V[j] = -1;
  }

  adj = pruneNoOutgoing(N, M, U, V);

  if (static_cast<int>(adj[u].size()) == 0) {
    return false;
  }

  invertedCnt = 0;
  lstEdge = -1;

  do {
    u = step(adj[u].back().second);
  } while (invertedCnt != 0);

  while (!pathToTwoDeg.empty()) {
    ans.push_back(pathToTwoDeg.back());
    pathToTwoDeg.pop_back();
  }
  return ans;
}
