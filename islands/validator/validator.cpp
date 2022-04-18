#include "testlib.h"

#include <bits/stdc++.h>
using namespace std;

int main(int, char *argv[]) {
  registerValidation();
  std::string secret = inf.readLine();
  ensuref(secret == (std::string)"4e5150f5a11b21d9ac3b44f2259e7a3452343ad4",
          "Secret not found!");

  const char* subtask_name = argv[1];
  int max_N = 100'000;
  if (strcmp(subtask_name, "n-equals-two") == 0) {
    max_N = 2;
  } else if (strcmp(subtask_name, "complete") == 0) {
    max_N = 400;
  } else if (strcmp(subtask_name, "bidirectional") == 0) {
    max_N = 1000;
  } else if (strcmp(subtask_name, "double") == 0) {
    max_N = 1000;
  }

  int N = inf.readInt(2, max_N, "N");
  inf.readSpace();
  int M = inf.readInt(1, 200'000, "M");
  inf.readEoln();

  vector<int> U(M), V(M);
  for (int i = 0; i < M; ++i) {
    U[i] = inf.readInt(0, N - 1, "Ui");
    inf.readSpace();
    V[i] = inf.readInt(0, N - 1, "Vi");
    inf.readEoln();

    ensuref(U[i] != V[i], "Found U[i] = V[i]");
  }
  inf.readEof();

  if (strcmp(subtask_name, "bidirectional") == 0) {
    ensuref(M % 2 == 0, "M is not even");
    for (int i = 0; i < M; i += 2) {
      ensuref(make_pair(U[i], V[i]) == make_pair(V[i + 1], U[i + 1]),
              "(U[i], V[i]) != (V[i + 1], U[i + 1]) for an even i");
    }
  } else if (strcmp(subtask_name, "double") == 0) {
    ensuref(M % 2 == 0, "M is not even");
    for (int i = 0; i < M; i += 2) {
      ensuref(make_pair(U[i], V[i]) == make_pair(U[i + 1], V[i + 1]),
              "(U[i], V[i]) != (U[i + 1], V[i + 1]) for an even i");
    }
  } else if (strcmp(subtask_name, "complete") == 0) {
    vector<vector<int>> cnt(N, vector<int>(N));
    for (int i = 0; i < M; ++i) {
      ++cnt[U[i]][V[i]];
    }
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        if (i != j) {
          ensuref(
              cnt[i][j] == 1,
              "Found more or less than one edge connecting a pair of island");
        }
      }
    }
  }
  
  return 0;
}
