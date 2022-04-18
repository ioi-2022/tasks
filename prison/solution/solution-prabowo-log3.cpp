#include "prison.h"

#include <vector>

std::vector<std::vector<int>> devise_strategy(int N) {
  std::vector<int> p3 = {1};
  while (p3.back() <= N) {
    p3.push_back(p3.back() * 3);
  }

  std::vector<std::vector<int>> ret(p3.size() * 3 - 2, std::vector<int>(N + 1));
  for (int i = 0; i + 1 < static_cast<int>(p3.size()); ++i) {
    int turn = i % 2;
    int len = p3.end()[-i - 2];
    for (int j = 0; j < 3; ++j) {
      int x = i * 3 + j + 1;
      ret[x][0] = turn;
      for (int k = 1; k <= N; ++k) {
        if (k / len % 3 < j) {
          ret[x][k] = turn == 0 ? -1 : -2;
        } else if (k / len % 3 > j) {
          ret[x][k] = turn == 0 ? -2 : -1;
        } else {
          ret[x][k] = len == 1 ? 0 : i * 3 + k * 3 / len % 3 + 4;
        }
      }
    }
  }
  ret[0][0] = 1;
  for (int k = 1; k <= N; ++k) {
    ret[0][k] = k / p3.end()[-2] % 3 + 1;
  }

  return ret;
}
