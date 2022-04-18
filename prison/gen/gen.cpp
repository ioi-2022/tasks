#include "testlib.h"

#include <bits/stdc++.h>

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 0);

  int minN = atoi(argv[1]);
  int maxN = atoi(argv[2]);
  int subtask = atoi(argv[3]);
  printf("%d %d\n", rnd.next(minN, maxN), subtask);
  return 0;
}
