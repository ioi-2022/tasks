#include "testlib.h"

#include <string>

int main(int argc, char * argv[]) {
  registerChecker("hoax", argc, argv);

  std::string output_secret = "e82d52fbc433a28e4a35ad586541c1b2265e19e3";

  readBothSecrets(output_secret);
  readBothGraderResults();

  compareRemainingLines();
}
