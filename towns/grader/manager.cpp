#include "testlib.h"

#include <cassert>
#include <csignal>
#include <cstdio>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

inline double roundToTwoDp(double score) {
  return round(score * 100) / 100;
}

class Strategy {
 public:
  Strategy(int N)
      : N(N), R(N, std::vector<int>(N, -1)),
        outdeg(N), unknown(N, N - 1), nodesWithOutdeg(N) {
    for (int i = 0; i < N; ++i) {
      nodesWithOutdeg[0].insert(i);
    }
  }

  bool check_road(int A, int B) {
    if (R[A][B] != -1) {
      return R[A][B];
    }
    R[A][B] = check_road_impl(A, B);
    R[B][A] = 1 - R[A][B];

    int outdegNode = R[A][B] ? A : B;
    nodesWithOutdeg[outdeg[outdegNode]].erase(outdegNode);
    ++outdeg[outdegNode];
    nodesWithOutdeg[outdeg[outdegNode]].insert(outdegNode);

    --unknown[A];
    --unknown[B];
    return R[A][B];
  }

  bool is_correct(int town) {
    if (town == -1) {
      for (int i = 0; i < N; ++i) {
        if (outdeg[i] + unknown[i] <= 1) {
          return false;
        }
      }
      if (all_of(outdeg.begin(), outdeg.end(), [] (int o) {
        return o > 1;
      })) {
        return true;
      }
      return is_correct_impl(town);
    }
    if (outdeg[town] + unknown[town] <= 1) {
      return true;
    }
    if (outdeg[town] > 1) {
      return false;
    }
    return is_correct_impl(town);
  }

  virtual ~Strategy() = default;

 protected:
  int N;
  std::vector<std::vector<int>> R;
  std::vector<int> outdeg;
  std::vector<int> unknown;
  std::vector<std::set<int>> nodesWithOutdeg;

 private:
  virtual bool check_road_impl(int A, int B) = 0;
  virtual bool is_correct_impl(int town) = 0;
};

class ManualStrategy : public Strategy {
 public:
  ManualStrategy(int N) : Strategy(N) {
    manualR.resize(N);
    for (int i = 0; i < N; ++i) {
      char buffer[N + 1];
      assert(1 == scanf("%s", buffer));
      manualR[i] = buffer;
    }
  }

  bool check_road_impl(int A, int B) override {
    return manualR[A][B] == '1';
  }

  bool is_correct_impl(int town) override {
    if (town == -1) {
      return std::all_of(manualR.begin(), manualR.end(), [] (std::string s) {
        return std::count(s.begin(), s.end(), '1') > 1;
      });
    } else {
      return std::count(manualR[town].begin(), manualR[town].end(), '1') <= 1;
    }
  }

 private:
  std::vector<std::string> manualR;
};

class MustSureStrategy : public Strategy {
 public:
  MustSureStrategy(int N) : Strategy(N) {
    manualR.resize(N);
    for (int i = 0; i < N; ++i) {
      char buffer[N + 1];
      assert(1 == scanf("%s", buffer));
      manualR[i] = buffer;
    }
  }

  bool check_road_impl(int A, int B) override {
    return manualR[A][B] == '1';
  }

  bool is_correct_impl(int) override {
    return false;
  }

 private:
  std::vector<std::string> manualR;
};

class GreedyOutdegStrategy : public Strategy {
 public:
  GreedyOutdegStrategy(int N, bool greedyMax)
     : Strategy(N), greedyMax(greedyMax) {}

  bool check_road_impl(int A, int B) override {
    return (greedyMax != (outdeg[A] < outdeg[B]));
  }

  bool is_correct_impl(int) override {
    return false;
  }

 private:
  bool greedyMax;
};

class NearFoundStrategy : public Strategy {
 public:
  NearFoundStrategy(int N) : Strategy(N) {}

  bool check_road_impl(int A, int B) override {
    if (outdeg[A] <= 1 && outdeg[B] <= 1) {
      return true;
    }

    if (outdeg[A] <= 1 && outdeg[B] > 1) {
      if (outdeg[A] + unknown[A] - 1 <= 1) {
        return true;
      }
      return false;
    }

    if (outdeg[A] > 1 && outdeg[B] <= 1) {
      if (outdeg[B] + unknown[B] - 1 <= 1) {
        return false;
      }
      return true;
    }

    return true;
  }

