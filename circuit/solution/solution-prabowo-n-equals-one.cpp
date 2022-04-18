// O(QM)

#include "circuit.h"

#include <numeric>
#include <vector>

std::vector<int> A;

void init(int, int, std::vector<int>, std::vector<int> _A) {
  A = _A;
}

int count_ways(int L, int R) {
  for (int j = --L; j < R; ++j) {
    A[j] ^= 1;
  }
  return std::accumulate(A.begin(), A.end(), 0);
}
