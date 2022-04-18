#include "fish.h"

#include <bits/stdc++.h>
using namespace std;

long long max_weights(int, int, std::vector<int>, std::vector<int>,
                      std::vector<int> W) {
  return accumulate(W.begin(), W.end(), 0);
}
