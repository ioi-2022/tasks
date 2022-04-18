// Compare binary number: 3 states for each bit
// X = 3 ⌈log2 N⌉ − 1 = 38

#include "prison.h"

#include <vector>

using namespace std;

vector<vector<int>> devise_strategy(int N) {
  int b = 1;
  while ((1 << b) <= N) {
    b++;
  }
  vector<vector<int>> ret(3 * b, vector<int>(N + 1, 0));
  for (int i = 0; i < 3 * b; i++) {
    int s = i / 3, t = i % 3;
    ret[i][0] = (t != 0);
    for (int j = 1; j <= N; j++) {
      if (t == 0) {
        ret[i][j] = (i + 1) + ((j >> (b - s - 1)) & 1);
      }
      else {
        int abit = t - 1;
        int bbit = (j >> (b - s - 1)) & 1;
        if (abit < bbit) {
          ret[i][j] = -1;
        }
        else if (abit > bbit) {
          ret[i][j] = -2;
        }
        else {
          ret[i][j] = (s != b - 1 ? (s + 1) * 3 : 0);
        }
      }
    }
  }
  return ret;
}
