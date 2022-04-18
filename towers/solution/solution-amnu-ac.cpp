#include "towers.h"
#include <bits/stdc++.h>
#define fi first
#define se second
using namespace std;
typedef pair<int,int> pii;
typedef pair<int,pii> piii;

const int MAXN = 1e5+5;
const int INF = 2e9;

struct gap {
  int L, R, mi, ma, val;
  
  gap operator + (gap o) {
    gap ret;
    ret.L = L;
    ret.R = o.R;
    ret.mi = min(mi,o.mi);
    ret.ma = max(ma,o.ma);
    ret.val = max(o.ma-mi,max(val,o.val));
    return ret;
  }
};

struct tree {
  int L, R;
  gap val;
  tree *lef, *rig;
  
  void build(int l,int r,vector<int> &v) {
    L = l; R = r;
    if (L == R) {
      val.L = L;
      val.R = R;
      val.mi = v[L];
      val.ma = v[R];
      val.val = 0;
      return;
    }
    
    int mid = (L+R)/2;
    lef = new tree();
    rig = new tree();
    lef->build(L,mid,v);
    rig->build(mid+1,R,v);
    val = lef->val + rig->val;
  }
  
  gap query(int D,gap x) {
    if (R <= x.R || L > x.R+1) return x;
    if (L == x.R+1 && (x+val).val < D) return x+val;
    if (L == R) return x;
    return rig->query(D,lef->query(D,x));
  }
}
pref, suff;

struct answer {
  int L, R, val;
  answer *lef, *rig;
  
  void build(int l,int r) {
    L = l; R = r;
    val = 0;
    if (L < R) {
      int mid = (L + R)/2;
      lef = new answer();
      rig = new answer();
      lef->build(L,mid);
      rig->build(mid+1,R);
    }
  }
  
  answer *toggle(int id) {
    answer *ret = new answer();
    *ret = *this;
    if (L == R) {
      ret->val = 1 - ret->val;
      return ret;
    }
    int mid = (L + R)/2;
    if (id <= mid) ret->lef = lef->toggle(id);
    else ret->rig = rig->toggle(id);
    ret->val = ret->lef->val + ret->rig->val;
    return ret;
  }
  
  int query(int l,int r) {
    if (l > R || r < L) return 0;
    if (l <= L && r >= R) return val;
    return lef->query(l,r) + rig->query(l,r);
  }
};

int N;
vector <int> delta;
vector <answer*> cand;
set <piii> zigzag;
set <pii> shortest;

void init(int n,vector<int> H) {
  N = n;
  
  pref.build(0,N-1,H);
  reverse(H.begin(),H.end());
  suff.build(0,N-1,H);
  
  H.push_back(INF);
  reverse(H.begin(),H.end());
  H.push_back(INF);
  H.push_back(0);
  
  piii now = {0,{INF,0}};
  for (int i = 1; i <= N+1; i++) {
    if (H[i] < H[i-1] && H[i] < H[i+1]) {
      now.fi = i-1;
      now.se.fi -= H[i];
      now.se.se = -H[i];
    }
    if (H[i] > H[i-1] && H[i] > H[i+1]) {
      now.se.se += H[i];
      zigzag.insert(now);
      now.se.fi = H[i];
    }
  }
  
  cand.push_back(new answer());
  cand[0]->build(0,N-1);
  for (piii zig : zigzag) {
    cand[0] = cand[0]->toggle(zig.fi);
    shortest.insert({zig.se.fi,-zig.fi});
    shortest.insert({zig.se.se,zig.fi});
  }
  
  while (zigzag.size() > 1) {
    pii pos = *shortest.begin();
    auto it = zigzag.lower_bound({abs(pos.se),{0,0}});
    piii zig = *it;
    
    delta.push_back(pos.fi);
    cand.push_back(cand.back()->toggle(abs(pos.se)));
    
    if (pos.se < 0) {
      piii zag = *--it;
      zigzag.erase(zag);
      shortest.erase({zag.se.se,zag.fi});
      zag.se.se += zig.se.se - zig.se.fi;
      zigzag.insert(zag);
      shortest.insert({zag.se.se,zag.fi});
    }
    else {
      piii zag = *++it;
      zigzag.erase(zag);
      shortest.erase({zag.se.fi,-zag.fi});
      zag.se.fi += zig.se.fi - zig.se.se;
      zigzag.insert(zag);
      shortest.insert({zag.se.fi,-zag.fi});
    }
    
    shortest.erase({zig.se.fi,-zig.fi});
    shortest.erase({zig.se.se,zig.fi});
    zigzag.erase(zig);
  }
}

int max_towers(int L,int R,int D) {
  L = pref.query(D,{0,L-1,INF,0,0}).R + 1;
  R = N - suff.query(D,{0,N-R-2,INF,0,0}).R - 2;
  if (L > R) return 1;
  D = lower_bound(delta.begin(),delta.end(),D) - delta.begin();
  return 2 + cand[D]->query(L,R);
}
