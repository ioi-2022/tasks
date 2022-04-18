#include "team.h"

#include <bits/stdc++.h>
using namespace std;

int maximum_teams(int N, int K, std::vector<int> L) {
  sort(L.begin(), L.end());
  return N > 2 && L[0] + L[N - 1] > K ? 1 : 0;
}
