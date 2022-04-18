#include "cards.h"

#include <bits/stdc++.h>

int N;

void init_assistant(int _N, int) { N = _N; }

void init_magician(int _N, int) { N = _N; }

std::vector<int> choose_cards(std::vector<int> cards) {
  for (int i = 0; i < static_cast<int>(cards.size()); ++i) {
    if (cards[i] % N + 1 == cards[(i + 1) % cards.size()]) {
      return { cards[i] };
    }
  }
  return {};
}

int find_discarded_card(std::vector<int> cards) {
  return cards[0] % N + 1;
}
