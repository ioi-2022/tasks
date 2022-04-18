#include "cards.h"

#include <bits/stdc++.h>

std::vector<std::vector<int>> leftNodes, rightNodes;

namespace BPM {
int N, M;
std::vector<std::vector<int>> adj;
std::vector<int> match, matchLeft;
std::vector<bool> vis;

void init(int _N, int _M) {
  N = _N; M = _M;
  adj.clear();
  adj.resize(N);
  match.assign(M, -1);
  matchLeft.assign(N, -1);
}

void addEdge(int u, int v) {
  adj[u].push_back(v);
}

bool dfs(int u) {
  if (vis[u]) return false;
  vis[u] = true;

  for (int v : adj[u]) {
    if (match[v] == -1 || dfs(match[v])) {
      match[v] = u;
      matchLeft[u] = v;
      return true;
    }
  }
  return false;
}

int maxMatch() {
  int ret = 0;
  for (int i = 0; i < N; ++i) {
    vis.assign(N, false);
    if (dfs(i)) ++ret;
  }
  return ret;
}

}

void init(int N, int K) {
  std::function<std::vector<int>(int)> cardsFromMask = [N](int mask) {
    std::vector<int> ret;
    for (int i = 0; i < N; ++i) {
      if (mask >> i & 1) ret.push_back(i + 1);
    }
    return ret;
  };

  std::vector<int> leftMasks, rightMasks;
  for (int mask = 0; mask < 1 << N; ++mask) {
    int popcount = __builtin_popcount(mask);
    if (popcount == K) leftMasks.push_back(mask);
    else if (popcount == K - 1) rightMasks.push_back(mask);
  }

  for (int rmask : rightMasks) {
    std::vector<int> perm = cardsFromMask(rmask);
    do {
      rightNodes.push_back(perm);
    } while (next_permutation(perm.begin(), perm.end()));
  }
  std::sort(rightNodes.begin(), rightNodes.end());

  for (int lmask : leftMasks) leftNodes.push_back(cardsFromMask(lmask));
  std::sort(leftNodes.begin(), leftNodes.end());

  BPM::init(leftNodes.size(), rightNodes.size());

  for (int lmask : leftMasks) {
    int lftIdx = std::lower_bound(leftNodes.begin(), leftNodes.end(), cardsFromMask(lmask)) - leftNodes.begin();
    for (int rmask : rightMasks) {
      if ((lmask & rmask) == rmask) {
        std::vector<int> perm = cardsFromMask(rmask);
        do {
          BPM::addEdge(
            lftIdx,
            std::lower_bound(rightNodes.begin(), rightNodes.end(), perm) - rightNodes.begin()
          );
        } while (std::next_permutation(perm.begin(), perm.end()));
      }
    }
  }

  BPM::maxMatch();
}

void init_assistant(int _N, int _K) { init(_N, _K); }

void init_magician(int _N, int _K) { init(_N, _K); }

std::vector<int> choose_cards(std::vector<int> cards) {
  return rightNodes[BPM::matchLeft[
      std::lower_bound(leftNodes.begin(), leftNodes.end(), cards) - leftNodes.begin()
  ]];
}

int find_discarded_card(std::vector<int> cards) {
  std::vector<int> original = leftNodes[BPM::match[
      std::lower_bound(rightNodes.begin(), rightNodes.end(), cards) - rightNodes.begin()
  ]];
  std::vector<bool> chosen(original.size(), false);

  for (int card : cards) {
    chosen[std::lower_bound(original.begin(), original.end(), card) - original.begin()] = true;
  }

  for (int i = 0; i < static_cast<int>(original.size()); ++i) {
    if (!chosen[i]) return original[i];
  }
  return -1;
}
