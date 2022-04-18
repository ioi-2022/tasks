#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

struct Test {
  int K;
  vector<int> L;

  void print() {
    printf("%d %d\n", static_cast<int>(L.size()), K);
    for (int i = 0; i < static_cast<int>(L.size()); ++i) {
      printf("%d%c", L[i], " \n"[i == static_cast<int>(L.size()) - 1]);
    }
  }
};

Test randomCase(
    int minN, int maxN, int minK, int maxK,
    int minLRatNum, int minLRatDen, int minLDelta,
    int maxLRatNum, int maxLRatDen, int maxLDelta) {
  Test T;

  int N = rnd.next(minN, maxN);
  T.L = vector<int>(N);
  T.K = rnd.next(minK, maxK);

  for (int i = 0; i < N; ++i) {
    T.L[i] = rnd.next(max(1, minLRatNum * T.K / minLRatDen + minLDelta),
                      maxLRatNum * T.K / maxLRatDen + maxLDelta);
  }

  return T;
}

Test rangesCase(int K, vector<tuple<int, int, int>> ranges) {
  Test T;

  T.K = K;
  for (auto range : ranges) {
    for (int i = get<0>(range); i <= get<1>(range); i += get<2>(range)) {
      T.L.push_back(i);
    }
  }

  return T;
}

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 0);

  if (strcmp(argv[1], "random") == 0) {
    int minN = atoi(argv[2]);
    int maxN = atoi(argv[3]);
    int minK = atoi(argv[4]);
    int maxK = atoi(argv[5]);
    int minLRatNum = atoi(argv[6]);
    int minLRatDen = atoi(argv[7]);
    int minLDelta = atoi(argv[8]);
    int maxLRatNum = atoi(argv[9]);
    int maxLRatDen = atoi(argv[10]);
    int maxLDelta = atoi(argv[11]);

    randomCase(minN, maxN, minK, maxK,
               minLRatNum, minLRatDen, minLDelta,
               maxLRatNum, maxLRatDen, maxLDelta).print();
  } else if (strcmp(argv[1], "ranges") == 0) {
    int K = atoi(argv[2]);

    int ranges_size = atoi(argv[3]);
    vector<tuple<int, int, int>> ranges(ranges_size);
    for (int i = 0; i < ranges_size; ++i) {
      ranges[i] = make_tuple(atoi(argv[4 + 3 * i]),
                             atoi(argv[5 + 3 * i]),
                             atoi(argv[6 + 3 * i]));
    }

    rangesCase(K, ranges).print();
  }
}
