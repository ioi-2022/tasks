#include "testlib.h"
#include <bits/stdc++.h>
#include <random>

using namespace std;


void print_path(int N, vector<pair<int,int>> &paths, bool is_double=false, bool is_bidirectional=false) {
  int MULT = 1;
  if (is_double)
    MULT *= 2;
  if (is_bidirectional)
    MULT *= 2;


  // node-number shuffler
  std::vector<int> node_mapper;
  for (int i = 0; i < N; i++)
    node_mapper.push_back(i);

  // shuffle except node 0, since we always start at 0
  shuffle(node_mapper.begin() + 1, node_mapper.end()); 

  printf("%d %d\n", N, (int)paths.size() * MULT);

  for (auto x: paths) {
    printf("%d %d\n", node_mapper[x.first], node_mapper[x.second]);
    if (is_double)
      printf("%d %d\n", node_mapper[x.first], node_mapper[x.second]);
    if (is_bidirectional)
      printf("%d %d\n", node_mapper[x.second], node_mapper[x.first]);
  }
}


pair<int, int> get_useless_path(int used_nodes, int N, int start = 0) {
  // divide unused nodes further
  int leftover =  N - used_nodes;

  // useless path requires new unused nodes
  assert(leftover > 0);

  // 20% of unused nodes will be used as 'in' node
  int unused_innode_size = int(leftover / 5);
  int in_N = used_nodes + unused_innode_size;

  
  // add a path of used nodes to/from unused nodes
  int rnd_ch = rnd.next(0, 100);
  if (rnd_ch < 20 && start > 0) {
    // connect a random node back to one of the starting nodes.
    int u = rnd.next(start, used_nodes - 1);
    int v = rnd.next(0, start - 1);
    return {u, v};
  }
  else if (rnd_ch < 80 || unused_innode_size == 0) {
    // connect a random unused nodes to/from any nodes
    int u = rnd.next(used_nodes, N - 1);
    int v = u;
    while (v == u) v = rnd.next(0, N - 1);
    if (u < in_N || u < v) // to avoid a cycle
      return {u, v}; // in
    else
      return {v, u}; // out
  }  else {

    // add path between unused nodes
    // need at least 1 unused_innode
    int u = rnd.next(used_nodes, in_N - 1);
    int v = rnd.next(in_N, N - 1);
    return {u, v};
  }
}



// has solution
vector<pair<int,int>> gen_solution(int N, int M, int S) {
  int start = rnd.next(0, int(S / 20));
  int leftover = S - start;
  int Lpath = rnd.next(2, leftover - 2); // at minimum 2 nodes to make a loop
  int Rpath = leftover - Lpath;

  vector<pair<int,int>> paths;

  // path from 0 to start
  // [0 ... start - 1]
  for (int i = 0;i <  start; i++) {
    paths.push_back({i, i + 1});
  }

  // first loop
  // [start ... start + Lpath]
  for (int i = 0; i < Lpath - 1; i++) {
    paths.push_back({start + i, start + i + 1});
  }
  paths.push_back({start + Lpath - 1, start + rnd.next(0, Lpath - 2)});

  // second loop
  // [start + Lpath ... S]
  // connect the start first to this loop
  paths.push_back({start, start + Lpath});
  for (int i = 0; i < Rpath - 1; i++) {
    paths.push_back({start + Lpath + i, start + Lpath + i + 1});
  }
  // 2nd loop connect back:
  // if v <= start + Lpath, then it's connected to the first loop
  // else, connected to itself
  int v = rnd.next(start, S - 2);
  paths.push_back({S - 1, v});

  // used nodes: 0 ... S - 1
  // leftover nodes: S ... N - 1
  while ((int) paths.size() < M) {
    paths.push_back(get_useless_path(S, N, start));
  }

  return paths;
}

