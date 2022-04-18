#include "testlib.h"

#include <bits/stdc++.h>
using namespace std;

int chooseRandom(vector<int> &v) {
  return v[rnd.next(v.size())];
}

int popRandom(vector<int> &v) {
  int idx = rnd.next(v.size());
  int ret = v[idx];
  v[idx] = v.back();
  v.pop_back();
  return ret;
}

struct Test {
  std::vector<int> P;
  std::vector<int> A;
  std::vector<pair<int, int>> queries;
  bool doNotReorder;

  int N, M;
  bool normalized;

  Test() {
    P.clear(); A.clear(); queries.clear();
    doNotReorder = normalized = false;
    N = M = 0;
  }

  // renumber such that all internal nodes are 0 .. N-1 and N .. M-1 are leaves
  void normalize() {
    normalized = true;

    vector<vector<int>> adj(P.size());
    for (int i = 1; i < static_cast<int>(P.size()); ++i) {
      adj[P[i]].push_back(i);
    }

    N = 0, M = 0;
    for (int i = 0; i < static_cast<int>(P.size()); ++i) {
      if (static_cast<int>(adj[i].size()) == 0) {
        ++M;
      } else {
        ++N;
      }
    }

    assert(static_cast<int>(P.size()) == N + M);

    if (doNotReorder) return;

    vector<int> reorder(N + M);

    int internalIdx = 0, leafIdx = N;

    vector<int> nodes = {0};
    while (!nodes.empty()) {
      int u = popRandom(nodes);
      if (static_cast<int>(adj[u].size()) == 0) {
        reorder[u] = leafIdx++;
      } else {
        reorder[u] = internalIdx++;
      }
      for (int v : adj[u]) {
        nodes.push_back(v);
      }
    }

    assert(internalIdx == N);
    assert(leafIdx == N + M);

    vector<int> newP(N + M);
    newP[0] = -1;
    for (int i = 1; i < N + M; ++i) {
      assert(reorder[P[i]] < N);
      newP[reorder[i]] = reorder[P[i]];
    }
    P = newP;
  }

  void print() {
    assert(normalized);
    printf("%d %d %d\n", N, M, static_cast<int>(queries.size()));

    for (int i = 0; i < N + M; ++i) {
      if (i > 0) {
        printf(" ");
      }
      printf("%d", P[i]);
    }
    printf("\n");

    for (int j = 0; j < M; ++j) {
      if (j > 0) {
        printf(" ");
      }
      printf("%d", A[j]);
    }
    printf("\n");

    for (auto [l, r] : queries) {
      printf("%d %d\n", l, r);
    }
  }
};

vector<int> randomBinaryVector(int N, int ratioOne, int ratioZero) {
  vector<int> ret(N);
  for (int i = 0; i < N; ++i) {
    if (rnd.next(ratioOne + ratioZero) < ratioOne) {
      ret[i] = 1;
    } else {
      ret[i] = 0;
    }
  }
  return ret;
}

vector<int> alternateBitVector(int N) {
  vector<int> ret(N);
  for (int i = 1; i < N; i += 2) {
    ret[i] = 1;
  }
  return ret;
}

vector<pair<int, int>> randomQueries(int N, int M, int Q) {
  vector<pair<int, int>> queries(Q);
  for (int k = 0; k < Q; ++k) {
    queries[k] = {rnd.next(M) + N, rnd.next(M) + N};
    if (queries[k].first > queries[k].second) {
      swap(queries[k].first, queries[k].second);
    }
  }
  return queries;
}

vector<pair<int, int>> largeQueries(int N, int M, int Q) {
  vector<pair<int, int>> queries(Q);
  for (int k = 0; k < Q; ++k) {
    queries[k] = {rnd.next(max(0, M - 10), M - 1) + N, rnd.next(min(M, 10)) + N};
    if (queries[k].first > queries[k].second) {
      swap(queries[k].first, queries[k].second);
    }
  }
  return queries;
}

vector<pair<int, int>> pointQueries(int N, int M, int Q) {
  vector<int> points(M);
  iota(points.begin(), points.end(), N);
  while (static_cast<int>(points.size()) < Q) {
    points.push_back((points.back() - N + 1) % M + N);
  }
  shuffle(points.begin(), points.end());
  points.resize(Q);
  vector<pair<int, int>> ret(Q);
  for (int k = 0; k < Q; ++k) {
    ret[k] = {points[k], points[k]};
  }
  return ret;
}

Test randomCase(int N, int M) {
  Test T;

  vector<int> internals = {0}, leaves = {1};
  T.P = {-1, 0};
  T.P.reserve(N + M);

  --N, --M;
  auto addLeaf = [&]() {
    T.P.push_back(chooseRandom(internals));
    --M;
  };
  auto addInternal = [&]() {
    T.P.push_back(popRandom(leaves));
    internals.push_back(T.P.back());
    --N;
  };
  while (N + M > 0) {
    int newLeaf = T.P.size();
    if (N == 0) {
      addLeaf();
    } else if (M == 0) {
      addInternal();
    } else if (rnd.next(2)) {
      addLeaf();
    } else {
      addInternal();
    }
    leaves.push_back(newLeaf);
  }

  return T;
}

