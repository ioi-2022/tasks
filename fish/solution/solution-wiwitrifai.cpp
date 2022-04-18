#include "fish.h"

#include <bits/stdc++.h>

using namespace std;

long long max_weights(int N, int M, vector<int> X, vector<int> Y, vector<int> W) {
  vector<vector<long long>> fishes(N+1);
  vector<vector<int>> states(N+1, {0});
  for (int i = 0; i < M; ++i) {
    ++Y[i];
    for (int j = max(X[i]-1, 0); j <= min(X[i]+1, N-1); ++j) {
      states[j].push_back(Y[i]);
    }
  }
  
  for (int i = 0; i <= N; ++i) {
    sort(states[i].begin(), states[i].end());
    states[i].erase(unique(states[i].begin(), states[i].end()), states[i].end());
    fishes[i].resize(states[i].size());
  }
  for (int i = 0; i < M; ++i) {
    int x = X[i];
    int y = lower_bound(states[x].begin(), states[x].end(), Y[i]) - states[x].begin();
    fishes[x][y] += W[i];
  }
  vector<vector<long long>> dp_up(N+1), dp_down(N+1);
  for (int i = 0; i <= N; ++i) {
    int sz = states[i].size();
    dp_up[i].assign(sz, 0);
    dp_down[i].assign(sz, 0);
    if (i > 1) {
      long long best = max(dp_up[i-2][0], dp_down[i-2][0]), sum = 0;
      for (int j = 0, lef = 0, mid = 0; j < sz; ++j) {
        while (lef+1 < (int)states[i-2].size() && states[i-2][lef+1] <= states[i][j]) {
          ++lef;
          while (mid+1 < (int)states[i-1].size() && states[i-1][mid+1] <= states[i-2][lef]) {
            ++mid;
            best += fishes[i-1][mid];
            sum += fishes[i-1][mid];
          }
          best = max(best, max(dp_up[i-2][lef], dp_down[i-2][lef]) + sum);
        }
        dp_up[i][j] = max(dp_up[i][j], best);
      }
      best = 0;
      sum = accumulate(fishes[i-1].begin(), fishes[i-1].end(), 0LL);
      for (int j = sz-1, lef = states[i-2].size(), mid = states[i-1].size(); j >= 0; --j) {
        while (lef > 0 && states[i-2][lef-1] >= states[i][j]) {
          --lef;
          while (mid > 0 && states[i-1][mid-1] > states[i-2][lef]) {
            --mid;
            sum -= fishes[i-1][mid];
          }
          best = max(best, max(dp_up[i-2][lef], dp_down[i-2][lef]) + sum);
        }
        dp_up[i][j] = max(dp_up[i][j], best);
      }
    }
    if (i > 0) {
      long long best = dp_up[i-1][0], sum = 0;
      for (int j = 0, lef = 0; j < sz; ++j) {
        while (lef+1 < (int)states[i-1].size() && states[i-1][lef+1] <= states[i][j]) {
          ++lef;
          best += fishes[i-1][lef];
          best = max(best, dp_up[i-1][lef]);
        }
        dp_up[i][j] = max(dp_up[i][j], best);
      }
      best = 0;
      sum = accumulate(fishes[i].begin(), fishes[i].end(), 0LL);
      for (int j = sz-1, lef = states[i-1].size(); j >= 0; --j) {
        while (lef > 0 && states[i-1][lef-1] >= states[i][j]) {
          --lef;
          best = max(best, max(dp_down[i-1][lef], dp_up[i-1][lef]) + sum);
        }
        dp_down[i][j] = max(dp_down[i][j], best-sum);
        sum -= fishes[i][j];
      }
    }
  }
  return max(dp_up[N][0], dp_down[N][0]);
}
