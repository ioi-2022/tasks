#include "prison.h"

#include <algorithm>
#include <functional>
#include <string>
#include <vector>

using namespace std;

vector<vector<int>> devise_strategy(int N) {
  vector<int> dp = { 2 };
  vector<vector<int> > dp_path = { vector<int>() };
  for (int i = 1; dp.back() < N; i++) {
    dp.push_back(0);
    dp_path.push_back(vector<int>());
    for (int j = i - 1; j >= 0; j--) {
      if (dp[i] < (i - j) * dp[j] + 2) {
        dp[i] = (i - j) * dp[j] + 2;
        dp_path[i] = dp_path[j];
        dp_path[i].push_back(i - j);
      }
    }
  }
  vector<int> splits = dp_path.back();
  reverse(splits.begin(), splits.end());
  int b = splits.size();
  vector<string> vals;
  function<void(int, string)> generate = [&](int depth, string str) {
    vals.push_back(str + string(b - depth + 1, 'L'));
    if (depth != b) {
      for (int ch = 0; ch < splits[depth]; ch++) {
        generate(depth + 1, str + char(ch + '0'));
      }
    }
    vals.push_back(str + string(b - depth + 1, 'R'));
  };
  generate(0, "");
  vector<int> cs(dp.back() + 1), ct(dp.back());
  int sum = 1;
  for (int i = 0; i < b; i++) {
    for (int j = 0; j < splits[i]; j++) {
      cs[sum + j] = i + 1;
      ct[sum + j] = j;
    }
    sum += splits[i];
  }
  vector<vector<int>> ret(sum, vector<int>(N+1, 0));
  for (int i = 0; i < sum; i++) {
    int s = cs[i], t = ct[i];
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
        ret[i][j+1] = (s != 0 ? i - t + splits[s - 1] : 1) + int(vals[j][s] - '0');
      }
    }
  }
  return ret;
}
