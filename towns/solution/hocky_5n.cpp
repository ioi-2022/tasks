#include "towns.h"
#include <bits/stdc++.h>
using namespace std;
 
int memo[2005][2005];
bool compared[2005][2005];
typedef pair<int, int> PII;
#define trav(nx, v) for(auto &nx : v)
#define rep(i,a,b) for(int i = a;i < (b);i++)
#define all(a) begin(a), end(a)
#define pb push_back
#define fi first
#define se second
 
int cnt = 0;
 
bool check(int a, int b){
  if(memo[a][b] != -1) return memo[a][b];
  // cout << a << " " << b << endl;
  cnt++;
  if(cnt > 40000){
    while(1){}
  }
  // cout << "ask " << a << " " << b << endl;
  memo[a][b] = check_road(a, b);
  memo[b][a] = !memo[a][b];
  return memo[a][b]; 
}
 
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
 
int find_town(int n) {
  memset(memo, -1, sizeof(memo));
 
  priority_queue <PII, vector <PII>, greater<PII>> antri;
 
  vector <PII> isi;
  rep(i,0,n) antri.push({0, i}), isi.pb({0, i});
 
  while(1){
    vector <PII> old = isi;
    vector <PII> done;
    while(!antri.empty()){
      auto baru = antri.top(); antri.pop();
      vector <PII> titip;
      while(!antri.empty()){
        auto depan = antri.top(); antri.pop();
        if(memo[depan.se][baru.se] == -1){
          if(check(depan.se, baru.se)){
            depan.fi++;
          }else baru.fi++;
        }
        if(depan.fi < 2) titip.pb(depan);
        if(baru.fi >= 2) break;
      }
      if(baru.fi < 2){
        done.pb(baru);
      }
      trav(cur, titip) antri.push(cur);
    }
    isi.clear();
    isi.insert(isi.end(), all(done));
    reverse(all(isi));
    trav(cur, isi) antri.push(cur);
    // cout << "iter " << endl;
    // trav(cur, isi) cout << cur.fi << " " << cur.se << endl;
    if(old == isi) break;
  }
 
  vector <int> sisa;
  trav(cur, isi) sisa.pb(cur.se);
  sort(all(sisa));
 
  vector <int> perm(n);
  iota(begin(perm), end(perm), 0);
  shuffle(begin(perm), end(perm), rng);
  for(int i = 0;i < n;i++){
    int curans = 0;
    int cur = perm[i];
    if(*lower_bound(all(sisa), cur) != cur) continue;
    for(int j = 0;j < n;j++){
      if(i == j) continue;
      curans += check(perm[i], perm[j]);
      if(curans == 2) break;
    }
    if(curans <= 1) return perm[i];
  }
  return -1;
}
