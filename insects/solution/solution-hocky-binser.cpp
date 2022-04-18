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

  do {
    pair<int, int> bounds = get_bounds(device.size(), checking_set.size());
    if (bounds.first == bounds.second) {
      return bounds.first;
    }
    int middle = (bounds.first + bounds.second + 1) / 2;

    vector<int> just_moved;
    vector<int> next_checking_set;
    for (const auto &insect : checking_set) {
      move_inside(insect);
      int max_occurrence = press_button();
      if (max_occurrence > middle) {
        move_outside(insect);
        next_checking_set.push_back(insect);
      } else {
        just_moved.push_back(insect);
      }
    }
    if ((int) just_moved.size() + (int) device.size() == distinct * middle) {
      // Increase the lower bound
      device.insert(device.end(), just_moved.begin(), just_moved.end());
      swap(checking_set, next_checking_set);
    } else {
      // Decrease the upper bound
      for(const auto &insect : just_moved){
        move_outside(insect);
      }
      swap(checking_set, just_moved);
    }
  } while (true);

  return 0;
}