// has solution dag
vector<pair<int,int>> gen_solution_dag(int N, int M) {
  vector<pair<int,int>> paths;

  // make a random tree
  for (int i = 1; i < N; i++) {
    int u = rnd.next(0, i - 1);
    paths.push_back({u, i});
  }

  // construct DAG
  while ((int) paths.size() < M) {
    int u = rnd.next(0, N - 1);
    int v  = u;
    while (u == v) v = rnd.next(0, N - 1);
    if (u > v)
      swap(u, v);

    if ((int) paths.size() != M - 1)
      paths.push_back({u, v});
    else
      paths.push_back({v, u});
  }

  return paths;
}


vector<pair<int,int>> gen_circle(int N, int M, int S) {
  int start = rnd.next(0, int(S / 5));
    
  vector<pair<int,int>> paths;

  // make a straight path
  for (int i = 0;i <  S - 1; i++) {
    paths.push_back({i, i + 1});
  }
  //return back to start to make a loop
  paths.push_back({S - 1, start});

  // used nodes: 0 ... S - 1
  // leftover nodes: S ... N - 1
  while ((int) paths.size() < M) {
     paths.push_back(get_useless_path(S, N, start));
  }

  return paths;
}


vector<pair<int,int>> gen_line(int N, int M, int S) {
  vector<pair<int,int>> paths;

  // make a straight path
  for (int i = 0;i <  S - 1; i++) {
    paths.push_back({i, i + 1});
  }

  // used nodes: 0 ... S - 1
  // leftover nodes: S ... N - 1
  while ((int) paths.size() < M) {
     paths.push_back(get_useless_path(S, N, 0));
  }

  return paths;
}


vector<pair<int,int>> gen_complete(int N) {
  vector<pair<int,int>> paths;

  for (int i = 0; i < N; i++)
    for (int j = 0; j < i; j++) {
      // only add one way first, the opposite direction will be printed later
      paths.push_back({i, j});
    }

  return paths;
}


vector<pair<int,int>> gen_random(int N, int M) {
  vector<pair<int,int>> paths;

  for (int i = 0; i < M; i++) {
    int u = rnd.next(0, N - 1);
    int v  = u;
    while (u == v) v = rnd.next(0, N - 1);
    paths.push_back({u, v});
  }

  return paths;
}

vector<pair<int,int>> gen_dag(int N, int M) {
  vector<pair<int,int>> paths;

  for (int i = 0; i < M; i++) {
    int u = rnd.next(0, N - 1);
    int v  = u;
    while (u == v) v = rnd.next(0, N - 1);
    if (u < v)
      paths.push_back({u, v});
    else
      paths.push_back({v, u});
      
  }

  return paths;
}


// a single line with a cicle at the end, and a branching on the same node-pair
// 0 1
// 1 2
// ..
// K K+1
// K K+1 <- branching
// K+1 K+2
// ..
// N-2 N-1
// N-1 N-2 <- loop
// K is chosen randomly from the N nodes
vector<pair<int,int>> gen_tricky_sol_1(int N) {
  vector<pair<int,int>> paths;
  for (int i = 0; i < N - 1; i++) {
    paths.push_back({i, i + 1});
  }
  // branch
  int K = min(N / 2, N - 2);
  paths.push_back({K, K + 1});
  // loop
  paths.push_back({N - 1, N - 2});

  return paths;
}


// a line, then an edge from 0 to every other node.
// then, we also have a cycle, in which we have K edge from 0 to that cycle
// K >= 1  to have a solution
vector<pair<int,int>> gen_tricky_sol_2(int N, int K) {
  vector<pair<int,int>> paths;
  int N_line = N - rnd.next(5, (int) sqrt(N));

  // line DAG
  for (int i = 0; i < N_line - 1; i++) {
    paths.push_back({i, i + 1});
    if (i > 0)
      paths.push_back({0, i + 1});
  }

  // cycle
  for (int i = N_line; i < N - 1; i++) {
    paths.push_back({i, i + 1});
  }
  paths.push_back({N - 1, N_line});

  for (int i = 0; i < K; i++)
    paths.push_back({0, N_line});

  return paths;
}