  bool is_correct_impl(int) override {
    // Answer should always be -1
    return false;
  }
};

class MaintainCycleStrategy : public Strategy {
 public:
  MaintainCycleStrategy(int N, bool deterministic=false)
      : Strategy(N), deterministic(deterministic) {
    adj.resize(N);
    cyclePart.resize(N);
  }

  bool check_road_impl(int A, int B) override {
    if (unknown[B] - 1 + outdeg[B] <= 1) {
      addEdge(B, A);
      // Cycle may be removed
      rebuildCycle();
      return false;
    }

    if (unknown[A] - 1 + outdeg[A] <= 1) {
      addEdge(A, B);
      // Cycle may be removed
      rebuildCycle();
      return true;
    }

    if (outdeg[A] == 0 && outdeg[B] == 0) {
      return randomAddEdge(A, B);
    }

    if (outdeg[A] == 0 && outdeg[B] == 1) {
      addEdge(A, B);
      // A new cycle may be formed
      rebuildCycle();
      return true;
    }

    if (outdeg[A] == 1 && outdeg[B] == 0) {
      addEdge(B, A);
      // A new cycle may be formed
      rebuildCycle();
      return false;
    }

    if (outdeg[A] == 1 && outdeg[B] == 1) {
      if (cyclePart[A] && cyclePart[B]) {
        bool ret = randomAddEdge(A, B);
        // Cycle becomes smaller
        rebuildCycle();
        return ret;
      }

      if (cyclePart[A] && !cyclePart[B]) {
        addEdge(B, A);
        return false;
      }

      if (!cyclePart[A] && cyclePart[B]) {
        addEdge(A, B);
        return true;
      }

      if (!cyclePart[A] && !cyclePart[B]) {
        return randomAddEdge(A, B);
      }

      assert(false);
    }

    if (outdeg[A] > 1 && outdeg[B] <= 1) {
      addEdge(A, B);
      return true;
    }

    if (outdeg[A] <= 1 && outdeg[B] > 1) {
      addEdge(B, A);
      return false;
    }

    assert(outdeg[A] > 1 && outdeg[B] > 1);
    return randomAddEdge(A, B);
  }

  bool is_correct_impl(int) override {
    // if your solution was <= 4N, then `town` should be -1
    // otherwise, your solution must be at least 5N
    return false;
  }

 private:
  bool deterministic;
  std::vector<std::vector<int>> adj;
  std::vector<bool> cyclePart;

  void addEdge(int u, int v) {
    adj[u].push_back(v);
  }

  bool randomAddEdge(int u, int v) {
    if (deterministic || rnd.next(2)) {
      addEdge(u, v);
      return true;
    }
    addEdge(v, u);
    return false;
  }

  // O(N)
  void rebuildCycle() {
    cyclePart.assign(N, false);
    std::vector<bool> vis(N, false);
    std::vector<bool> visTour(N, false);
    for (int i = 0; i < N; ++i) {
      if (vis[i] || adj[i].size() != 1) continue;

      int u = i;
      std::vector<int> tour;
      while (adj[u].size() == 1) {
        visTour[u] = true;
        tour.push_back(u);
        u = adj[u][0];

        if (vis[u]) {
          break;
        }

        if (visTour[u]) {
          // Found cycle
          while (tour.back() != u) {
            cyclePart[tour.back()] = true;
            vis[tour.back()] = true;
            visTour[tour.back()] = false;
            tour.pop_back();
          }
          cyclePart[u] = true;
          break;
        }
      }

      for (int u : tour) {
        vis[u] = true;
        visTour[u] = false;
      }
    }
  }
};

class DelayEliminationStrategy : public Strategy {
 public:
  DelayEliminationStrategy(int N) : Strategy(N) {}

  bool check_road_impl(int A, int B) override {
    if (outdeg[A] > 1 && outdeg[B] > 1) {
      return rnd.next(2);
    }
    if (outdeg[A] > 1) {
      return true;
    }
    if (outdeg[B] > 1) {
      return false;
    }
    if (outdeg[A] == 1 && outdeg[B] == 1) {
      return rnd.next(2);
    }
    if (outdeg[A] == 1) {
      return false;
    }
    if (outdeg[B] == 1) {
      return true;
    }
    if (outdeg[A] + unknown[A] <= 2 && outdeg[B] + unknown[B] <= 2) {
      return rnd.next(2);
    }
    if (outdeg[A] + unknown[A] <= 2) {
      return true;
    }
    if (outdeg[B] + unknown[B] <= 2) {
      return false;
    }
    return rnd.next(2);
  }

