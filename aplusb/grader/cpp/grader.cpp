#include "aplusb.h"

#include <cassert>
#include <cstdio>
// BEGIN SECRET

#include <string>
// END SECRET

int main() {
  // BEGIN SECRET
  {
    std::string in_secret = "ini-adalah-masukan-rahasia";
    std::string out_secret = "ini-adalah-keluaran-rahasia";
    char secret[1000];
    assert(1 == scanf("%s", secret));
    if (std::string(secret) != in_secret) {
      printf("%s\n", out_secret.c_str());
      printf("SV\n");
      fclose(stdout);
      return 0;
    }
  }
  // END SECRET
  int A, B;
  assert(2 == scanf("%d %d", &A, &B));

  int result = add_two_numbers(A, B);
  // BEGIN SECRET
  {
    std::string out_secret = "ini-adalah-keluaran-rahasia";
    printf("%s\n", out_secret.c_str());
    printf("OK\n");
  }
  // END SECRET
  printf("%d\n", result);
  return 0;
}
