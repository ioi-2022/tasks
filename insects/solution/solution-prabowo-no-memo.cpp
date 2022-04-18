#include "insects.h"

#include <numeric>
#include <vector>

int min_cardinality(int N) {
  std::vector<int> insects(N);
  std::iota(insects.begin(), insects.end(), 0);

  std::vector<bool> isInside(N);

  // Count distincts
  std::vector<int> insides, outsides;
  int inside = 0;
  for (int insect : insects) {
    move_inside(insect);
    if (press_button() > 1) {
      move_outside(insect);
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
      move_inside(insect);
      if (press_button() > mid) {
        move_outside(insect);
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
        move_outside(insect);
        --inside;
      }
    }
  }

  return ans;
}