Test maxDegreeCase(int size, int degree) {
  Test T;
  vector<int> degrees(size);
  vector<int> nodes = {0};
  T.P = vector<int>(size, -1);
  for (int i = 1; i < size; ++i) {
    T.P[i] = popRandom(nodes);
    if (++degrees[T.P[i]] < degree) {
      nodes.push_back(T.P[i]);
    }
    nodes.push_back(i);
  }
  return T;
}

Test exactDegreeCase(int N, int degree) {
  Test T;
  vector<int> degrees(N);
  vector<int> nodes = {0};
  int M = N * degree + 1 - N;
  T.P = vector<int>(N + M, -1);
  for (int i = 1; i < N + M; ++i) {
    T.P[i] = popRandom(nodes);
    if (++degrees[T.P[i]] < degree) {
      nodes.push_back(T.P[i]);
    }
    if (i < N) {
      nodes.push_back(i);
    }
  }
  return T;
}

Test completeBinaryCase(int size) {
  Test T;
  T.P = std::vector<int>(size, -1);
  for (int i = 1; i < size; ++i) {
    T.P[i] = (i - 1) / 2;
  }
  T.doNotReorder = true;
  return T;
}

Test caterpillarCase(int size) {
  Test T;
  T.P = std::vector<int>(size, -1);
  for (int i = 1; i < size; ++i) {
    if (i % 2 == 1) {
      T.P[i] = i - 1;
    } else {
      T.P[i] = i - 2;
    }
  }
  return T;
}

Test broomCase(int size) {
  Test T;
  T.P = std::vector<int>(size, -1);
  for (int i = 1; i < size / 2; ++i) {
    T.P[i] = i - 1;
  }
  for (int i = size / 2; i < size; ++i) {
    T.P[i] = size / 2 - 1;
  }
  return T;
}

Test tallCase(int size) {
  Test T;
  T.P = std::vector<int>(size, -1);
  for (int i = 1; i < size; ++i) {
    T.P[i] = rnd.next(max(0, i - 10), i - 1);
  }
  return T;
}

Test minDegreeCase(int size, int degree) {
  Test T;
  std::vector<bool> leaves(size, true);
  T.P = std::vector<int>(size, -1);
  int sz = 1;
  while (sz < size) {
    int u = rnd.next(sz);
    if (leaves[u]) {
      if (sz + degree > size) continue;
      leaves[u] = false;
      for (int i = 0; i < degree; ++i) {
        T.P[sz++] = u;
      }
    } else {
      T.P[sz++] = u;
    }
  }
  return T;
}

// Internal nodes have degree 1, 2, ..., maxDegree
Test distinctDegreesCase(int maxDegree) {
  Test T;
  T.P = {-1};
  vector<int> degrees(maxDegree);
  iota(degrees.begin(), degrees.end(), 1);

  int size = 0;
  vector<int> leaves = {0};
  while (!degrees.empty()) {
    int deg = popRandom(degrees);
    int p = popRandom(leaves);
    for (int i = 0; i < deg; ++i) {
      T.P.push_back(p);
      leaves.push_back(size++);
    }
  }
  return T;
}

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 0);
  Test T;
  int argIdx = 1;
  auto getNextRnd = [&]() {
    int ret = rnd.next(atoi(argv[argIdx]), atoi(argv[argIdx + 1]));
    argIdx += 2;
    return ret;
  };
  auto getNextInt = [&]() {
    return atoi(argv[argIdx++]);
  };
  auto getNextStr = [&]() {
    return string(argv[argIdx++]);
  };

  // Gen tree
  string tree = getNextStr();
  if (tree == "random-tree") {
    int N = getNextRnd();
    int M = getNextRnd();
    T = randomCase(N, M);
  } else if (tree == "max-degree-tree") {
    int size = getNextRnd();
    int degree = getNextInt();
    T = maxDegreeCase(size, degree);
  } else if (tree == "exact-degree-tree") {
    int N = getNextRnd();
    int degree = getNextInt();
    T = exactDegreeCase(N, degree);
  } else if (tree == "complete-binary-tree") {
    int size = getNextRnd();
    if (size % 2 == 0) --size;
    T = completeBinaryCase(size);
  } else if (tree == "caterpillar-tree") {
    int size = getNextRnd();
    T = caterpillarCase(size);
  } else if (tree == "broom-tree") {
    int size = getNextRnd();
    T = broomCase(size);
  } else if (tree == "tall-tree") {
    int size = getNextRnd();
    T = tallCase(size);
  } else if (tree == "min-degree-tree") {
    int size = getNextRnd();
    int degree = getNextInt();
    T = minDegreeCase(size, degree);
  } else if (tree == "distinct-degrees-tree") {
    int degree = getNextInt();
    T = distinctDegreesCase(degree);
  }

  T.normalize();

  // Gen array
  string arr = getNextStr();
  if (arr == "random-array") {
    int ratioOne = getNextInt();
    int ratioZero = getNextInt();
    T.A = randomBinaryVector(T.M, ratioOne, ratioZero);
  } else if (arr == "alternate-array") {
    T.A = alternateBitVector(T.M);
  }

  // Gen queries
  string queries = getNextStr();
  if (queries == "random-queries") {
    int Q = getNextRnd();
    T.queries = randomQueries(T.N, T.M, Q);
  } else if (queries == "large-queries") {
    int Q = getNextRnd();
    T.queries = largeQueries(T.N, T.M, Q);
  } else if (queries == "point-queries") {
    int Q = getNextRnd();
    T.queries = pointQueries(T.N, T.M, Q);
  }

  T.print();
  return 0;
}
