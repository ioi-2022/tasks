#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// Only the upper-right part of R is read, the rest is mirrored
void printR(vector<vector<int>> R) {
  for (int i = 0; i < static_cast<int>(R.size()); ++i) {
    for (int j = 0; j < i; ++j) {
      printf("%c", 1 - R[j][i] + '0');
    }
    printf("0");
    for (int j = i + 1; j < static_cast<int>(R[i].size()); ++j) {
      printf("%c", R[i][j] + '0');
    }
    printf("\n");
  }
}

vector<vector<int>> randomR(int N) {
  vector R(N, vector(N, 0));
  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j < N; ++j) {
      R[i][j] = rnd.next(2);
    }
  }
  return R;
}

vector<vector<int>> hasKOutgoingR(int N, int K, int pivot=-1) {
  if (pivot == -1) {
    pivot = rnd.next(N);
  }
  auto R = randomR(N);
  for (int i = 0; i < N; ++i) {
    if (i < pivot) {
      R[i][pivot] = 1;
    } else if (i > pivot) {
      R[pivot][i] = 0;
    }
  }

  vector<int> outgoings(N - 1);
  for (int i = 0; i < pivot; ++i) {
    outgoings[i] = i;
  }
  for (int i = pivot + 1; i < N; ++i) {
    outgoings[i - 1] = i;
  }
  shuffle(outgoings.begin(), outgoings.end());

  outgoings.resize(K);
  for (int u : outgoings) {
    if (u < pivot) {
      R[u][pivot] ^= 1;
    } else {
      R[pivot][u] ^= 1;
    }
  }
  return R;
}

vector<vector<int>> dagR(int N) {
  vector R(N, vector(N, 0));
  vector<int> perm(N);
  iota(perm.begin(), perm.end(), 0);
  shuffle(perm.begin(), perm.end());

  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j < N; ++j) {
      int u = perm[i], v = perm[j];
      R[u][v] = 1;
      R[v][u] = 0;
    }
  }
  return R;
}

vector<vector<int>> cycleR(int N, int cycles) {
  auto R = randomR(N);

  vector<int> order(N);
  iota(order.begin(), order.end(), 0);
  shuffle(order.begin(), order.end());

  for (int i = 1; i <= cycles; ++i) {
    for (int j = 0; j < N; ++j) {
      int u = order[j];
      int v = order[(j + i) % N];
      if (u < v) {
        R[u][v] = 1;
      } else {
        R[v][u] = 0;
      }
    }
  }
  return R;
}

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 0);

  int N = rnd.next(atoi(argv[1]), atoi(argv[2]));
  int subtask = atoi(argv[3]);
  printf("%d %d\n", N, subtask);

  const char* strategy = argv[4];
  printf("%s\n", strategy);

  if (strcmp(strategy, "manual") == 0) {
    printR(randomR(N));
  }

  if (strcmp(strategy, "must-sure") == 0) {
    const char* graph = argv[5];
    if (strcmp(graph, "random") == 0) {
      printR(randomR(N));
    } else if (strcmp(graph, "has-k-outgoing") == 0) {
      int K = atoi(argv[6]);
      printR(hasKOutgoingR(N, K));
    } else if (strcmp(graph, "dag") == 0) {
      printR(dagR(N));
    } else if (strcmp(graph, "cycle") == 0) {
      int cycles = atoi(argv[6]);
      printR(cycleR(N, cycles));
    }
  }
}
