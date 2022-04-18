#include "testlib.h"

#include <bits/stdc++.h>
using namespace std;

void generateRandomStrictlyIncreasingVector(int size, int lo, int hi, vector<int> &A){
  assert(hi-lo+1 >= size);
  A.clear();
  int last = lo-1;
  for(int i = size-1; i >= 0; --i){
    int nextElem = rnd.next(last+1, hi-i);
    A.push_back(nextElem);
    last = nextElem;
  }
}

void generateRandomIntervals(int onlineTime, int lo, int hi,
                             vector<int> &AB){
  AB.clear();
  vector<int> A; A = vector<int>();
  generateRandomStrictlyIncreasingVector(onlineTime, lo, hi, A);
  for(int i = 0; i < onlineTime-1; ++i){
    int nextElem = rnd.next(A[i], A[i+1]-1);
    AB.push_back(A[i]);
    AB.push_back(nextElem);
  }
  AB.push_back(A[onlineTime-1]);
  AB.push_back(rnd.next(A[onlineTime-1], hi));
}

void generateRandomIntervalsForAll(int N, int S, int maxSumO,
                                   vector<int> &O,
                                   vector< vector<int> > &AB){
  assert(maxSumO >= N);
  AB.assign(N, vector<int>());
  // Generate O
  int remainingSum = maxSumO-N;
  for(int i = 0; i < N; ++i){
    int addTime = rnd.next(0, min(S-1, remainingSum)); // Making sure everyone gets at least 1 online time
    O.push_back(1 + addTime);
    remainingSum -= addTime;
  }
  // Generate A and B
  for(int i = 0; i < N; ++i){
    generateRandomIntervals(O[i], 1, S, AB[i]);
  }
}

void generateRandomQuery(int N, int Q, vector<int> &P){
  generateRandomStrictlyIncreasingVector(Q, 1, N, P);
}

void shufflePartition(
    vector<int> &O,
    vector< vector<int> > &AB){
  vector<int> shuffled;
  shuffled.resize(O.size());
  for(int i = 0; i < static_cast<int>(O.size()); ++i)
    shuffled[i] = i;
  shuffle(shuffled.begin(), shuffled.end());
  vector<int> newO;
  vector< vector<int> > newAB;
  for(int i = 0; i < static_cast<int>(O.size()); ++i){
    newO.push_back(O[shuffled[i]]);
    newAB.push_back(AB[shuffled[i]]);
  }
  O = newO;
  AB = newAB;
}

void generateChainingIntervals(int N, int &S,
                               vector<int> &O,
                               vector< vector<int> > &AB){
  S = N+1;
  O.assign(N, 1);
  AB.clear();
  for(int i = 0; i < N; ++i){
    AB.push_back(vector<int>({i+1, i+2}));
  }
  shufflePartition(O, AB);
}

void generateAllPeopleActive(int N, int S, int maxO,
                             vector<int> &O,
                             vector< vector<int> > &AB,
                             bool split){
  AB.assign(N, vector<int>());
  if(!split){
    O.assign(N, 1);
    for(int i = 0; i < N; ++i){
      AB[i] = vector<int>({1, S});
    }
  }else{
    int intervalNum = min(S, maxO/N);
    O.assign(N, intervalNum);
    for(int i = 0; i < N; ++i){
      for(int j = 0; j < intervalNum; ++j){
        int l = (S/intervalNum)*j + 1, r = (S/intervalNum)*(j+1);
        AB[i].push_back(l);
        AB[i].push_back(r);
      }
    }
  }
}

void generateIntervalsFromPartition(
    int N,
    int S,
    int partitionSize,
    vector<int> &O,
    vector< vector<int> > &AB){
  vector<int> partition; // Number of people in each partition

  O.assign(N, 1);
  partition.assign(partitionSize, 1);
  for(int i = 0; i < N-partitionSize; ++i){
    int nextToAdd = rnd.next(0, partitionSize-1);
    partition[nextToAdd]++;
  }

  // Determine ranges of each partition
  vector<int> range;
  generateRandomStrictlyIncreasingVector(2*partitionSize, 1, S, range);
  for(int i = 0; i < partitionSize; ++i){
    int l = range[2*i], r = range[2*i+1];
    int newL, newR, meetingPoint;
    newL = rnd.next(l, r);
    newR = rnd.next(newL, r);
    meetingPoint = rnd.next(newL, newR);
    AB.push_back({newL, newR});
    for(int j = 1; j < partition[i]; ++j){
      newL = rnd.next(l, meetingPoint);
      newR = rnd.next(meetingPoint, r);
      meetingPoint = rnd.next(newL, newR);
      AB.push_back({newL, newR});
    }
  }
  shufflePartition(O, AB);
}

