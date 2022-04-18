#include "cards.h"

#include <bits/stdc++.h>

using namespace std;

int K;
int suit_size;

vector<vector<int>> permutations;

void init(int _K) {
  K = _K;
  suit_size = 1;
  for (int i = 1; i <= K - 2; ++i) suit_size *= i;
  suit_size = suit_size * 2 + 1;

  vector<int> perm(K - 2);
  iota(perm.begin(), perm.end(), 0);
  do {
    permutations.push_back(perm);
  } while (next_permutation(perm.begin(), perm.end()));
}

void init_assistant(int, int _K) { init(_K); }

void init_magician(int, int _K) { init(_K); }

vector<int> choose_cards(vector<int> cards) {
  vector<int> suits[K - 1];
  for (int &card : cards) {
    --card;
    suits[card / suit_size].push_back(card % suit_size);
  }

  int discarded_card = -1, pivot = -1, delta = -1;
  for (int i = 0; i < K - 1; ++i) {
    if (suits[i].size() < 2) continue;

    suits[i].push_back(suits[i][0]);
    for (int j = 1; j < static_cast<int>(suits[i].size()); ++j) {
      delta = (suits[i][j] - suits[i][j - 1] + suit_size) % suit_size;
      if (delta <= suit_size / 2) {
        discarded_card = suits[i][j] + suit_size * i;
        pivot = suits[i][j - 1] + suit_size * i;
        break;
      }
    }

    if (discarded_card != -1) break;
  }

  assert(discarded_card >= 0);

  vector<int> perm_card;
  for (int card : cards) {
    if (card == discarded_card || card == pivot) continue;
    perm_card.push_back(card);
  }

  vector<int> chosen_cards;
  chosen_cards.push_back(pivot + 1);
  for (int i = 0; i < K - 2; ++i) {
    chosen_cards.push_back(perm_card[permutations[delta - 1][i]] + 1);
  }

  return chosen_cards;
}

int find_discarded_card(vector<int> cards) {
  vector<int> card_order(cards.begin() + 1, cards.end());
  sort(card_order.begin(), card_order.end());

  vector<int> perm;
  for (int i = 1; i < static_cast<int>(cards.size()); ++i) {
    perm.push_back(
        lower_bound(card_order.begin(), card_order.end(), cards[i])
        - card_order.begin()
    );
  }

  int suit_num = (cards[0] - 1) / suit_size;
  int delta = lower_bound(permutations.begin(), permutations.end(), perm) - permutations.begin() + 1;
  return ((cards[0] - 1) + delta) % suit_size + suit_num * suit_size + 1;
}