  bool is_correct_impl(int) override {
    return false;
  }
};

class MaintainAdjacentNodesStrategy : public Strategy {
 public:
  MaintainAdjacentNodesStrategy(int N) : Strategy(N), nextNode(N) {}

  bool check_road_impl(int A, int B) override {
    if (outdeg[A] + unknown[A] <= 2) {
      nextNode[A] = B;
      return true;
    }
    if (outdeg[B] + unknown[B] <= 2) {
      nextNode[B] = A;
      return false;
    }

    findRoot();
    findAdjacentNode();
    
    if (A == root) {
      if (B == adjacentNode.second) {
        nextNode[A] = B;
        return true;
      }
      nextNode[B] = A;
      return false;
    }
    if (B == root) {
      if (A == adjacentNode.first) {
        nextNode[B] = A;
        return false;
      }
      nextNode[A] = B;
      return true;
    }
    if (A == adjacentNode.first || A == adjacentNode.second) {
      nextNode[B] = A;
      return false;
    }
    if (B == adjacentNode.first || B == adjacentNode.second) {
      nextNode[A] = B;
      return true;
    }
    if (rnd.next(2)) {
      nextNode[B] = A;
      return false;
    } else {
      nextNode[A] = B;
      return true;
    }
  }

  bool is_correct_impl(int) override {
    return false;
  }

 private:
  void findRoot() {
    int prevRoot = root;
    root = -1;
    for (int i : nodesWithOutdeg[0]) {
      if (root == -1 || unknown[i] > unknown[root]) {
        root = i;
      }
    }
    if (root == -1) {
      root = prevRoot;
    }
  }

  void findAdjacentNode() {
    std::pair<int, int> prevAdjacentNode = adjacentNode;
    adjacentNode = std::make_pair(-1, -1);
    for (int i : nodesWithOutdeg[1]) {
      int j = nextNode[i];
      if (i == root || j == root) {
        continue;
      }
      if (outdeg[j] == 1) {
        if (adjacentNode == std::make_pair(-1, -1)
            || unknown[i] + unknown[j] >
               unknown[adjacentNode.first] + unknown[adjacentNode.second]) {
          adjacentNode = std::make_pair(i, j);
        }
      }
    }
    if (adjacentNode == std::make_pair(-1, -1)) {
      adjacentNode = prevAdjacentNode;
    }
  }

  int root = 0;
  std::vector<int> nextNode;
  std::pair<int, int> adjacentNode = std::make_pair(-1, -1);
};

class MaintainThreeNodesStrategy : public Strategy {
 public:
  MaintainThreeNodesStrategy(int N) : Strategy(N) {}

  bool check_road_impl(int A, int B) override {
    if (outdeg[A] + unknown[A] <= 2 && outdeg[B] + unknown[B] <= 2) {
      return rnd.next(2);
    }
    if (outdeg[A] + unknown[A] <= 2) {
      return true;
    }
    if (outdeg[B] + unknown[B] <= 2) {
      return false;
    }

    updateNodes();

    if (A == std::get<0>(nodes)) {
      return B == std::get<1>(nodes);
    }
    if (A == std::get<1>(nodes)) {
      return B == std::get<2>(nodes);
    }
    if (A == std::get<2>(nodes)) {
      return B == std::get<0>(nodes);
    }
    if (B == std::get<0>(nodes)
        || B == std::get<1>(nodes)
        || B == std::get<2>(nodes)) {
      return true;
    }
    return rnd.next(2);
  }

  bool is_correct_impl(int) override {
    return false;
  }

 private:
  void updateNodes() {
    if (nodesWithOutdeg[0].size() >= 3) {
      std::vector<int> nodesNoOutdeg(nodesWithOutdeg[0].begin(),
                                     nodesWithOutdeg[0].end());
      std::sort(nodesNoOutdeg.begin(), nodesNoOutdeg.end(), [&] (int u, int v) {
        return unknown[u] > unknown[v];
      });
      nodes = std::make_tuple(
          nodesNoOutdeg[0], nodesNoOutdeg[1], nodesNoOutdeg[2]);
    }
  }

