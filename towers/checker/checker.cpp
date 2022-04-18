#include "testlib.h"

#include <string>

int main(int argc, char * argv[]) {
  registerChecker("towers", argc, argv);

  std::string output_secret = "07cd724d595507c6bd737a863fa1a319a887084b";

  readBothSecrets(output_secret);
  readBothGraderResults();

  compareRemainingLines();
}
