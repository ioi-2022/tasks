#include <csignal>
#include <cstdarg>
#include <cstdio>

#include <algorithm>
#include <random>
#include <string>
#include <vector>

/******************************** Begin testlib-similar material ********************************/

#ifdef _MSC_VER
#   define NORETURN __declspec(noreturn)
#elif defined __GNUC__
#   define NORETURN __attribute__ ((noreturn))
#else
#   define NORETURN
#endif


static char __testlib_format_buffer[16777216];

#define FMT_TO_RESULT(fmt, cstr, result)  std::string result;                              \
            va_list ap;                                                                    \
            va_start(ap, fmt);                                                             \
            vsnprintf(__testlib_format_buffer, sizeof(__testlib_format_buffer), cstr, ap); \
            va_end(ap);                                                                    \
            __testlib_format_buffer[sizeof(__testlib_format_buffer) - 1] = 0;              \
            result = std::string(__testlib_format_buffer);                                 \


typedef std::string TResult;
const TResult _ok = "Output is correct";
const TResult _wa = "Output isn't correct";
const TResult _fail = "Judge Failure; Contact staff!";
const TResult _points = "Output is partially correct";

NORETURN void quit(TResult result, std::string message = "") {
	fprintf(stdout, "%d\n", ((result==_ok) ? 1 : 0));
	fprintf(stderr, "%s\n", result.c_str());
	if (!message.empty())
		fprintf(stderr, "%s\n", message.c_str());
	exit(0);
}

NORETURN void quitp(double points, std::string message = "") {
	fprintf(stdout, "%.12lf\n", points);
	fprintf(stderr, "%s\n", _points.c_str());
	if (!message.empty())
		fprintf(stderr, "%s\n", message.c_str());
	exit(0);
}

#ifdef __GNUC__
__attribute__ ((format (printf, 2, 3)))
#endif
NORETURN void quitf(TResult result, const char* fmt, ...) {
	FMT_TO_RESULT(fmt, fmt, message);
	quit(result, message);
}

#ifdef __GNUC__
__attribute__ ((format (printf, 2, 3)))
#endif
NORETURN void quitpf(double points, const char* fmt, ...) {
	FMT_TO_RESULT(fmt, fmt, message);
	quitp(points, message);
}

/********************************* End testlib-similar material *********************************/

FILE *grader1in, *grader1out, *grader2in, *grader2out;
std::mt19937 rng;

#ifdef __GNUC__
__attribute__ ((format (printf, 3, 4)))
#endif
NORETURN inline void die(TResult result, bool sendDie, const char* fmt, ...) {
	if (sendDie) {
	  fprintf(grader2out, "-1\n");
	  fflush(grader2out);
	}
	FMT_TO_RESULT(fmt, fmt, message);
	quit(result, message);
}

inline FILE* openFile(const char* name, const char* mode) {
	FILE* file = fopen(name, mode);
	if (!file) {
		quitf(_fail, "Could not open file '%s' with mode '%s'", name, mode);
	}
	return file;
}

int main(int argc, char *argv[]) {
  if (argc < 5) {
    quit(_fail, "Insufficient number of args for manager of 'cards'");
  }

  {
    // Keep alive on broken pipes
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, NULL);
  }

  // Must be in this order
  grader1out = openFile(argv[2], "a");
  grader1in = openFile(argv[1], "r");
  grader2out = openFile(argv[4], "a");
  grader2in = openFile(argv[3], "r");

  // Read input
  int N, K, Q;
  std::vector<std::vector<int>> cards;
  if (scanf("%d %d %d", &N, &K, &Q) != 3) quit(_fail, "Could not read N K Q");
  cards = std::vector<std::vector<int>>(Q);
  for (int i = 0; i < Q; ++i) {
    cards[i] = std::vector<int>(K);
    for (int j = 0; j < K; ++j) {
      if (scanf("%d", &cards[i][j]) != 1) quit(_fail, "Could not read card");
    }
  }
  fclose(stdin);

  // Send cards to first grader
  std::vector<std::vector<int>> chosen_cards(Q);
  fprintf(grader1out, "%d %d %d\n", N, K, Q);
  fflush(grader1out);
  for (int i = 0; i < Q; ++i) {
    for (int j = 0; j < K; ++j) {
      fprintf(grader1out, "%d\n", cards[i][j]);
    }
    fflush(grader1out);

    int chosen_cards_size;
    if (fscanf(grader1in, "%d", &chosen_cards_size) != 1) {
      die(_wa, true, "Could not read chosen cards size from first grader.");
    }
    if (chosen_cards_size != K - 1) {
      die(_wa, true, "Invalid length of chosen cards size. K=%d, len=%d", K, chosen_cards_size);
    }
    chosen_cards[i] = std::vector<int>(chosen_cards_size);
    for (int j = 0; j < chosen_cards_size; ++j) {
      if (fscanf(grader1in, "%d", &chosen_cards[i][j]) != 1) {
        die(_fail, true, "Could not read array elements from first grader. element #%d", j);
      }
    }
  }
  fclose(grader1out);
  fclose(grader1in);

  // Validate chosen cards and shuffle queries
  std::vector<std::vector<int>> queries(Q);
  for (int i = 0; i < Q; ++i) {
    // Find discarded card
    std::vector<bool> chosen(K, false);
    for (int chosen_card : chosen_cards[i]) {
      std::vector<int>::iterator it = std::lower_bound(cards[i].begin(), cards[i].end(), chosen_card);
      if (it == cards[i].end()) {
        die(_wa, true, "Invalid chosen card");
      }
      int chosen_idx = std::distance(cards[i].begin(), it);
      if (chosen[chosen_idx]) {
        die(_wa, true, "Chosen cards contain duplicate");
      }
      chosen[chosen_idx] = true;
    }

    int discardedCard = -1;
    for (int j = 0; j < K; ++j) {
      if (!chosen[j]) discardedCard = cards[i][j];
    }

    queries[i] = chosen_cards[i];
    queries[i].push_back(discardedCard);
  }
  shuffle(queries.begin(), queries.end(), rng);

  // Send queries to second grader
  fprintf(grader2out, "0\n%d %d %d\n", N, K, Q);
  fflush(grader2out);
  bool fail = false;
  for (int i = 0; i < Q; ++i) {
    for (int j = 0; j < K - 1; ++j) {
      fprintf(grader2out, "%d\n", queries[i][j]);
    }
    fflush(grader2out);

    int guessCard;
    if (fscanf(grader2in, "%d", &guessCard) != 1) {
      die(_wa, false, "Could not read query response from second grader");
    }

    if (guessCard != queries[i].back()) {
      fail = true;
    }
  }
  fclose(grader2in);
  fclose(grader2out);

  // Verdict
  if (fail) quitf(_wa, "Wrong response");
  quit(_ok);
  return 0;
}
