#include "testlib.h"

#include <string>

int main(int argc, char * argv[]) {
  registerChecker("circuit", argc, argv);

  std::string output_secret = "4faa78c22ee35083e49b65601698a1bd98a87a0f";

  readBothSecrets(output_secret);
  readBothGraderResults();

  compareRemainingLines();
}
