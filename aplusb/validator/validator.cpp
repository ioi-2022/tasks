#include "testlib.h"

const int kMaxVal = 100;

int main(int, char *argv[]) {
  registerValidation();
  std::string secret = inf.readLine();
  ensuref(secret == (std::string)"ini-adalah-masukan-rahasia",
          "Secret not found!");

  const char* subtask_name = argv[1];
  int maxA = (strcmp(subtask_name, "smallAB") == 0 ? 10 : kMaxVal);
  int maxB = (strcmp(subtask_name, "bZero") == 0 ? 0 : maxA);

  inf.readInt(0, maxA, "A");
  inf.readSpace();
  inf.readInt(0, maxB, "B");
  inf.readEoln();
  inf.readEof();
  return 0;
}
