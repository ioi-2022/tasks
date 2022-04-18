#include "testlib.h"

#include <string>

int main(int argc, char *argv[])
{
    registerChecker("fish", argc, argv);

    std::string output_secret = "938f2698235a9ff1d1d91e23381b68bec7bed102";

    readBothSecrets(output_secret);
    readBothGraderResults();

    compareRemainingLines();
}
