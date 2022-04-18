#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

constexpr int kMinH = 1;
constexpr int kMaxH = 1000000000;
constexpr int kMinD = 1;
constexpr int kMaxD = 1000000000;

struct Test {
  vector<int> H;
  vector<tuple<int, int, int>> queries;

  void print() {
    int N = H.size();
    int Q = queries.size();
    printf("%d %d\n", N, Q);
    for (int i = 0; i < N; ++i) {
      printf("%d%c", H[i] + 1, " \n"[i == N - 1]);
    }
    for (int i = 0; i < Q; ++i) {
      printf("%d %d %d\n",
             get<0>(queries[i]), get<1>(queries[i]), get<2>(queries[i]));
    }
  }
};

void assignRandomHeights(Test& T, int N) {
  T.H = vector<int>(N);
  set<int> used_height;
  for (int i = 0; i < N; ++i) {
    while (true) {
      T.H[i] = rnd.next(kMinH, kMaxH);
      if (used_height.count(T.H[i]) == 0) {
        used_height.insert(T.H[i]);
        break;
      }
    }
  }
}

void addRandomQueries(
    Test& T, int N, int queries,
    int maxD = kMaxD, bool full_interval = false, bool constantD = false) {
  for (int i = 0; i < queries; ++i) {
    int R = rnd.next(N);
    int L = rnd.next(N + 1);
    if (L > R) {
      L--;
      swap(L, R);
    }
    if (full_interval) {
      L = 0;
      R = N - 1;
    }
    int D = rnd.next(kMinD, maxD);
    if (constantD && T.queries.size() > 0) {
      D = get<2>(T.queries[0]);
    }
    T.queries.emplace_back(L, R, D);
  }
}

void swapHeights(Test& T, int N, int swaps) {
  while (swaps--) {
    swap(T.H[rnd.next(N)], T.H[rnd.next(N)]);
  }
}

Test randomCase(
    int N, int Q, bool bitonic, int maxD, bool full_interval, bool constantD) {
  Test T;

  assignRandomHeights(T, N);
  if (bitonic) {
    int peak_index = max_element(T.H.begin(), T.H.end()) - T.H.begin();
    sort(T.H.begin(), T.H.begin() + peak_index);
    sort(T.H.begin() + peak_index + 1, T.H.end());
    reverse(T.H.begin() + peak_index + 1, T.H.end());
  }
  addRandomQueries(T, N, Q, maxD, full_interval, constantD);

  return T;
}

Test almostSortedCase(
    int N, int Q, int maxD, bool full_interval, bool constantD,
    bool rev, int swaps) {
  Test T = randomCase(N, Q, false, maxD, full_interval, constantD);
  sort(T.H.begin(), T.H.end());
  if (rev) {
    reverse(T.H.begin(), T.H.end());
  }
  swapHeights(T, N, swaps);
  return T;
}

Test evenOddCase(
    int N, int Q, int maxD, bool full_interval, bool constantD, int swaps) {
  Test T = randomCase(N, Q, false, maxD, full_interval, constantD);
  vector<int> sorted_H = T.H;
  sort(sorted_H.begin(), sorted_H.end());
  vector<int> lower_half, upper_half;
  for (int i = 0; i < N; ++i) {
    if (T.H[i] <= sorted_H[(N - 1) >> 1]) {
      lower_half.push_back(T.H[i]);
    } else {
      upper_half.push_back(T.H[i]);
    }
  }
  for (int i = 0; i < N; ++i) {
    if (i % 2 == 0) {
      T.H[i] = lower_half[i >> 1];
    } else {
      T.H[i] = upper_half[i >> 1];
    }
  }

  swapHeights(T, N, swaps);
  return T; 
}

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 0);

  if (strcmp(argv[1], "random") == 0) {
    int N = rnd.next(atoi(argv[2]), atoi(argv[3]));
    int Q = rnd.next(atoi(argv[4]), atoi(argv[5]));
    bool bitonic = strcasecmp("true", argv[6]) == 0;
    int maxD = atoi(argv[7]);
    bool full_interval = strcasecmp("true", argv[8]) == 0;
    bool constantD = strcasecmp("true", argv[9]) == 0;
    randomCase(N, Q, bitonic, maxD, full_interval, constantD).print();
  } else if (strcmp(argv[1], "almost-sorted") == 0) {
    int N = rnd.next(atoi(argv[2]), atoi(argv[3]));
    int Q = rnd.next(atoi(argv[4]), atoi(argv[5]));
    int maxD = atoi(argv[6]);
    bool full_interval = strcasecmp("true", argv[7]) == 0;
    bool constantD = strcasecmp("true", argv[8]) == 0;
    bool rev = strcasecmp("true", argv[9]) == 0;
    int swaps = atoi(argv[10]);
    almostSortedCase(N, Q, maxD, full_interval, constantD, rev, swaps).print();
  } else if (strcmp(argv[1], "even-odd") == 0) {
    int N = rnd.next(atoi(argv[2]), atoi(argv[3]));
    int Q = rnd.next(atoi(argv[4]), atoi(argv[5]));
    int maxD = atoi(argv[6]);
    bool full_interval = strcasecmp("true", argv[7]) == 0;
    bool constantD = strcasecmp("true", argv[8]) == 0;
    int swaps = atoi(argv[9]);
    evenOddCase(N, Q, maxD, full_interval, constantD, swaps).print();
  }
}
