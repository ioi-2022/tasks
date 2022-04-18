#pragma GCC diagnostic ignored "-Wparentheses"

#include "prison.h"

#include <vector>

std::vector<std::vector<int>> devise_strategy(int N) {
  int msb = 32 - __builtin_clz(N);
  std::vector<std::vector<int>> ret(msb * 2 + 1, std::vector<int>(N + 1, msb & 1));
  for (int bit = msb - 1; bit >= 0; --bit) {
    int st = (msb - bit) * 2 - 1;
    ret[st][0] = ret[st + 1][0] = bit & 1;
    for (int i = 1; i <= N; ++i) {
      ret[st + (i >> bit & 1)][i] = bit == 0 ? 0 : st + (i >> bit - 1 & 1) + 2;
      ret[st + (i >> bit & 1 ^ 1)][i] = bit & 1 ^ i >> bit & 1 ? -2 : -1;
    }
  }
  for (int i = 1; i <= N; ++i) ret[0][i] = (i >> msb - 1 & 1) + 1;
  return ret;
}
