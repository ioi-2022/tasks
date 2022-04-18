#include "testlib.h"

#include <cstring>

#include <algorithm>
#include <string>
#include <vector>

const int kMaxN = 100'000;
const int kMaxQ = 100'000;
const int kMaxH = 1'000'000'000;

int main(int, char *argv[]) {
  registerValidation();
  std::string secret = inf.readLine();
  ensuref(secret == (std::string)"557c2e17ba69b458cd8202c239ce25526cdad3b5",
          "Secret not found!");

  const char* subtask_name = argv[1];

  int max_N = kMaxN, max_Q = kMaxQ, max_D = kMaxH;

  if (strcmp(subtask_name, "quadratic") == 0) {
    max_Q = 1;
    max_N = 2000;
  } else if (strcmp(subtask_name, "linear") == 0) {
    max_Q = 1;
  } else if (strcmp(subtask_name, "d-equals-one") == 0) {
    max_D = 1;
  }

  int N = inf.readInt(1, max_N, "N");
  inf.readSpace();
  int Q = inf.readInt(1, max_Q, "Q");
  inf.readEoln();

  std::vector<int> H = inf.readInts(N, 1, kMaxH, "H");
  inf.readEoln();

  if (strcmp(subtask_name, "bitonic") == 0) {
    int mx_idx = max_element(H.begin(), H.end()) - H.begin();

    for (int i = 0 ; i < mx_idx ; i++) {
      ensuref(H[i] < H[i+1], "First part of H is not bitonic");
    }
    for (int i = mx_idx ; i+1 < N ; i++) {
      ensuref(H[i] > H[i+1], "Second part of H is not bitonic");
    }
  }

  std::sort(H.begin(), H.end());
  ensuref(std::unique(H.begin(), H.end()) == H.end(), "H is not unique");

  int last_D = -1;
  for (int i = 0; i < Q; ++i) {
    int L = inf.readInt(0, N - 1, "L");
    inf.readSpace();
    int R = inf.readInt(L, N - 1, "R");
    inf.readSpace();
    int D = inf.readInt(1, max_D, "D");
    inf.readEoln();

    if (i > 0 && strcmp(subtask_name, "constant-d") == 0) {
      ensuref(last_D == D, "D is not constant");
    }
    if (strcmp(subtask_name, "full-interval") == 0) {
      ensuref(L == 0 && R == N-1, "L R not full interval");
    }

    last_D = D;
  }
  inf.readEof();

  return 0;
}
