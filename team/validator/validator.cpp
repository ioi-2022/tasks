#include "testlib.h"

#include <string>

constexpr int kMaxVal = 100000000;

int main(int, char *argv[]) {
  registerValidation();
  std::string secret = inf.readLine();
  ensuref(secret == (std::string)"243fee4b5311df05c33f71fb5383801a8316dcbf",
          "Secret not found!");

  const char* subtask_name = argv[1];
  int maxN = 100000;
  if (strcmp(subtask_name, "one-team") == 0) {
    maxN = 3;
  } else if (strcmp(subtask_name, "exponential") == 0) {
    maxN = 8;
  } else if (strcmp(subtask_name, "quadratic") == 0) {
    maxN = 1000;
  }

  int N = inf.readInt(1, maxN, "N");
  inf.readSpace();
  inf.readInt(1, kMaxVal, "K");
  inf.readEoln();
  
  inf.readInts(N, 1, kMaxVal, "L");
  inf.readEoln();

  inf.readEof();
  return 0;
}
