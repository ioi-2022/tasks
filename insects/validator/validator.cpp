#include "testlib.h"

#include <cstring>

const int kMaxN = 2000;
const int kMaxT = 1'000'000'000;

int main(int, char *argv[]) {
  registerValidation();

  const char* subtask_name = argv[1];

  int max_N = kMaxN;
  int max_is_partial = 1;

  if (strcmp(subtask_name, "samples") == 0) {
    max_is_partial = 0;
  } else if (strcmp(subtask_name, "quadratic") == 0) {
    max_N = 200;
    max_is_partial = 0;
  } else if (strcmp(subtask_name, "subquadratic") == 0) {
    max_N = 1000;
    max_is_partial = 0;
  }

  int N = inf.readInt(2, max_N, "N");
  inf.readSpace();
  inf.readInt(0, max_is_partial, "is_partial");
  inf.readEoln();
  inf.readInts(N, 0, kMaxT, "T");
  inf.readEoln();

  inf.readEof();

  return 0;
}
