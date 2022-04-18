#include "prison.h"

#include <cmath>
#include <vector>

std::vector<std::vector<int>> devise_strategy(int N) {
  const int sqrt_limit = (int)sqrt(N) + 1;
  const int x = sqrt_limit * 3;
  std::vector<std::vector<int>> ret(x + 1, std::vector<int>(N + 1, 0));
  for (int i = 1; i <= N; ++i) {
    ret[0][i] = (int)floor(sqrt(i));
  }
  for (int i = 1; i < sqrt_limit; ++i) {
    ret[i][0] = 1;
    for (int j = 1; j <= N; ++j) {
      int cur = (int)floor(sqrt(j));
      if (cur == i)
        ret[i][j] = sqrt_limit;
      else
        ret[i][j] = (i < cur) ? -1 : -2;
    }
  }
  ret[sqrt_limit][0] = 0;
  for (int i = 1; i <= N; ++i) {
    int perfect_square = (int)floor(sqrt(i));
    perfect_square *= perfect_square;
    ret[sqrt_limit][i] = sqrt_limit + 1 + i - perfect_square;
  }
  for (int i = sqrt_limit + 1; i <= x; ++i) {
    ret[i][0] = 1;
    int now = i - sqrt_limit - 1;
    for (int j = 1; j <= N; ++j) {
      int perfect_square = (int)floor(sqrt(j));
      perfect_square *= perfect_square;
      int cur = j - perfect_square;
      ret[i][j] = (cur < now) ? -2 : -1;
    }
  }
  return ret;
}
