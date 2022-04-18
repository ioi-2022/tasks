#include <bits/stdc++.h>
using namespace std;

constexpr int kUp = 0;
constexpr int kDown = 1;

long long max_weights(int N, int M, std::vector<int> X, std::vector<int> Y,
                      std::vector<int> W) {
  vector<vector<pair<int, int>>> fishes(N);

  for (int i = 0; i < M; ++i) {
    fishes[X[i]].push_back(make_pair(Y[i], i));
  }

  long long largest = 0;
  vector<pair<int, long long>> up, down;
  vector<vector<long long>> dp(M, vector<long long>(2, LLONG_MIN / 2));
  for (int x = 0; x < N; ++x) {
    sort(fishes[x].begin(), fishes[x].end());

    if (x > 0) {
      long long max_down = 0;
      for (int j = (int)fishes[x].size() - 1; j >= 0; --j) {
        int y = fishes[x][j].first;
        int i = fishes[x][j].second;
        while (down.size() > 0 && down.back().first > y) {
          max_down = max(max_down, down.back().second);
          down.pop_back();
        }

        long long &ret = dp[i][kDown];
        ret = largest + W[i];
        ret = max(ret, max_down + W[i]);
        if (j + 1 < static_cast<int>(fishes[x].size())) {
          ret = max(ret, dp[fishes[x][j + 1].second][kDown] + W[i]);
        }
      }
    }

    if (x >= 1) {
      for (auto [y, i] : fishes[x - 1]) {
        largest = max(largest, dp[i][kDown]);
      }
    }

    long long max_up = 0;
    for (int j = 0; j < static_cast<int>(fishes[x].size()); ++j) {
      int y = fishes[x][j].first;
      int i = fishes[x][j].second;
      while (up.size() > 0 && up.back().first < y) {
        max_up = max(max_up, up.back().second);
        up.pop_back();
      }

      long long &ret = dp[i][kUp];
      ret = largest + W[i];
      ret = max(ret, max_up + W[i]);
      if (j > 0) {
        ret = max(ret, dp[fishes[x][j - 1].second][kUp] + W[i]);
      }
    }

    if (x >= 1) {
      for (auto [y, i] : fishes[x - 1]) {
        largest = max(largest, dp[i][kUp]);
      }
    }

    down.clear();
    for (int j = 0; j < static_cast<int>(fishes[x].size()); ++j) {
      down.push_back(make_pair(fishes[x][j].first, dp[fishes[x][j].second][kDown]));
    }
    up.clear();
    for (int j = static_cast<int>(fishes[x].size()) - 1; j >= 0; --j) {
      up.push_back(make_pair(fishes[x][j].first, dp[fishes[x][j].second][kUp]));
    }  
  }

  long long answer = 0;
  for (int i = 0; i < M; ++i) {
    answer = max(answer, dp[i][kDown]);
    if (X[i] + 1 < N) {
      answer = max(answer, dp[i][kUp]);
    }
  }
  return answer;
}
