#include "team.h"

#include <bits/stdc++.h>
using namespace std;

int maximum_teams(int N, int K, std::vector<int> L) {
  sort(L.begin(), L.end());
  int answer = 0;
  int hi = N - 1;
  int lo = 0;
  while (lo + 1 < hi) {
    while (lo + 1 < hi && L[lo] + L[hi] <= K) {
      ++lo;
    }
    if (lo + 1 < hi) {
      ++answer;
      --hi;
      lo += 2;
    }
  }
  return answer;
}
