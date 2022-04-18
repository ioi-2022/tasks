// Compare binary number: store bit of a and b alternatingly based on parity
// X = 2 ⌈log2 N⌉ = 26

#include "prison.h"

#include <vector>

using namespace std;

vector<vector<int>> devise_strategy(int N) {
  int b = 0;
  while ((1 << b) <= N) {
    b++;
  }
  vector<vector<int>> ret(2 * b + 1, vector<int>(N + 1, 0));
  for (int i = 0; i <= 2 * b; i++) {
    int s = (i + 1) / 2, t = (i + 1) % 2;
    ret[i][0] = (s % 2);
    for (int j = 1; j <= N; j++) {
      if (i == 0) {
        ret[i][j] = ((j >> (b - 1)) & 1) + 1;
      }
      else {
        int xbit = t;
        int ybit = (j >> (b - s)) & 1;
        if (xbit < ybit) {
          ret[i][j] = (s % 2 == 1 ? -1 : -2);
        }
        else if (xbit > ybit) {
          ret[i][j] = (s % 2 == 1 ? -2 : -1);
        }
        else {
          ret[i][j] = (s != b ? (s * 2 + 1) + ((j >> (b - s - 1)) & 1) : 0);
        }
      }
    }
  }
  return ret;
}
