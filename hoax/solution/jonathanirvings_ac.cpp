#include "hoax.h"

#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> adj;
vector<int> inside;
vector<int> answer;

const int UNVISITED = 0;
const int VISITED = 1;

void dfs(int u) {
  if (answer[u] == VISITED) {
    return;
  }
  answer[u] = VISITED;
  inside.push_back(u);
  for (int v : adj[u]) {
    dfs(v);
  }
}

void init(int N, int S, std::vector<int> T,
          std::vector<std::vector<int>> A, std::vector<std::vector<int>> B) {
  adj.resize(N);
  answer.resize(N, UNVISITED);

  vector<int> hours;
  unordered_map<int, vector<int>> in, out;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < T[i]; ++j) {
      hours.push_back(A[i][j]);
      hours.push_back(B[i][j]);
      in[A[i][j]].push_back(i);
      out[B[i][j]].push_back(i);
    }
  }

  sort(hours.begin(), hours.end());
  hours.erase(unique(hours.begin(), hours.end()), hours.end());

  vector<bool> isOnline(N, false);
  stack<int> onlineUsers;

  for (int h : hours) {
    for (int user : in[h]) {
      while (!onlineUsers.empty()) {
        if (!isOnline[onlineUsers.top()]) {
          onlineUsers.pop();
        } else {
          break;
        }
      }
      if (!onlineUsers.empty()) {
        adj[onlineUsers.top()].push_back(user);
        adj[user].push_back(onlineUsers.top());
      }
      isOnline[user] = true;
      onlineUsers.push(user);
    }
    for (int user : out[h]) {
      isOnline[user] = false;
    }
  }

  for (int i = 0; i < N; ++i) {
    if (answer[i] == UNVISITED) {
      inside.clear();
      dfs(i);
      for (int user : inside) {
        answer[user] = inside.size();
      }
    }
  }
}

int count_users(int P) {
  return answer[P];
}
