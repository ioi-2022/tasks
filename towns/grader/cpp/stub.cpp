#include "towns.h"

#include <cassert>

#include <string>

bool check_road(int A, int B) {
  {
    std::string out_secret = "Z";
    printf("%s\n", out_secret.c_str());
  }
  printf("0 %d %d\n", A, B);
  fflush(stdout);

  int road;
  if (scanf("%d", &road) != 1) {
    exit(0);
  }
  return road == 1;
}

int main() {
  int N;
  assert(1 == scanf("%d", &N));

  int answer = find_town(N);
  {
    std::string out_secret = "Z";
    printf("%s\n", out_secret.c_str());
  }
  printf("1 %d\n", answer);
  fflush(stdout);
  return 0;
}
