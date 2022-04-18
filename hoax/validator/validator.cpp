#include "testlib.h"

#include <numeric>
#include <set>
#include <vector>

constexpr int kMaxN = 200000;
constexpr int kMaxS = 1000000000;
constexpr int kMaxQ = 100000;
constexpr int kMaxSumT = 200000;

int main(int, char *argv[]) {
  registerValidation();
  std::string secret = inf.readLine();
  ensuref(secret == (std::string)"e13edc64b8b182df2e0d65c78699ec2375edb5cb",
          "Secret not found!");

  const char* subtask_name = argv[1];

  int maxN = kMaxN;
  int maxQ = kMaxQ;
  int maxS = kMaxS;
  int maxSumT = kMaxSumT;

  if (strcmp(subtask_name, "small-S") == 0) {
    maxN = maxS = maxQ = 50;
    maxSumT = 50;
  } else if (strcmp(subtask_name, "fourth") == 0) {
    maxN = maxQ = 50;
    maxSumT = 50;
  } else if (strcmp(subtask_name, "cubic") == 0) {
    maxN = maxQ = 300;
    maxSumT = 300;
  } else if (strcmp(subtask_name, "quadratic") == 0) {
    maxN = maxQ = 2000;
    maxSumT = 2000;
  }

  int N = inf.readInt(1, maxN, "N");
  inf.readSpace();
  int S = inf.readInt(1, maxS, "S");
  inf.readSpace();
  int Q = inf.readInt(1, maxQ, "Q");
  inf.readEoln();

  std::vector<int> T(N);
  for (int i = 0; i < N; ++i) {
    T[i] = inf.readInt(1, S, "T");
    int A = 0, B = 0;
    for (int j = 0; j < T[i]; ++j) {
      inf.readSpace();
      A = inf.readInt(B + 1, S, "A");
      inf.readSpace();
      B = inf.readInt(A, S, "B");
    }
    inf.readEoln();
  }

  ensuref(std::accumulate(T.begin(), T.end(), 0LL) <= maxSumT,
          "Sum of T exceeds the limit");

  std::set<int> P;
  for (int i = 0; i < Q; ++i) {
    P.insert(inf.readInt(0, N - 1, "P"));
    inf.readEoln();
  }
  ensuref(static_cast<int>(P.size()) == Q,
          "The values of P are not pairwise distinct");

  inf.readEof();
  return 0;
}
