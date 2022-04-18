// similar to solution-wiwitrifai-ac.cpp but changing only one leaf value.

#include "prison.h"

#include <bits/stdc++.h>

using namespace std;

std::vector<std::vector<int>> devise_strategy_impl(int N) {
  std::vector<int> dp = {2}, best = {0};
  for (int i = 1; dp.back() < N; ++i) {
    dp.push_back(2);
    best.push_back(0);
    for (int j = 1; j <= i; ++j) {
      int cur = dp[i - j] * j + 2;
      if (cur > dp[i]) {
        dp[i] = cur;
        best[i] = j;
      }
    }
  }
  int x = (int)dp.size() - 1;
  int cover = dp[x];
  std::vector<int> divide;
  int now = x;
  while (now > 0) {
    divide.push_back(best[now]);
    now -= best[now];
  }
  std::vector<std::vector<int>> ret(x + 1);
  for (int i = 0; i <= x; ++i) {
    ret[i].assign(cover+1, 0);
  }
  std::vector<int> offset(divide.size(), 1);
  for (int i = 0; i < (int)offset.size(); ++i) {
    for (int j = 0; j < divide[i]; ++j)
      ret[offset[i] + j][0] = (i + 1) & 1;
    if (i + 1 < (int)offset.size())
      offset[i+1] = offset[i] + divide[i];
  }
  auto build = [&](auto& self, int id, int depth, int child, int l, int r) -> void {
    if (child == -1) {
      ret[id][l] = (ret[id][0]) ? -2 : -1;
      ret[id][r] = (ret[id][0]) ? -1 : -2;
      if (l + 1 >= r)
        return;
      assert(depth < (int)divide.size());
      for (int i = 0; i < divide[depth]; ++i) {
        self(self, id, depth, i,  l, r);
      }
    } else {
      assert(depth < (int)divide.size());
      ret[offset[depth] + child][l] = (ret[id][0]) ? -1 : -2;
      ret[offset[depth] + child][r] = (ret[id][0]) ? -2 : -1;
      ++l, --r;
      int len = (r - l + 1), sublen = len / divide[depth];
      assert(sublen * divide[depth] == len);
      for (int i = 0; i < divide[depth]; ++i) {
        int from = l + sublen * i;
        int to = l + sublen * (i + 1) - 1;
        if (i == child) {
          for (int j = from; j <= to; ++j) {
            ret[id][j] = offset[depth] + i;
          }
          self(self, offset[depth] + i, depth + 1, -1, from, to);
        } else {
          int ans = (ret[id][0] ^ (i < child)) ? -2 : -1;
          for (int j = from; j <= to; ++j) {
            ret[offset[depth] + child][j] = ans;
          }
        }
      }
    }
  };
  build(build, 0, 0, -1, 1, cover);
  for (int i = 0; i <= x; ++i)
    ret[i].resize(N + 1);
  return ret;
}

bool simulate_all(int N, const vector<vector<int>>& strategy) {
  int x = strategy.size() - 1;
  std::vector<int> visited(x+1, -1);
  int counter = 0;
  auto simulate = [&](int A, int B) {
    ++counter;
    int expected = A < B ? -1 : -2;
    int whiteboard = 0;
    while (visited[whiteboard] != counter) {
      visited[whiteboard] = counter;
      int check = strategy[whiteboard][0];
      whiteboard = strategy[whiteboard][check == 0 ? A : B];
      if (whiteboard < 0) {
        return whiteboard == expected;
      }
    }
    return false;
  };

  for (int A = 1; A <= N; ++A) {
    for (int B = 1; B <= N; ++B) {
      if (A == B) continue;
      if (!simulate(A, B))
        return false;
    }
  }
  return true;
}

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int rnd(int l, int r) {
  return uniform_int_distribution<int>(l, r)(rng);
}

vector<vector<int>> devise_strategy(int N) {
  auto strategy = devise_strategy_impl(N);
  int x = (int)strategy.size() - 1;
  if (N >= 2000) {
    while (true) {
      int id = rnd(0, x), val = rnd(1, N);
      if (strategy[id][val] < 0) {
        strategy[id][val] = -3 - strategy[id][val];
        break;
      }
    }
    assert(!simulate_all(N, strategy));
  }
  return strategy;
}
