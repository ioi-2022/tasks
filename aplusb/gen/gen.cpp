#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

void bZero(int maxA) {
  printf("%d 0\n", rnd.next(0, maxA));
}

void random(int maxA, int maxB) {
  printf("%d %d\n", rnd.next(0, maxA), rnd.next(0, maxB));
}

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 0);

  if (strcmp(argv[1], "b-zero") == 0) {
    int maxA = atoi(argv[2]);
    bZero(maxA);
  } else if (strcmp(argv[1], "random") == 0) {
    int maxA = atoi(argv[2]);
    int maxB = atoi(argv[3]);
    random(maxA, maxB);
  }
}
