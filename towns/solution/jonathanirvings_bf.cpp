#include "towns.h"

#include <bits/stdc++.h>
using namespace std;

int find_town(int N) {
  std::vector<int> out_deg(N, 0);
  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j < N; ++j) {
      ++out_deg[check_road(i, j) ? i : j];
    }
  }
  for (int i = 0; i < N; ++i) {
    if (out_deg[i] <= 1) {
      return i;
    }
  }
  return -1;
}
