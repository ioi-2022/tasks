#include "insects.h"

#include <map>
#include <numeric>
#include <vector>

int N;
std::vector<bool> lastPress, where;

void _move_inside(int i) {
  where[i] = true;
}

void _move_outside(int i) {
  where[i] = false;
}

std::map<std::vector<bool>, int> memo;
int _press_button() {
  if (memo.find(where) != memo.end()) return memo[where];
  for (int i = 0; i < N; ++i) {
    if (where[i] == lastPress[i]) continue;
    if (where[i]) move_inside(i);
    else move_outside(i);
  }
  lastPress = where;
  return memo[where] = press_button();
}

int min_cardinality(int _N) {
  N = _N;
  lastPress.assign(N, false); where.assign(N, false);

  std::vector<int> insects(N);
  std::iota(insects.begin(), insects.end(), 0);

  std::vector<bool> isInside(N);

  // Count distincts
  std::vector<int> insides, outsides;
  int inside = 0;
  for (int insect : insects) {
    _move_inside(insect);
    if (_press_button() > 1) {
      _move_outside(insect);
      outsides.push_back(insect);
    } else {
      insides.push_back(insect);
      ++inside;
    }
  }
  int distinct = inside;
  insects = outsides;

  int l = 1, r = N / distinct;
  int ans = -1;
  while (l <= r) {
    int mid = (l + r + 1) / 2;

    insides.clear(); outsides.clear();
    for (int insect : insects) {
      _move_inside(insect);
      if (_press_button() > mid) {
        _move_outside(insect);
        outsides.push_back(insect);
      } else {
        insides.push_back(insect);
        ++inside;
      }
    }

    if (inside == mid * distinct) {
      ans = mid;
      l = mid + 1;
      insects = outsides;
    } else {
      r = mid - 1;
      insects = insides;
      for (int insect : insects) {
        _move_outside(insect);
        --inside;
      }
    }
  }

  return ans;
}
