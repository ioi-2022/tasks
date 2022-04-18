#include "islands.h"

#include <cassert>
#include <cstdio>

// BEGIN SECRET
#include <string>
// END SECRET
#include <variant>
#include <vector>

int main() {
  // BEGIN SECRET
  {
    std::string in_secret = "4e5150f5a11b21d9ac3b44f2259e7a3452343ad4";
    std::string out_secret = "91041670623a69783795ed82bda33426101ce311";
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

  std::vector<int> U(M), V(M);
  for (int i = 0; i < M; ++i) {
    assert(2 == scanf("%d %d", &U[i], &V[i]));
  }

  std::variant<bool, std::vector<int>> result = find_journey(N, M, U, V);
  // BEGIN SECRET
  {
    std::string out_secret = "91041670623a69783795ed82bda33426101ce311";
    printf("%s\n", out_secret.c_str());
    printf("OK\n");
  }
  // END SECRET
  if (result.index() == 0) {
    printf("0\n");
    if (std::get<bool>(result)) {
      printf("1\n");
    } else {
      printf("0\n");
    }
  } else {
    printf("1\n");
    std::vector<int> &canoes = std::get<std::vector<int>>(result);
    // BEGIN SECRET
    {
      if (canoes.size() > 2000000) {
        canoes = {};
      }
    }
    // END SECRET
    printf("%d\n", static_cast<int>(canoes.size()));
    for (int i = 0; i < static_cast<int>(canoes.size()); ++i) {
      if (i > 0) {
        printf(" ");
      }
      printf("%d", canoes[i]);
    }
    printf("\n");
  }
  return 0;
}
