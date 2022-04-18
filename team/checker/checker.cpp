#include "testlib.h"

#include <string>

int main(int argc, char * argv[]) {
  registerChecker("team", argc, argv);

  std::string output_secret = "d2796822e56534d4568aa0ea80c43f3df8ef7476";

  readBothSecrets(output_secret);
  readBothGraderResults();

  compareRemainingLines();
}
