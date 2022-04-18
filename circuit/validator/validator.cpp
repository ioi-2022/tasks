#include "testlib.h"

#include <cstring>

#include <string>
#include <vector>

const int kMaxN = 100000;
const int kMaxM = 100000;
const int kMaxQ = 100000;

int main(int, char *argv[]) {
  registerValidation();
  std::string secret = inf.readLine();
  ensuref(secret == (std::string)"385045b28fbc1903eca997d513040235e0229e01",
          "Secret not found!");

  const char* subtask_name = argv[1];

  int max_N = kMaxN, max_M = kMaxM, max_Q = kMaxQ;

  if (strcmp(subtask_name, "n-equals-one") == 0) {
    max_Q = 5;
    max_N = 1;
    max_M = 1000;
  } else if ((strcmp(subtask_name, "brute-binary") == 0) || (strcmp(subtask_name, "brute") == 0)) {
    max_Q = 5;
    max_N = 1000;
    max_M = 1000;
  } else if (strcmp(subtask_name, "quadratic") == 0) {
    max_N = 5000;
    max_M = 5000;
  }

  int N = inf.readInt(1, max_N, "N");
  inf.readSpace();
  int M = inf.readInt(1, max_M, "M");
  inf.readSpace();
  int Q = inf.readInt(1, max_Q, "Q");
  inf.readEoln();

  if ((strcmp(subtask_name, "perfect-point-update") == 0) || (strcmp(subtask_name, "perfect") == 0)) {
    ensuref(__builtin_popcount(M) == 1, "M is not power of two");
    ensuref(M == N + 1, "M is not equal to N + 1");
  }

  std::vector<int> P = inf.readInts(N+M, -1, N+M-1, "P");
  inf.readEoln();

  std::vector<int> child_count(N+M, 0);
  
  ensuref(P[0] == -1, "P[0] != -1");
  for (int i = 1 ; i < N+M ; i++) {
    ensuref(0 <= P[i] && P[i] < i, "Invalid P[i]");
    child_count[P[i]]++;
  }
  for (int i = 0 ; i < N ; i++) {
    ensuref(child_count[i] > 0, "Should be internal node but no children");
  }
  for (int i = N ; i < N+M ; i++) {
    ensuref(child_count[i] == 0, "Should be leaf but has children");
  }

  if ((strcmp(subtask_name, "perfect-point-update") == 0) || (strcmp(subtask_name, "perfect") == 0)) {
    for (int i = 1 ; i < N+M ; i++) {
      ensuref(P[i] == (i-1)/2, "Not a complete binary tree");
    }
  } else if ((strcmp(subtask_name, "binary") == 0) || (strcmp(subtask_name, "brute-binary") == 0)) {
    for (int i = 0 ; i < N ; i++) {
      ensuref(child_count[i] == 2, "Not a binary tree");
    }
  }

  std::vector<int> A = inf.readInts(M, 0, 1, "A");
  inf.readEoln();

  for (int i = 0 ; i < Q ; i++) {
    int L = inf.readInt(N, N+M-1, "L");
    inf.readSpace();

    int max_R = strcmp(subtask_name, "perfect-point-update") == 0 ? L : N+M-1;
    inf.readInt(L, max_R, "R");
    inf.readEoln();
  }

  inf.readEof();

  return 0;
}
