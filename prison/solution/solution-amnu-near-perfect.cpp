#include "prison.h"
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> ans;

void dnc(int P,int A,int B,int LL,int RR, int L,int R) {
  for (int i = L; i <= R; i++) ans[P][i] = A*3+B;
  for (int i = LL; i <= L; i++) ans[A*3+B][i] = -1 - ans[A*3+B][0];
  for (int i = R; i <= RR; i++) ans[A*3+B][i] = -2 + ans[A*3+B][0];
  L++; R--;
  if (R - L < 0) return;
  dnc(A*3+B, A+1, 1, L-1, R+1, L, (L+L+R)/3);
  dnc(A*3+B, A+1, 2, L-1, R+1, (L+L+R)/3+1, (L+R+R)/3);
  dnc(A*3+B, A+1, 3, L-1, R+1, (L+R+R)/3+1, R);
}

vector<vector<int>> devise_strategy(int N) {
  for (int i = 0; i <= 21; i++) {
    ans.push_back(vector<int>(N+1,0));
    if ((i + 2) % 6 < 3) ans[i][0] = 1;
  }
  dnc(0, -1, 3, 1, N, 1, N);
  return ans;
}
