#include "fish.h"
 
#include <bits/stdc++.h>
using namespace std;
 
#define MAXN 100100
#define MAXM 300300
 
vector<pair<int, int>> byx[MAXN];
vector<long long> sum[MAXN];
 
long long max_weights(int N, int M, vector<int> X, vector<int> Y, vector<int> W) {
  // Subtask 1.
  if (all_of(W.begin(), W.end(), [](int i) { return i % 2 == 0; })) {
    return accumulate(W.begin(), W.end(), 0LL);
  }
 
  for (int i = 0; i < M; i++) {
    byx[X[i]].emplace_back(Y[i], W[i]);
  }
 
  for (int x = 0; x < N; x++) {
    sort(byx[x].begin(), byx[x].end());
    sum[x].push_back(0);
    for (auto p : byx[x]) {
      sum[x].push_back(sum[x].back() + p.second);
    }
  }
 
  // Subtask 2.
  if (all_of(X.begin(), X.end(), [](int i) { return i <= 1; })) {
    return max(sum[0].back(), sum[1].back());
  }
 
  // Subtask 3 - 5.
  int maxy = (*max_element(Y.begin(), Y.end())) + 1;
  // dp[n][cur][prev]
  long long dp[N + 1][maxy + 1][maxy + 1] = {};
  long long dpmax[N + 1][maxy + 1] = {};
 
  for (int x = 1; x < N; x++) {
    long long wpreb = 0;
    int ipre = 0;
    for (int cur = 0; cur <= maxy; cur++) {
      if (ipre < byx[x - 1].size() && byx[x - 1][ipre].first == cur - 1) {
        wpreb += byx[x - 1][ipre].second;
        ipre++;
      }
      int jpre = 0;
      long long wcur = 0;
      int icur = 0;
      long long wpre = wpreb;
      for (int pre = 0; pre <= maxy; pre++) {
        if (icur < byx[x].size() && byx[x][icur].first == pre - 1) {
          if (pre > cur) {
            wcur += byx[x][icur].second;
          }
          icur++;
        }
        if (jpre < byx[x - 1].size() && byx[x - 1][jpre].first == pre - 1) {
          if (jpre < ipre) {
            wpre -= byx[x - 1][jpre].second;
          }
          jpre++;
        }
 
        if (pre == 0 && cur == 0) {
          // case 1: pre == 0 && cur == 0
          dp[x + 1][cur][pre] = dpmax[x][pre];
        }
        else if (cur == 0) {
          // case 2: pre > 0 && cur == 0
          dp[x + 1][cur][pre] = wcur + dpmax[x][pre];
        }
        else if (pre == 0) {
          // case 3: pre == 0 && cur > 0
          dp[x + 1][cur][pre] = dpmax[x][pre];
          int kpre = 0;
          long long wkpre = 0;
          for (int k = 0; k <= cur; k++) {
            if (kpre < byx[x - 1].size() && byx[x - 1][kpre].first == k - 1) {
              wkpre += byx[x - 1][kpre].second;
              kpre++;
            }
            dp[x + 1][cur][pre] = max(dp[x + 1][cur][pre], dp[x][pre][k] + wpre - wkpre);
          }
        }
        else {
          // case 4: pre > 0 && cur > 0
          dp[x + 1][cur][pre] = wpre + wcur + dp[x][pre][0];
        }
 
        dpmax[x + 1][cur] = max(dpmax[x + 1][cur], dp[x + 1][cur][pre]);
 
        // printf("dp[x = %d][cur = %d][pre = %d] = %lld (wpre = %lld, wcur = %lld)\n", x, cur, pre, dp[x + 1][cur][pre], wpre, wcur);
      }
    }
  }
 
  return *max_element(dpmax[N], dpmax[N] + maxy + 1);
}
