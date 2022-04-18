// This solution only fails when N = 3 or N = 7

#include "prison.h"

#include <tuple>
#include <vector>

std::vector<std::vector<int>> devise_strategy(int N) {
  std::vector<int> dp(N + 1), divides(N + 1, 1);
  dp[2] = 0;
  for (int n = 3; n <= N; ++n) {
    dp[n] = n;
    for (int i = 1; i <= n - 2; ++i) {
      int res = i + dp[(n - 2 + i - 1) / i];
      if (res < dp[n]) {
        dp[n] = res;
        divides[n] = i;
      }
    }
  }
  std::vector<std::vector<int>> ret(dp[N] + 1, std::vector<int>(N + 1));

  std::vector<std::tuple<int, int, int>> ranges;
  ranges.emplace_back(1, N, 0);

  for (int size = N, current = 0, turn = 0; !ranges.empty(); ) {
    int nsize = (size - 2 + divides[size] - 1) / divides[size];
    std::vector<std::tuple<int, int, int>> nranges;
    for (auto [l, r, label] : ranges) {
      if (r - l <= 0) continue;

      ret[label][0] = turn;
      ret[label][l] = turn == 0 ? -1 : -2;
      ret[label][r] = turn == 0 ? -2 : -1;

      for (int d = 0, i = l + 1; d < divides[size]; ++d, i += nsize) {
        int nlabel = current + d + 1;
        if (nlabel > dp[N]) continue;
        for (int j = l; j < i; ++j) {
          if (j > r) break;
          ret[nlabel][j] = turn == 0 ? -2 : -1;
        }
        for (int j = i; j < i + nsize; ++j) {
          if (j > r - 1) break;
          ret[label][j] = nlabel;
        }
        for (int j = i + nsize < r ? i + nsize : r; j <= r; ++j) {
          ret[nlabel][j] = turn == 0 ? -1 : -2;
        }
        nranges.emplace_back(i, i + nsize - 1 < r - 1 ? i + nsize - 1 : r - 1, nlabel);
      }
    }

    current += divides[size];
    turn ^= 1;
    size = nsize;
    ranges = nranges;
  }

  return ret;
}
