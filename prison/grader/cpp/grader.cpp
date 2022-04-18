#include "prison.h"

#include <cassert>
#include <cstdio>

#include <string>
#include <vector>

static void invalid_strategy(std::string message) {
  {
    std::string out_secret = "d42f2a420c2a91c29a00736354e52427eecfabae";
    printf("%s\n", out_secret.c_str());
    printf("WA\n");
  }
  printf("%s\n", message.c_str());
  exit(0);
}

int main() {
  {
    std::string in_secret = "9bbd7c649d0a9529ee32e00a6b96ee2f93e5b138";
    std::string out_secret = "d42f2a420c2a91c29a00736354e52427eecfabae";
    char secret[1000];
    assert(1 == scanf("%s", secret));
    if (std::string(secret) != in_secret) {
      printf("%s\n", out_secret.c_str());
      printf("SV\n");
      fclose(stdout);
      return 0;
    }
  }
  int N, subtask;
  assert(2 == scanf("%d %d", &N, &subtask));

  std::vector<std::vector<int>> strategy = devise_strategy(N);

  if (strategy.size() == 0) {
    invalid_strategy("s is an empty array");
  }
  int x = strategy.size() - 1;
  const std::vector<int> kLimits = {500, 500, 70, 60};
  if (x > kLimits[subtask]) {
    invalid_strategy("The value of x exceeds the limit in this subtask");
  }
  for (int i = 0; i <= x; ++i) {
    if (static_cast<int>(strategy[i].size()) != N + 1) {
      invalid_strategy("s[i] contains incorrect length");
    }
    if (strategy[i][0] < 0 || strategy[i][0] > 1) {
      invalid_strategy("First element of s[i] is non-binary");
    }
    for (int j = 1; j <= N; ++j) {
      if (strategy[i][j] < -2 || strategy[i][j] > x) {
        invalid_strategy("s[i][j] contains incorrect value");
      }
    }
  }

  {
    std::string out_secret = "d42f2a420c2a91c29a00736354e52427eecfabae";
    printf("%s\n", out_secret.c_str());
    printf("OK\n");
  }

  printf("%d\n", x);
  for (int i = 0; i <= x; ++i) {
    for (int j = 0; j <= N; ++j) {
      printf("%d%c", strategy[i][j], " \n"[j == N]);
    }
  }
}
