#include "testlib.h"

#include <bitset>
#include <string>
#include <utility>
#include <vector>

constexpr int kMaxN = 5000;

std::bitset<kMaxN+1> added[kMaxN+1];

std::vector<std::pair<int, int>> generateTestcases(int N) {
  std::vector<std::pair<int, int>> testcases;
  constexpr int kMaxTestcases = 200'000;
  if (N * (N - 1) <= kMaxTestcases) {
    for (int i = 1; i <= N; ++i) {
      for (int j = i + 1; j <= N; ++j) {
        testcases.emplace_back(i, j);
        testcases.emplace_back(j, i);
      }
    }
    return testcases;
  }

  for (int i = 1; i <= N; ++i)
    added[i].reset();
  auto add = [&](int A, int B) {
    if (A == B || added[A][B]) return;
    added[A][B] = 1;
    testcases.emplace_back(A, B);
  };

  // ~ 6 * 2 * 5000 = 60k testcases
  constexpr int kLocalNum = 6;
  for (int i = 1; i <= N; ++i) {
    for (int j = i + 1; j <= std::min(i + kLocalNum, N); ++j) {
      add(i, j);
      add(j, i);
    }
  }

  // ~ 7 * 2 * 5000 = 70k testcases
  constexpr int kGroupNum = 7;
  const int kGroupSize = (N + kGroupNum - 1) / kGroupNum;
  for (int i = 1; i <= N; ++i) {
    for (int j = 1; j <= N; j += kGroupSize) {
      add(i, rnd.next(j, std::min(j+kGroupSize-1, N)));
      add(rnd.next(j, std::min(j+kGroupSize-1, N)), i);
    }
  }

  // ~ 50 * 50 * 2 = 5000 testcases
  constexpr int kEndPoints = 50;
  for (int i = 1; i <= std::min(kEndPoints, N); ++i) {
    for (int j = std::max(1, N - kEndPoints + 1); j <= N; ++j) {
      add(i, j);
      add(j, i);
    }
  }

  // full random
  int remaining = kMaxTestcases - static_cast<int>(testcases.size());
  while (remaining-- > 0) {
    add(rnd.next(1, N), rnd.next(1, N));
  }

  return testcases;
}

int main(int argc, char * argv[]) {
  registerChecker("prison", argc, argv);

  std::string input_secret = "9bbd7c649d0a9529ee32e00a6b96ee2f93e5b138";
  std::string output_secret = "d42f2a420c2a91c29a00736354e52427eecfabae";

  readBothSecrets(output_secret);
  readBothGraderResults();

  inf.readSecret(input_secret);

  int N = inf.readInt();
  int subtask = inf.readInt();

  const std::vector<int> kLimits = {500, 500, 70, 60};
  int x = ouf.readInt(0, kLimits[subtask]);
  std::vector<std::vector<int>> strategy(x + 1);
  for (int i = 0; i <= x; ++i) {
    strategy[i].resize(N + 1);
    strategy[i][0] = ouf.readInt(0, 1);
    for (int j = 1; j <= N; ++j) {
      strategy[i][j] = ouf.readInt(-2, x);
    }
  }

  if (!ouf.seekEof()) {
    quitf(_wa, "Extra output found");
  }
  std::vector<int> visited(x+1, -1);
  int counter = 0;
  auto simulate = [&](int A, int B) {
    ++counter;
    int expected = A < B ? -1 : -2;
    int whiteboard = 0;
    while (visited[whiteboard] != counter) {
      visited[whiteboard] = counter;
      int check = strategy[whiteboard][0];
      whiteboard = strategy[whiteboard][check == 0 ? A : B];
      if (whiteboard < 0) {
        return whiteboard == expected;
      }
    }
    return false;
  };

  for (auto [A, B] : generateTestcases(N)) {
    if (!simulate(A, B)) {
      quitf(_wa, "Strategy failed for N=%d, A=%d, B=%d", N, A, B);
    }
  }

  if (subtask != 3 || x <= 20) {
    quit(_ok);
  } else {
    double score = 90.0;
    if (x > 60) {
      score = 0;
    } else if (x >= 40) {
      score = 20;
    } else if (x >= 26) {
      score = 25 + 1.5 * (40 - x);
    } else if (x == 25) {
      score = 50;
    } else if (x == 24) {
      score = 55;
    } else if (x == 23) {
      score = 62;
    } else if (x == 22) {
      score = 70;
    } else if (x == 21) {
      score = 80;
    }
    score /= 90.0;
    quitp(score);
  }
}
