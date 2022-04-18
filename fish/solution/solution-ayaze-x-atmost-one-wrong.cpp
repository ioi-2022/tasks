// WA: only take all fish in column 0 or column 1
#include "fish.h"

#include <bits/stdc++.h>
using namespace std;

long long max_weights(int, int M, std::vector<int> X, std::vector<int>,
                      std::vector<int> W) {
  long long ret = 0;
  for (int x = 0 ; x < 2 ; x++) {
    long long total_weight = 0;
    for (int i = 0 ; i < M ; i++) {
      if (X[i] == x) {
        total_weight += W[i];
      }
    }
    ret = max(ret, total_weight);
  }
  return ret;
}
