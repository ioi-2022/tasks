#include "insects.h"

#include <algorithm>
#include <map>
#include <numeric>
#include <random>
#include <vector>

std::vector<int> idx;
void _move_inside(int i) { move_inside(idx[i]); }
void _move_outside(int i) { move_outside(idx[i]); }
int _press_button() { return press_button(); }

int min_cardinality(int N) {
  idx.resize(N);
  std::iota(idx.begin(), idx.end(), 0);
  std::shuffle(idx.begin(), idx.end(), std::mt19937());

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
