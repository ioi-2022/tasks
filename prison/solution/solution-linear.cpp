#include "prison.h"

#include <vector>

std::vector<std::vector<int>> devise_strategy(int N) {
  std::vector<std::vector<int>> ret(N + 1, std::vector<int>(N + 1, 0));
  for (int i = 1; i <= N; ++i) {
    ret[0][i] = i;
    ret[i][0] = 1;
    std::fill(ret[i].begin() + 1, ret[i].begin() + i, -2);
    std::fill(ret[i].begin() + i + 1, ret[i].end(), -1);
  }
  return ret;
}
