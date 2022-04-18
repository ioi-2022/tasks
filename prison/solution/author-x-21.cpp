// X = 3 ⌈log3 (N-1)⌉ - 3 = 21

#include "prison.h"

#include <string>
#include <vector>
#include <functional>

using namespace std;

vector<vector<int>> devise_strategy(int N) {
  int b = 0, cur = 2;
  while (cur < N) {
    cur = 3 * cur + 2;
    b++;
  }
  vector<string> vals;
  function<void(int, string)> generate = [&](int depth, string str) {
    vals.push_back(str + string(b - depth + 1, 'L'));
    if (depth != b) {
      for (char ch = '0'; ch <= '2'; ch++) {
        generate(depth + 1, str + ch);
      }
    }
    vals.push_back(str + string(b - depth + 1, 'R'));
  };
  generate(0, "");
  vector<vector<int>> ret(3 * b + 1, vector<int>(N+1, 0));
  for (int i = 0; i <= 3 * b; i++) {
    int s = (i + 2) / 3, t = (i + 2) % 3;
    ret[i][0] = (s % 2);
    for (int j = 0; j < N; j++) {
      int xbit = (s != 0 ? t : -1);
      int ybit = (s != 0 && vals[j][s - 1] != 'L' && vals[j][s - 1] != 'R' ? vals[j][s - 1] - '0' : -1);
      if (vals[j][s - 1] == 'L') {
        ret[i][j+1] = (s % 2 == 1 ? -2 : -1);
      }
      else if (vals[j][s - 1] == 'R') {
        ret[i][j+1] = (s % 2 == 1 ? -1 : -2);
      }
      else if (xbit < ybit) {
        ret[i][j+1] = (s % 2 == 1 ? -1 : -2);
      }
      else if (xbit > ybit) {
        ret[i][j+1] = (s % 2 == 1 ? -2 : -1);
      }
      else if (vals[j][s] == 'L') {
        ret[i][j+1] = (s % 2 == 1 ? -2 : -1);
      }
      else if (vals[j][s] == 'R') {
        ret[i][j+1] = (s % 2 == 1 ? -1 : -2);
      }
      else {
        ret[i][j+1] = (s * 3 + 1) + int(vals[j][s] - '0');
      }
    }
  }
  return ret;
}
