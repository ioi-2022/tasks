// O((N+M)^2 Q log (N + M))
#include "circuit.h"
#include <bits/stdc++.h>
using namespace std;

const int kMod = 1'000'002'022;

vector<pair<int, int>> dp;
vector<int> P;
vector<int> depth;
vector<vector<int>> children;
int N;

void calculate(int v) {
  if (v >= N) return;

  vector<int> temp_dp = {1};

  for (int chld : children[v]) {
    vector<int> next_dp(temp_dp.size()+1, 0);
    
    for (int i = 0 ; i < static_cast<int>(temp_dp.size()) ; i++) {
      next_dp[i] = (next_dp[i] + 1ll * temp_dp[i] * dp[chld].first) % kMod;
      next_dp[i+1] = (next_dp[i+1] + 1ll * temp_dp[i] * dp[chld].second) % kMod;
    }

    temp_dp = next_dp;
  }

  int chld_count = children[v].size();
  dp[v] = {0, 0};

  for (int i = 0 ; i < static_cast<int>(temp_dp.size()) ; i++) {
    dp[v].first = (dp[v].first + 1ll * (chld_count - i) * temp_dp[i]) % kMod;
    dp[v].second = (dp[v].second + 1ll * i * temp_dp[i]) % kMod;
  }
}

void init(int _N, int M, std::vector<int> _P, std::vector<int> A) {
  N = _N;
  P = _P;
  depth.resize(N+M);
  dp.resize(N+M);
  children.resize(N+M);

  for (int i = 1 ; i < static_cast<int>(P.size()) ; i++) {
    children[P[i]].push_back(i);
    depth[i] = 1 + depth[P[i]];
  }

  for (int i = N+M-1 ; i >= 0 ; i--) {
    if (i >= N) {
      dp[i] = {A[i-N] == 0, A[i-N] == 1};
    } else {
      calculate(i);
    }
  }
}

int count_ways(int L, int R) {
  set<pair<int, int>> st;
  for (int i = L ; i <= R ; i++) {
    swap(dp[i].first, dp[i].second);
    st.insert({depth[P[i]], P[i]});
  }

  while (!st.empty()) { // try to optimize, but this might be slower..
    pair<int, int> cur = *st.rbegin();
    st.erase(cur);

    int v = cur.second;
    calculate(v);
    if (v > 0) {
      st.insert({depth[P[v]], P[v]});
    }
  }
  return dp[0].second;
}
