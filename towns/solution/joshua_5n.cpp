#include "towns.h"
#include <bits/stdc++.h>

int cnt[2005] = {0};
int ask[2005][2005] = {0};

int find_town(int N) 
{
  int ans = -1;
  for(int i=0;i<N;i++)
  {
    for(int j=N-1;j>=0;j--)
    {
      if(i==j) continue;
      if(cnt[i]>=2) break;
      if(cnt[j]>=2) continue;
      if(ask[i][j]) continue;
      bool tmp = check_road(i,j);
      if(tmp==true) cnt[i]++;
      else cnt[j]++;   
      ask[i][j] = ask[j][i] = 1;
    }

    if(cnt[i]<2)
    {
      for(int j=0;j<N;j++)
      {
        if(i==j) continue;
        if(cnt[i]>=2) break;
        if(ask[i][j]) continue;
        bool tmp = check_road(i,j);
        if(tmp==true) cnt[i]++;
        else cnt[j]++;   
        ask[i][j] = ask[j][i] = 1;
      }
      if(cnt[i]<2) 
      {
        ans = i;
        break;
      } 
    }
  }
  return ans;
}
