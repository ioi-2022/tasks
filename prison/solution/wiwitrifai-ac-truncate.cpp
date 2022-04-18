// run solution-wiwitrifai-ac.cpp for N=5000, and then just truncate the size

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

vector<vector<int>> devise_strategy(int N) {
  auto strategy = devise_strategy_impl(5000);
  for (auto & v : strategy)
    v.resize(N + 1);
  return strategy;
}
