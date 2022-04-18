#include "cards.h"

#include <cstdio>

#include <string>
#include <vector>

static std::string grader_name = "grader";
static void errorGrader(std::string reason) {
  fprintf(stderr, "%s error\n", grader_name.c_str());
  fprintf(stderr, "%s\n", reason.c_str());
  exit(0);
}

static void graderA() {
  grader_name = "grader A";

  int N, K, Q;
  if (scanf("%d %d %d", &N, &K, &Q) != 3) {
    errorGrader("Could not read N K Q");
  }

  init_assistant(N, K);

  for (int i = 0; i < Q; ++i) {
    std::vector<int> cards(K);
    for (int j = 0; j < K; ++j) {
      if (scanf("%d", &cards[j]) != 1) {
        errorGrader("Could not read card");
      }
    }

    std::vector<int> chosen_cards = choose_cards(cards);
    printf("%d\n", static_cast<int>(chosen_cards.size()));
    if (static_cast<int>(chosen_cards.size()) != K - 1) {
      fflush(stdout);
      exit(0);
    }
    for (int card : chosen_cards) {
      printf("%d\n", card);
    }
    fflush(stdout);
  }
}

static void graderB() {
  grader_name = "grader B";

  int flag;
  if (scanf("%d", &flag) != 1) {
    errorGrader("Could not read flag");
  }
  if (flag < 0) {  // error in grader A
    exit(0);
  }

  int N, K, Q;
  if (scanf("%d %d %d", &N, &K, &Q) != 3) {
    errorGrader("Could not read N K Q");
  }

  init_magician(N, K);

  for (int i = 0; i < Q; ++i) {
    std::vector<int> cards(K - 1);
    for (int j = 0; j < K - 1; ++j) {
      if (scanf("%d", &cards[j]) != 1) {
        errorGrader("could not read cards");
      }
      if (cards[j] < 0) {  // error in answers
        exit(0);
      }
    }

    printf("%d\n", find_discarded_card(cards));
    fflush(stdout);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    errorGrader("number of arguments isn't 4");
  }

  if (std::string(argv[1]) == "0") {  // grader A
    graderA();
  } else if (std::string(argv[1]) == "1") {  // grader B
    graderB();
  } else {
    errorGrader("unknown grader type " + std::string(argv[1]));
  }
  return 0;
}