  std::tuple<int, int, int> nodes = std::make_tuple(0, 1, 2);
};

inline FILE* openFile(const char* name, const char* mode) {
  FILE* file = fopen(name, mode);
  if (!file) {
    quitf(_fail, "Could not open file '%s' with mode '%s'", name, mode);
  }
  return file;
}

int main(int argc, char *argv[]) {
  testlibMode = _checker;
  ouf.mode = _output;

  if (argc < 3) {
    quit(_fail, "Insufficient number of args for manager of 'towns'");
  }

  {
    // Keep alive on broken pipes
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, NULL);
  }

  // Must be in this order
  FILE *fout = openFile(argv[2], "a");
  FILE *fin = openFile(argv[1], "r");

  int N;
  int subtask;
  assert(2 == scanf("%d %d", &N, &subtask));

  std::unique_ptr<Strategy> strategy;
  char buffer[100];
  assert(1 == scanf("%s", buffer));
  if (std::string(buffer) == "manual") {
    strategy.reset(new ManualStrategy(N));
  } else if (std::string(buffer) == "must-sure") {
    strategy.reset(new MustSureStrategy(N));
  } else if (std::string(buffer) == "greedy-outdeg-min") {
    strategy.reset(new GreedyOutdegStrategy(N, false));
  } else if (std::string(buffer) == "greedy-outdeg-max") {
    strategy.reset(new GreedyOutdegStrategy(N, true));
  } else if (std::string(buffer) == "near-found") {
    strategy.reset(new NearFoundStrategy(N));
  } else if (std::string(buffer) == "maintain-cycle") {
    strategy.reset(new MaintainCycleStrategy(N));
  } else if (std::string(buffer) == "maintain-cycle-deterministic") {
    strategy.reset(new MaintainCycleStrategy(N, true));
  } else if (std::string(buffer) == "delay-elimination") {
    strategy.reset(new DelayEliminationStrategy(N));
  } else if (std::string(buffer) == "maintain-adjacent-nodes") {
    strategy.reset(new MaintainAdjacentNodesStrategy(N));
  } else if (std::string(buffer) == "maintain-three-nodes") {
    strategy.reset(new MaintainThreeNodesStrategy(N));
  } else {
    assert(false);
  }

  fprintf(fout, "%d\n", N);
  fflush(fout);

  int Q = 0;
  while (true) {
    {
      std::string in_secret = "Z";
      char secret[100];
      if (fscanf(fin, "%5s", secret) != 1) {
        quit(_sv, "Could not read secret (possibly, an unexpected termination"
                  " of the program)");
      }
      if (std::string(secret) != in_secret) {
        quit(_sv, "Secret mismatch (possible tampering with the output)");
      }
    }

    int op;
    if (fscanf(fin, "%d", &op) != 1) {
      quit(_fail, "Could not read op.");
    }

    if (op == 0) {
      if (Q == 40000) {
        quit(_sv, "Too many queries.");
      }
      ++Q;

      int A, B;
      if (fscanf(fin, "%d %d", &A, &B) != 2) {
        quit(_fail, "Could not read query parameters.");
      }
      if (A == B || A < 0 || A >= N || B < 0 || B >= N) {
        quit(_sv, "Invalid query parameters.");
      }

      int road = strategy->check_road(A, B) ? 1 : 0;
      fprintf(fout, "%d\n", road);
      fflush(fout);
    } else if (op == 1) {
      int answer;
      if (fscanf(fin, "%d", &answer) != 1) {
        quit(_fail, "Could not read answer.");
      }

      if (answer < -1 || answer >= N) {
        quit(_wa, "Wrong answer.");
      }

      if (strategy->is_correct(answer)) {
        if (subtask == 1) {
          quit(_ok);
        } else if (subtask == 2) {
          if (Q <= 8000) {
            quit(_ok);
          } else if (Q <= 20000) {
            quitp(roundToTwoDp(90.0 - 70.0 * sqrt((Q - 8000) * 1.0 / 12000))
                  / 90.0);
          } else {
            quitp(20.0 / 90.0);
          }
        } else {
          assert(false);
        }
      } else {
        quit(_wa, "Wrong answer.");
      }
    } else {
      quit(_fail, "Invalid value of op.");
    }
  }
}
