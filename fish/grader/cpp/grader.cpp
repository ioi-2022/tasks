#include "fish.h"

#include <cassert>
#include <cstdio>

// BEGIN SECRET
#include <string>
// END SECRET
#include <vector>

int main() {
  // BEGIN SECRET
  {
    std::string in_secret = "f785163bfcb92ce6ac387bba5d2f29a0e0f37f19";
    std::string out_secret = "938f2698235a9ff1d1d91e23381b68bec7bed102";
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
  int N, M;
  assert(2 == scanf("%d %d", &N, &M));

  std::vector<int> X(M), Y(M), W(M);
  for (int i = 0; i < M; ++i) {
    assert(3 == scanf("%d %d %d", &X[i], &Y[i], &W[i]));
  }

  long long result = max_weights(N, M, X, Y, W);
  // BEGIN SECRET
  {
    std::string out_secret = "938f2698235a9ff1d1d91e23381b68bec7bed102";
    printf("%s\n", out_secret.c_str());
    printf("OK\n");
  }
  // END SECRET
  printf("%lld\n", result);
  return 0;
}
