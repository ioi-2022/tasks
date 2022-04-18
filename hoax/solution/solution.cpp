#include "hoax.h"

#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
using namespace std;

#define ii pair<int,int>

const int MAXN = 2e5 + 5;

int N, S, Q;
int O[MAXN];
vector<ii> onlineTime[MAXN];
vector<ii> onlineEvent;

class UDFS{
	vector<int> p, lvl, num;
	public:
		UDFS(){}
		UDFS(int n){
			p.reserve(n+1); lvl.reserve(n+1); num.reserve(n+1);
			for(int i = 1; i <= n; ++i){
				p[i] = i;
				lvl[i] = 0;
				num[i] = 1;
			}
		}
		int getParent(int x){
			return (p[x] == x) ? x : getParent(p[x]);
		}
		void connect(int a, int b){
			int pa = getParent(a), pb = getParent(b);
			if(pa != pb){
				if(lvl[pa] > lvl[pb]){
					p[pb] = pa;
					num[pa] += num[pb];
				}else{
					p[pa] = pb;
					num[pb] += num[pa];
					if(lvl[pa] == lvl[pb]){
						lvl[pb]++;
					}
				}
			}
		}
		int getNum(int x){
			return num[getParent(x)];
		}
} groups;

void work(){
	groups = UDFS(N);

	// Masukkan daftar login/logout ke timeline
	for(int i = 1; i <= N; ++i){
		for(ii j : onlineTime[i]){
			// User login dulu, baru logout
			onlineEvent.push_back(ii(j.first, -i));
			onlineEvent.push_back(ii(j.second, i));
		}
	}
	sort(onlineEvent.begin(), onlineEvent.end());

	int groupCurrentlyOnline = -1;
	int numCurrentlyOnline = 0;
	for(ii event : onlineEvent){
		if(event.second < 0){
			// Ada user masuk
			event.second = -event.second;
			if(numCurrentlyOnline > 0){
				// User ini bisa digabung ke grup
				groups.connect(groupCurrentlyOnline, event.second);
			}else{
				// User ini menjadi kepala untuk grup berikutnya
				groupCurrentlyOnline = event.second;
			}

			numCurrentlyOnline++;
		}else{
			// Ada user keluar
			numCurrentlyOnline--;
			if(numCurrentlyOnline == 0){
				// Harus buat grup baru kalau ada user baru login
				groupCurrentlyOnline = -1;
			}
		}
	}
}

void init(int _N, int _S, vector<int> T,
          vector<vector<int>> A, vector<vector<int>> B) {
  N = _N;
  S = _S;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < T[i]; ++j) {
      onlineTime[i + 1].push_back(ii(A[i][j], B[i][j]));
    }
  }

  work();
}

int count_users(int P) {
  return groups.getNum(P + 1);
}
