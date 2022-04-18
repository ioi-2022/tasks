#include "testlib.h"

#include <algorithm>
#include <string>
#include <vector>

int main(int, char *argv[]) {
  registerValidation();
  
  const char* subtask_name = argv[1];
  int maxN = (strcmp(subtask_name, "smallN") == 0 ? 250 : 2000);
  int subtask_index = (strcmp(subtask_name, "full") == 0 ? 2 : 1);
  
  int N = inf.readInt(3, maxN, "N");
  inf.readSpace();
  inf.readInt(subtask_index, subtask_index, "subtask_index");
  inf.readEoln();

  std::string strategy = inf.readLine();

  if (strategy == "manual" || strategy == "must-sure") {
    std::vector<std::string> R(N);
    for (int i = 0; i < N; ++i) {
      R[i] = inf.readLine("[0-1]*");
      ensuref(static_cast<int>(R[i].size()) == N, "Found len(R[i]) != N");
    }
    for (int i = 0; i < N; ++i) {
      ensuref(R[i][i] == '0', "Found R[i][i] != 0");
      for (int j = i + 1; j < N; ++j) {
        ensuref(R[i][j] != R[j][i], "Found R[i][j] = R[j][i]");
      }
    }
  }

  std::vector<std::string> strategies = {
    "manual", "must-sure", "greedy-outdeg-min", "greedy-outdeg-max",
    "near-found", "maintain-cycle", "maintain-cycle-deterministic",
    "delay-elimination", "maintain-adjacent-nodes", "maintain-three-nodes"
  };
  ensuref(std::find(strategies.begin(),
                    strategies.end(), strategy) != strategies.end(),
          "Strategy not found");
  inf.readEof();
  return 0;
}
