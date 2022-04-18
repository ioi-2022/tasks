#include "towns.h"

#include <cassert>
#include <functional>
#include <vector>

int find_town(int N) {
  std::vector adj(N, std::vector(N, -1));
  std::function<int(int, int)> _check_road = [&](int u, int v) {
    if (adj[u][v] != -1) return adj[u][v];
    adj[u][v] = check_road(u, v);
    adj[v][u] = 1 - adj[u][v];
    return adj[u][v];
  };

  // Find a single node with unknown outgoing, but others has at least one
  int noOut = 0;
  for (int u = 1; u < N; ++u) {
    if (_check_road(noOut, u)) {
      noOut = u;
    }
  }

  // List all nodes with one known outgoing
  int deg = 0;
  std::vector<int> candidates;
  for (int u = 0; u < N; ++u) {
    if (u == noOut) continue;
    if (adj[noOut][u] != -1 || deg > 1) {
      candidates.push_back(u);
      continue;
    }
    if (_check_road(noOut, u)) {
      candidates.push_back(u);
      ++deg;
    }
  }

  if (deg <= 1) return noOut;

  while (candidates.size() > 2) {
    bool found = false;
    // Find any pair with unknown edge between them
    for (int i = 0; i < static_cast<int>(candidates.size()); ++i) {
      for (int j = i + 1; j < static_cast<int>(candidates.size()); ++j) {
        int u = candidates[i], v = candidates[j];
        if (adj[u][v] != -1) continue;
        found = true;
        if (_check_road(u, v)) {
          candidates[i] = candidates.back();
        } else {
          candidates[j] = candidates.back();
        }
        candidates.pop_back();
      }
      if (found) break;
    }
    assert(found);
  }

  assert(candidates.size() <= 2);
  for (int u : candidates) {
    deg = 0;
    for (int v = 0; v < N; ++v) {
      if (u != v) deg += _check_road(u, v);
    }
    if (deg <= 1) return u;
  }
  return -1;
}