/*
0 1
1 2
2 3
...
x-1 x
1 2
1 3
1 4
...
1 x
then add x 0 for no solution; x x+1 and x+1 x for another no solution; 
x x+1, x+1 x, x x+2, x+2 x for a solution to exist (the only branch with 2 cycles). 
This will give a WA and TLE to the incorrect solution of random-start-of-branch
*/
vector<pair<int,int>> gen_tricky_sol_3(int N, int K) {
  vector<pair<int,int>> paths;
  int N_line = N - 2;

  // line
  for (int i = 0; i < N_line - 1; i++)
    paths.push_back({i, i + 1});

  // loop back
  paths.push_back({N_line - 1, 0});

  // spam from 1
  for (int i = 2; i < N_line; i++)
    paths.push_back({1, i});
 
  // sol
  for (int i = 0; i < K; i++){
    paths.push_back({1, N - 1});
    paths.push_back({N - 1, 1});
  }

  return paths;
}


/*
Suppose that S is a set of node, and x is a random node in S. S' is S without x.
If an edge is made from/to a set, then it is meant for all nodes in that set.

0 1
1 S
S 2
2 3
3 S'
2 4
4 2


This is a counter case to the assumption that random path from 0 to start of branch can be picked arbitrarily.
*/
vector<pair<int,int>> gen_tricky_sol_4(int N) {
  // used node: 0, 1, 2, 3, 4, 5
  // S = {5, 6, ..., N-1}
  // x = 5

  vector<pair<int,int>> paths;
  paths.push_back({0, 1});

  for (int i = 5; i < N; i++) {
    paths.push_back({1, i});
    paths.push_back({i, 2});
  }

  paths.push_back({2, 3});
  
  for (int i = 6; i < N; i++) {
    paths.push_back({3, i});
  }

  paths.push_back({2, 4});
  paths.push_back({4, 2});

  return paths;
}


int main(int argc, char* argv[]) {
  registerGen(argc, argv, 0);

  int N = atoi(argv[2]);
  int M = atoi(argv[3]);
  vector<pair<int,int>> paths;

  string argv1 = (string) argv[1];

  if (argv1.find("circle") != std::string::npos) {
    int star_size = atoi(argv[4]);
    paths = gen_circle(N, M, star_size);
  }
  else if (argv1.find("solution1") != std::string::npos) {
    int sol_size = atoi(argv[4]);
    paths = gen_solution(N, M, sol_size);
  }
else if (argv1.find("solution2") != std::string::npos) {
    paths = gen_solution_dag(N, M);
  }
  else if (argv1.find("complete") != std::string::npos) {
    paths = gen_complete(N);
  }
  else if (argv1.find("random") != std::string::npos){
    paths = gen_random(N, M);
  }
  else if (argv1.find("dag") != std::string::npos){
    paths = gen_dag(N, M);
  }
  else if (argv1.find("line") != std::string::npos){
    int line_size = atoi(argv[4]);
    paths = gen_line(N, M, line_size);
  }
  else if (argv1.find("tricky1") != std::string::npos){
    paths = gen_tricky_sol_1(N);
  }

  else if (argv1.find("tricky2") != std::string::npos){
    int K = atoi(argv[4]);
    paths = gen_tricky_sol_2(N, K);
  }
  
  else if (argv1.find("tricky3") != std::string::npos){
    int K = atoi(argv[4]);
    paths = gen_tricky_sol_3(N, K);
  }
  
  else if (argv1.find("tricky4") != std::string::npos){
    paths = gen_tricky_sol_4(N);
  }

  // shuffle
  shuffle(paths.begin(), paths.end());


  print_path(N, paths, 
             argv1.find("double") != std::string::npos, 
             argv1.find("bidirectional") != std::string::npos || argv1.find("complete") != std::string::npos);
}
