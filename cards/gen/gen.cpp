#include "testlib.h"

#include <bits/stdc++.h>

using namespace std;

void generate(int n, int k, int q) {
  printf("%d %d %d\n", n, k, q);

  vector<bool> chosen(n, false);
  for (int i = 0; i < q; ++i) {
    vector<int> cards;
    while (static_cast<int>(cards.size()) != k) {
      int card = rnd.next(n);
      if (chosen[card]) continue;
      chosen[card] = true;
      cards.push_back(card);
    }

    sort(cards.begin(), cards.end());

    for (int i = 0; i < static_cast<int>(cards.size()); ++i) {
      printf("%d%c", cards[i] + 1, " \n"[i + 1 == static_cast<int>(cards.size())]);
    }

    for (int card : cards) chosen[card] = false;
  }
}

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 0);
  int N = atoi(argv[1]);
  int K = atoi(argv[2]);
  int Q = atoi(argv[3]);
  generate(N, K, Q);
  return 0;
}
