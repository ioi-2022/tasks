#include "towers.h"

#include <vector>

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define pb push_back
#define mp make_pair
#define fi first
#define se second
const int NN = 2e5 + 5;
int ma = 0, k, a[NN], n, kan[NN], nx[NN], d[NN], m;
int ST[4 * NN];
int BIT[3 * NN];

map<int, int> me;

void init(int N, std::vector<int> H) {
	n = N;
	for(int i = 0; i < N; i++)
	{
		a[i + 1] = H[i];
	}
}

int max_towers(int L, int R, int D) {
	L++;
	R++;
	long long ans = 0;
	for(int i = L; i <= R; i++)
	{
		kan[i] = R + 1;
		int X = i;
		if(i > L && a[i] <= a[i - 1])
			X = kan[i - 1] - 1;
		for(ll j = X + 1; j <= min(R, X + 1000); j++)
			if(a[j] <= a[i] - D)
			{
				kan[i] = j;
				break;
			}
		// for(ll j = max(X + 1, (X + 1 + n)/2 - 1000); j <= min(R, (X + 1 + n)/2 + 1000); j++)
		// 	if(a[j] <= a[i] - D)
		// 	{
		// 		kan[i] = j;
		// 		break;
		// 	}
		for(ll j = max(X + 1, X - 1000); j <= R; j++)
		{
			++ans;
			if(a[j] <= a[i] - D)
			{
				
				kan[i] = j;
				break;
			}
		}
	}
	// printf("%lld\n", ans);
	for(int i = L; i <= R; i++)
	{
		nx[i] = R + 1;
		for(int j = i + 1; j <= min((int)R, i + 2000); j++)
			if(a[j] >= a[i] + D)
				nx[i] = min(nx[i], kan[j]);
		for(int j = max(i + 1, (i+1+R)/2 - 2000); j <= min((int)R, (i+1+R)/2 + 2000); j++)
		{
			if(j > nx[i])
				break;
			if(a[j] >= a[i] + D)
				nx[i] = min(nx[i], kan[j]);
		}
		for(int j = max(i + 1, (int)R - 2000); j <= R; j++)
		{
			if(j > nx[i])
				break;
			if(a[j] >= a[i] + D)
				nx[i] = min(nx[i], kan[j]);
		}
	}
	d[R + 1] = 0;
	for(int i = R; i >= L; i--)
		d[i] = max(d[i + 1], 1 + d[nx[i]]);
	return d[L];
}


/*
7 1
10 20 60 30 50 40 70
1 5 10

7 3
10 20 60 30 50 40 70
1 5 10
2 2 100
0 6 17
*/