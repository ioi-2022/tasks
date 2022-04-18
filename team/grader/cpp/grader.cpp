#include "team.h"

#include <cassert>
#include <cstdio>

// BEGIN SECRET
#include <string>
// END SECRET
#include <vector>

int main() {
  // BEGIN SECRET
  {
    std::string in_secret = "243fee4b5311df05c33f71fb5383801a8316dcbf";
    std::string out_secret = "d2796822e56534d4568aa0ea80c43f3df8ef7476";
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
  int N, K;
  assert(2 == scanf("%d %d", &N, &K));

  std::vector<int> L(N);
  for (int i = 0; i < N; ++i) {
    assert(1 == scanf("%d", &L[i]));
  }

  int result = maximum_teams(N, K, L);
  // BEGIN SECRET
  {
    std::string out_secret = "d2796822e56534d4568aa0ea80c43f3df8ef7476";
    printf("%s\n", out_secret.c_str());
    printf("OK\n");
  }
  // END SECRET
  printf("%d\n", result);
  return 0;
}
