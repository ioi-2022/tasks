#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

vector<int> random(int N,int D) {
  vector<int> T(N);
  for (int i = 0; i < N; ++i) {
    T[i] = rnd.next(D);
  }
  return T;
}

vector<int> uniform(int N,int D) {
  vector<int> T(N);
  for (int i = 0; i < N; ++i) {
    T[i] = i % D;
  }
  
  shuffle(T.begin(), T.end());
  return T;
}

vector<int> linear(int N,int D) {
  vector<int> T;
  for (int i = 0; i < D; ++i) {
    for (int j = 0; i + j < D; ++j) {
      T.push_back(i);
    }
  }
  for (int i = 0; (int)T.size() < N; ++i) {
    T.push_back(i % D);
  }
  
  shuffle(T.begin(), T.end());
  return T;
}

vector<int> dominant(char* pos,int N,int D) {
  int L = N - D, R = 0;
  if (strcmp(pos, "right") == 0) {
    R = L;
    L = 0;
  }
  if (strcmp(pos, "both") == 0) {
    R = L / 2;
    L -= R;
  }
  
  vector<int> T;
  for (int i = 0; i < L; ++i) {
    T.push_back(D - 1);
  }
  for (int i = 0; i < D; ++i) {
    T.push_back(i);
  }
  for (int i = 0; i < R; ++i) {
    T.push_back(D - 1);
  }
  
  if (strcmp(pos, "shuffle") == 0) {
    shuffle(T.begin(), T.end());
  }
  return T;
}

vector<int> optimum(char* pos,int N,int D,int ans) {
  int L = ans, R = 0;
  if (strcmp(pos, "right") == 0) {
    R = L;
    L = 0;
  }
  if (strcmp(pos, "both") == 0) {
    R = L / 2;
    L -= R;
  }
  
  vector<int> T;
  for (int i = 0; i < L; ++i) {
    T.push_back(D - 1);
  }
  for (int i = 0; i < N - ans; ++i) {
    T.push_back(i % (D - 1));
  }
  for (int i = 0; i < R; ++i) {
    T.push_back(D - 1);
  }
  
  if (strcmp(pos, "shuffle") == 0) {
    shuffle(T.begin(), T.end());
  }
  return T;
}

vector<int> special(vector<int> f) {
  vector<int> T;
  for (int i = 0; i < (int)f.size(); ++i) {
    for (int j = 0; j < f[i]; ++j) {
      T.push_back(i);
    }
  }
  
  shuffle(T.begin(), T.end());
  return T;
}

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 0);

  int N = rnd.next(atoi(argv[1]), atoi(argv[2]));
  int partial = atoi(argv[3]);

  vector<int> T;
  if (strcmp(argv[4], "random") == 0) {
    T = random(N, atoi(argv[5]));
  }
  else if (strcmp(argv[4], "uniform") == 0) {
    T = uniform(N, atoi(argv[5]));
  }
  else if (strcmp(argv[4], "linear") == 0) {
    T = linear(N, atoi(argv[5]));
  }
  else if (strcmp(argv[4], "dominant") == 0) {
    T = dominant(argv[5], N, atoi(argv[6]));
  }
  else if (strcmp(argv[4], "optimum") == 0) {
    T = optimum(argv[5], N, atoi(argv[6]), atoi(argv[7]));
  }
  else if (strcmp(argv[4], "special") == 0) {
    if (atoi(argv[5]) == 1) {
      T = special(vector<int>({1,3,8,24,89,415}));
    }
    else if (atoi(argv[5]) == 2) {
      T = special(vector<int>({1,1,4,14,55,261,1498}));
    }
    N = T.size();
  }

  printf("%d %d\n", N, partial);
  for (int i = 0; i < N; ++i) {
    printf("%d%c", T[i], " \n"[i==N-1]);
  }
}
