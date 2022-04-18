#include "team.h"

#include <bits/stdc++.h>
using namespace std;

int maximum_teams(int N, int K, std::vector<int> L) {
  sort(L.begin(), L.end());
  int answer = 0;
  int hi = N - 1;
  int lo = 0;
  while (lo < hi - 1) {
    while (lo < hi - 1 && L[lo] + L[hi] < K) {
      ++lo;
    }
    if (lo < hi - 1) {
      ++answer;
      hi -= 2;
      ++lo;
    }
  }
  return answer;
}
