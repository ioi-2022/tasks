#include "testlib.h"

#include <cstring>

constexpr int kMaxN = 5000;

int main(int, char *argv[]) {
  registerValidation();
  std::string secret = inf.readLine();
  ensuref(secret == (std::string)"9bbd7c649d0a9529ee32e00a6b96ee2f93e5b138",
          "Secret not found!");

  const char* subtask_name = argv[1];

  int max_N = kMaxN;
  int subtask = 3;

  if (strcmp(subtask_name, "samples") == 0) {
    subtask = 0;
  } else if (strcmp(subtask_name, "linear") == 0) {
    max_N = 500;
    subtask = 1;
  } else if (strcmp(subtask_name, "square-root") == 0) {
    max_N = 500;
    subtask = 2;
  }

  inf.readInt(2, max_N, "N");
  inf.readSpace();
  inf.readInt(subtask, subtask, "subtask");
  inf.readEoln();

  inf.readEof();

  return 0;
}
