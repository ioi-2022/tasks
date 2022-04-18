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

void upd(int L, int R, int x, int y, int idx)
{
	if(L == R)
	{
		ST[idx] = y;
	}
	else
	{
		int C = (L + R) / 2;
		if(x <= C)
			upd(L, C, x, y, idx * 2);
		else
			upd(C + 1, R, x, y, idx * 2 + 1);
		ST[idx] = min(ST[idx * 2], ST[idx * 2 + 1]);
	}
}

int cari(int L, int R, int z, int idx)
{
	// cout << L << " " << R << " " << z << " " << idx << "__\n";
	if(L == R)
		return L;
	else
	{
		int C = (L + R) / 2;
		// cout << ST[idx * 2] << " dan " << z << "\n";
		if(ST[idx * 2] <= z)
			return cari(L, C, z, idx * 2);
		else
			return cari(C + 1, R, z, idx * 2 + 1);
	}
}


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
	for(int i = L; i <= R; i++)
		upd(L, R, i, a[i], 1);
	for(int i = L; i <= R; i++)
	{
		upd(L, R, i, 1e9, 1);
		if(ST[1] > a[i] - D)
			kan[i] = R + 1;
		else
			kan[i] = cari(L, R, a[i] - D, 1);
	}
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