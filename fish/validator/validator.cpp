#include "testlib.h"

#include <bits/stdc++.h>
using namespace std;

constexpr int kMaxN = 100'000;
constexpr int kMaxM = 300'000;
constexpr int kMaxW = 1'000'000'000;

int main(int, char *argv[]) {
  registerValidation();
  std::string secret = inf.readLine();
  ensuref(secret == (std::string)"f785163bfcb92ce6ac387bba5d2f29a0e0f37f19",
          "Secret not found!");

  const char* subtask_name = argv[1];

  int max_N = kMaxN;
  int max_X = INT_MAX;
  int max_Y = INT_MAX;

  if (strcmp(subtask_name, "two-columns") == 0) {
    max_X = 1;
  } else if (strcmp(subtask_name, "one-dim") == 0) {
    max_Y = 0;
  } else if (strcmp(subtask_name, "cubic-small-y") == 0) {
    max_N = 300;
    max_Y = 8;
  } else if (strcmp(subtask_name, "cubic") == 0) {
    max_N = 300;
  } else if (strcmp(subtask_name, "quadratic") == 0) {
    max_N = 3000;
  }

  int N = inf.readInt(2, max_N, "N");
  inf.readSpace();
  int M = inf.readInt(1, kMaxM, "M");
  inf.readEoln();

  max_X = min(max_X, N - 1);
  max_Y = min(max_Y, N - 1);

  vector<int> X(M), Y(M);
  set<pair<int, int>> positions;
  for (int i = 0; i < M; ++i) {
    X[i] = inf.readInt(0, max_X, "Xi");
    inf.readSpace();
    Y[i] = inf.readInt(0, max_Y, "Yi");
    inf.readSpace();
    inf.readInt(1, kMaxW, "Wi");
    inf.readEoln();

    ensuref(positions.find(make_pair(X[i], Y[i])) == positions.end(),
            "Found more than one fish in the same cell");
    positions.insert(make_pair(X[i], Y[i]));
  }
  inf.readEof();

  if (strcmp(subtask_name, "sum-all") == 0) {
    ensuref(all_of(X.begin(), X.end(), [] (int x) { return x % 2 == 0; }),
            "Found an odd value of Xi");
  } else if (strcmp(subtask_name, "max-per-column") == 0) {
    vector<int> fish_in_column(N);
    for (int i = 0; i < M; ++i) {
      ++fish_in_column[X[i]];
    }
    ensuref(all_of(fish_in_column.begin(),
                   fish_in_column.end(), [] (int fish) { return fish <= 2; }),
            "Found a column with more than two fish");
  }

  return 0;
}
