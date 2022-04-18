#include "fish.h"

#include <bits/stdc++.h>
using namespace std;

const int kMaxN = 3000;
const int kUp = 0, kDown = 1;
const long long kInf = 4e18;

long long dp[kMaxN+2][kMaxN+2][2];
long long psum[kMaxN+2][kMaxN+2];

long long max_weights(int N, int M, std::vector<int> X, std::vector<int> Y,
                      std::vector<int> W) {
  for (int i = 0 ; i < M ; i++) {
    psum[X[i]][Y[i]+1] += W[i]; // increment y by one to ease our life
  }
  for (int i = 0 ; i < N ; i++) {
    for (int j = 1 ; j <= N ; j++) {
      psum[i][j] += psum[i][j-1];
    }
  }

  for (int col = N; col <= N+1 ; col++) {
    for (int i = 0 ; i <= N ; i++) {
      dp[col][i][kUp] = 0;
      dp[col][i][kDown] = 0;
    }
  }

  for (int col = N-1 ; col >= 0 ; col--) {
    for (int i = 0 ; i <= N ; i++) {
      dp[col][i][kUp] = dp[col][i][kDown] = -kInf;
    }
    
    // prefix max
    long long prefix_max = -kInf;
    for (int i = 1 ; i <= N ; i++) {
      long long val = psum[col+1][i];
      if (col > 0) val -= psum[col][i];
      prefix_max = max(prefix_max, val + dp[col+1][i][kDown]);

      dp[col][i][kDown] = max(dp[col][i][kDown], prefix_max);
      dp[col][i][kUp] = max(dp[col][i][kUp], prefix_max);
      dp[col][i][kUp] = max(dp[col][i][kUp], val + dp[col+1][i][kUp]);
    }

    // suffix max
    long long suffix_max = -kInf;
    for (int i = N ; i >= 0 ; i--) {
      if (i > 0) {
        long long val = psum[col+1][i];
        if (col > 0) val += psum[col-1][i];
        val += dp[col+1][i][kUp];
        suffix_max = max(suffix_max, val);
      }

      long long nval = suffix_max;
      if (col > 0) nval -= (psum[col][i] + psum[col-1][i]);
      dp[col][i][kUp] = max(dp[col][i][kUp], nval);
    }

    // if we turn current col to 0

    for (int i = 1 ; i <= N ; i++) {
      dp[col][i][kUp] = max(dp[col][i][kUp], dp[col+2][0][kUp]);
      dp[col][i][kDown] = max(dp[col][i][kDown], dp[col+2][0][kUp]);
    }
    dp[col][0][kUp] = max(dp[col][0][kUp], dp[col+1][0][kUp]);

    prefix_max = -kInf;
    for (int i = 1 ; i <= N ; i++) {
      prefix_max = max(prefix_max, psum[col+2][i] + dp[col+2][i][kUp]);

      dp[col][i][kUp] = max(dp[col][i][kUp], prefix_max);
      dp[col][i][kDown] = max(dp[col][i][kDown], prefix_max);
    }

    if (col+2 <= N) {
      suffix_max = -kInf;
      for (int i = N ; i >= 0 ; i--) {
        if (i > 0) {
          long long val = psum[col+2][i] + dp[col+2][i][kUp];
          val += psum[col][i];
          suffix_max = max(suffix_max, val);
        }

        long long nval = suffix_max - psum[col][i];
        dp[col][i][kUp] = max(dp[col][i][kUp], nval);
        dp[col][i][kDown] = max(dp[col][i][kDown], nval);      
      }
    }
  }

  long long ret = dp[0][0][kUp];
  return ret;
}
