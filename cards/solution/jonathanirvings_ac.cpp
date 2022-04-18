#include "cards.h"

#include <bits/stdc++.h>
using namespace std;

typedef unsigned int uint;

struct PermutationUtil {
  PermutationUtil() {}

  PermutationUtil(int _N) : N(_N) {
    if (N > 0) {
      vector<int> permutation(N);
      iota(permutation.begin(), permutation.end(), 0);
      do {
        permutation_to_index[permutation] = permutations.size();
        permutations.push_back(permutation);
      } while (next_permutation(permutation.begin(), permutation.end()));
    }
  }

  int get_index(const vector<int>& P) {
    return N == 0 ? 0 : permutation_to_index[P];
  }

  template <typename T>
  int get_index_sparse(const vector<T>& P) {
    vector<int> sorted_P = P;
    sort(sorted_P.begin(), sorted_P.end());
    map<T, int> index;
    for (uint i = 0; i < sorted_P.size(); ++i) {
      index[sorted_P[i]] = i;
    }
    vector<int> P_int(P.size());
    for (uint i = 0; i < P.size(); ++i) {
      P_int[i] = index[P[i]];
    }
    return get_index(P_int);
  }

  vector<int> get_permutation(int index) {
    return N == 0 ? vector<int>({}) : permutations[index];
  }

  int N;
  vector<vector<int>> permutations;
  map<vector<int>, int> permutation_to_index;
};

PermutationUtil permutation_util;
int K;
int number;
int suit;
vector<pair<int, int>> deck;

void init(int N, int _K) {
  K = _K;
  suit = K - 1;
  number = (N + suit - 1) / suit;
  permutation_util = PermutationUtil(K - 2);
  for (int i = 0; i < number; ++i) {
    for (int j = 0; j < suit; ++j) {
      deck.push_back(make_pair(i, j));
    }
  }
}

int get_distance_number(int a, int b) {
  return (b - a + number) % number;
}

void init_assistant(int N, int _K) {
  init(N, _K);
}

vector<int> choose_cards(vector<int> cards) {
  for (uint i = 0; i < cards.size(); ++i) {
    --cards[i];
  }
  for (uint i = 0; i < cards.size(); ++i) {
    for (uint j = i + 1; j < cards.size(); ++j) {
      if (deck[cards[i]].second == deck[cards[j]].second) {
        vector<int> sorted_remaining_cards = cards;
        sorted_remaining_cards.erase(sorted_remaining_cards.begin() + j);
        sorted_remaining_cards.erase(sorted_remaining_cards.begin() + i);
        sort(sorted_remaining_cards.begin(), sorted_remaining_cards.end());
        
        int number_i = deck[cards[i]].first;
        int number_j = deck[cards[j]].first;
        if (get_distance_number(number_i, number_j) >
            get_distance_number(number_j, number_i)) {
          swap(i, j);
          swap(number_i, number_j);
        }

        vector<int> answer;
        vector<int> permutation = permutation_util.get_permutation(
            get_distance_number(number_i, number_j) - 1);
        for (uint k = 0; k < permutation.size(); ++k) {
          answer.push_back(sorted_remaining_cards[permutation[k]] + 1);
        }
        answer.push_back(cards[i] + 1);
        return answer;
      }
    }
  }
  assert(false);
}

void init_magician(int N, int _K) {
  init(N, _K);
}

int find_discarded_card(vector<int> cards) {
  for (uint i = 0; i < cards.size(); ++i) {
    --cards[i];
  }
  int base_card = cards.back();
  cards.pop_back();
  int index = permutation_util.get_index_sparse(cards);

  return (base_card + (index + 1) * suit) % deck.size() + 1;
}
