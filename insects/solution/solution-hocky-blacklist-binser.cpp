#include "insects.h"

#include <bits/stdc++.h>
using namespace std;

int min_cardinality(int N) {

  vector<int> device;
  vector<int> checking_set;

  for (int i = 0; i < N; i++) {
    move_inside(i);
    int max_occurrence = press_button();
    if (max_occurrence == 2) {
      move_outside(i);
      checking_set.push_back(i);
    } else {
      device.push_back(i);
    }
  }

  int distinct = device.size();
  auto get_bounds = [&](int device_size, int checking_size) -> pair<int, int> {
    return {device_size / distinct, (device_size + checking_size) / distinct};
  };

  vector <int> blacklist(N);
  pair<int, int> bounds = get_bounds(device.size(), checking_set.size());

  vector<pair<int, int>> just_moved;
  do {
    if (bounds.first == bounds.second) {
      return bounds.first;
    }
    assert(bounds.first < bounds.second);
    int middle = (bounds.first + bounds.second + 1) / 2;
    vector<int> next_checking_set;
    for (const auto &insect : checking_set) {
      if(blacklist[insect]) {
        next_checking_set.push_back(insect);
        continue;
      }
      move_inside(insect);
      int max_occurrence = press_button();
      if (max_occurrence > middle) {
        move_outside(insect);
        next_checking_set.push_back(insect);
      } else {
        just_moved.push_back({insect, max_occurrence});
      }
    }
    if ((int) just_moved.size() + (int) device.size() == distinct * middle) {
      // Increase the lower bound
      for (const auto &insect : just_moved) {
        device.push_back(insect.first);
      }
      swap(checking_set, next_checking_set);
      bounds = get_bounds(device.size(), checking_set.size());
      just_moved.clear();
      blacklist.assign(N, 0);
    } else {

      // Decrease the upper bound
      bounds = get_bounds(device.size(), just_moved.size());
      if (bounds.first < bounds.second) {
        checking_set.clear();
        int next_middle = (bounds.first + bounds.second + 1) / 2;
        int pushed = 0;
        for (int i = 0; i < (int) just_moved.size(); i++) {
          const auto &insect = just_moved[i];
          if (insect.second <= next_middle) {
            pushed++;
          } else {
            if(i > 0 && just_moved[i - 1].second < just_moved[i].second) blacklist[insect.first] = 1;
            move_outside(insect.first);
            checking_set.push_back(insect.first);
          }
        }
        just_moved.resize(pushed);
      } else {
        return bounds.first;
      }
    }
  } while (true);

  return 0;
}