void generateRandomSingleIntervalCase(int N, int S, int Q,
    vector<int> &O,
    vector< vector<int> > &AB,
    vector<int> &P){
  // Determine number of partitions
  int partitionSize = rnd.next(1, min(N, S/2));
  generateIntervalsFromPartition(N, S, partitionSize, O, AB);
  generateRandomQuery(N, Q, P);
}

void generateRandomMultipleIntervalCase(int N, int S, int Q,
    vector<int> &O,
    vector< vector<int> > &AB,
    vector<int> &P,
    int connectingUser=10){
  vector<int> partition;

  int partitionSize = rnd.next(connectingUser+1, min(N, S/2));
  O.assign(N, 1);
  partition.assign(partitionSize, 1);

  for(int i = 0; i < N-partitionSize; ++i){
    int nextToAdd = rnd.next(0, partitionSize-1);
    partition[nextToAdd]++;
  }

  // Determine ranges of each partition
  vector<int> range;
  generateRandomStrictlyIncreasingVector(2*partitionSize, 1, S, range);
  for(int i = 0; i < partitionSize; ++i){
    int l = range[2*i], r = range[2*i+1];
    int newL, newR, meetingPoint;
    newL = rnd.next(l, r);
    newR = rnd.next(newL, r);
    meetingPoint = rnd.next(newL, newR);
    if(partitionSize-i <= connectingUser){
      // Define first user as connecting user. Choose one partition to connect and add another interval
      int conn = rnd.next(0, static_cast<int>(AB.size()-1));
      int interL = rnd.next(AB[conn][0], AB[conn][1]);
      int interR = rnd.next(AB[conn][0], AB[conn][1]);
      if(interL > interR)
        swap(interL, interR);
      O[AB.size()]++;
      AB.push_back({interL, interR, newL, newR});
    }else{
      AB.push_back({newL, newR});
    }
    for(int j = 1; j < partition[i]; ++j){
      newL = rnd.next(l, meetingPoint);
      newR = rnd.next(meetingPoint, r);
      meetingPoint = rnd.next(newL, newR);
      AB.push_back({newL, newR});
    }
  }

  shufflePartition(O, AB);
  generateRandomQuery(N, Q, P);
}

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 0);

  int N = 0, S = 0, Q = 0;
  vector<int> O, P;
  vector<vector<int>> AB;

  int cur_index = 2;
  while (cur_index < argc) {
    if (strcmp(argv[cur_index], "N") == 0) {
      N = atoi(argv[++cur_index]);
    } else if (strcmp(argv[cur_index], "S") == 0) {
      S = atoi(argv[++cur_index]);
    } else if (strcmp(argv[cur_index], "Q") == 0) {
      Q = atoi(argv[++cur_index]);
    } else if (strcmp(argv[cur_index], "generateAllPeopleActive") == 0) {
      int maxO = atoi(argv[++cur_index]);
      bool split = strcmp(argv[++cur_index], "True") == 0;
      generateAllPeopleActive(N, S, maxO, O, AB, split);
    } else if (strcmp(argv[cur_index], "generateChainingIntervals") == 0) {
      generateChainingIntervals(N, S, O, AB);
    } else if (strcmp(argv[cur_index], "generateRandomIntervalsForAll") == 0) {
      int maxSumO = atoi(argv[++cur_index]);
      generateRandomIntervalsForAll(N, S, maxSumO, O, AB);
    } else if (strcmp(argv[cur_index], "generateIntervalsFromPartition") == 0) {
      int partitionSize = atoi(argv[++cur_index]);
      generateIntervalsFromPartition(N, S, partitionSize, O, AB);
    } else if (strcmp(argv[cur_index], "generateRandomQuery") == 0) {
      generateRandomQuery(N, Q, P);
    } else if (strcmp(argv[cur_index], "generateRandomMultipleIntervalCase") == 0) {
      int connectingUser = atoi(argv[++cur_index]);
      generateRandomMultipleIntervalCase(N, S, Q, O, AB, P, connectingUser);
    } else if (strcmp(argv[cur_index], "generateRandomSingleIntervalCase") == 0) {
      generateRandomSingleIntervalCase(N, S, Q, O, AB, P);
    } else {
      assert(false);
    }
    ++cur_index;
  }

  printf("%d %d %d\n", N, S, Q);
  for (int i = 0; i < N; ++i) {
    printf("%d", O[i]);
    for (int j = 0; j < 2 * O[i]; ++j) {
      printf(" %d", AB[i][j]);
    }
    printf("\n");
  }
  shuffle(P.begin(), P.end());
  for (int i = 0; i < Q; ++i) {
    printf("%d\n", P[i] - 1);
  }
}
