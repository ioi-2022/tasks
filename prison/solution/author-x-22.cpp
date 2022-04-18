// Compare ternary number + pruning on last digit
// X = 3 ⌈log3 N⌉ - 2 = 22

#include "prison.h"

#include <vector>

using namespace std;

vector<vector<int>> devise_strategy(int N) {
  vector<int> power3 = { 1, 3, 9 };
  int b = power3.size() - 1;
  while (power3.back() <= N) {
    power3.push_back(power3.back() * 3);
    b++;
  }
  vector<vector<int>> ret(3 * b - 1, vector<int>(N+1, 0));
  for (int i = 0; i <= 3 * b - 2; i++) {
    int s = (i + 2) / 3, t = (i + 2) % 3;
    ret[i][0] = (s % 2);
    for (int j = 1; j <= N; j++) {
      if (i == 0) {
        ret[i][j] = (j / power3[b - 1]) + 1;
      }
      else {
        int xbit = (s != b ? t : 1);
        int ybit = (j / power3[b - s]) % 3;
        if (xbit < ybit) {
          ret[i][j] = (s % 2 == 1 ? -1 : -2);
        }
        else if (xbit > ybit) {
          ret[i][j] = (s % 2 == 1 ? -2 : -1);
        }
        else if (s != b - 1) {
          ret[i][j] = (s != b ? (s * 3 + 1) + ((j / power3[b - s - 1]) % 3) : 0);
        }
        else {
          ret[i][j] = (j % 3 == 1 ? s * 3 + 1 : ((s + (j % 3 == 0 ? 1 : 0)) % 2 == 1 ? -1 : -2));
        }
      }
    }
  }
  return ret;
}
