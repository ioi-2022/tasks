#include "testlib.h"

#include <algorithm>
#include <string>
#include <vector>

bool validate_trip(std::vector<int>& canoes,
                   std::vector<int> U,
                   std::vector<int> V) {
  if (canoes.size() == 0)
    return 0;

  int curr_pos = 0;
  int prev = -1;

  int M = U.size();
  std::vector<int> used(M);

  for (auto x: canoes) {
    // can't use the same canoe twice
    if (x == prev)
      return 0;
    prev = x;

    // can't use illegal canoe
    if (x < 0 || x >= M) 
      return 0;

    if (U[x] != curr_pos) {
      return 0;
    }

    curr_pos = V[x];

    used[x]++;
    std::swap(U[x], V[x]);
  }

  // each canoe has to be used even times
  for (int i=0;i<M;i++) {
    if (used[i] % 2) {
      return 0;
    }
  }

  // guarantee to return at 0
  return 1;
}

// return:
// 0 = has no solution
// 1 = has a solution but no path. Invalid path will also return this result
// 2 = has a solution and a valid path
int read_and_validate_input(InStream& stream, 
                            const std::vector<int>& U, 
                            const std::vector<int>& V) {
  int type = stream.readInt(0, 1);
  if (type == 0) {
    int result  = stream.readInt(0, 1);

    if (!stream.seekEof())
      stream.quitf(_wa, "Extra output found");

    return result;
  } else {
    std::vector<int> canoes;
    int C = stream.readInt(0, 2000000);
    for (int i = 0; i < C; i++) {
      // no need to check the range yet, will be checked later
      canoes.push_back(stream.readInt());
    }

    if (!stream.seekEof())
      stream.quitf(_wa, "Extra output found");

    if (validate_trip(canoes, U, V))
      return 2;
    else
      return 1;
  }

}

int main(int argc, char * argv[]) {
  registerChecker("islands", argc, argv);

  std::string output_secret = "91041670623a69783795ed82bda33426101ce311";

  readBothSecrets(output_secret);
  readBothGraderResults();

  inf.readSecret("4e5150f5a11b21d9ac3b44f2259e7a3452343ad4");
  inf.readInt();
  int M = inf.readInt();

  std::vector<int> U(M);
  std::vector<int> V(M);
  for (int i = 0; i < M; i++) {
    U[i] = inf.readInt();
    V[i] = inf.readInt();
  }

  int ret_sub = read_and_validate_input(ouf, U, V);
  int ret_judge = read_and_validate_input(ans, U, V);

  if (ret_judge == 0) {
    if (ret_sub == 0)
      quit(_ok);
    else if (ret_sub == 1)
      quit(_wa);
    else
      quit(_fail, "Contestant found a valid sol while judge didn't");
  } else if (ret_judge == 2) {
    if (ret_sub == 0)
      quit(_wa);
    else if (ret_sub == 1)
      quitp(0.35);
    else
      quit(_ok);
  } else {
    quit(_fail, "Judge found an invalid sol");
  }
}
