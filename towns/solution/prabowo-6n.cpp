#include "towns.h"

#include <cassert>
#include <functional>
#include <vector>

int find_town(int N) {
  int a = 0, b = 1, c = 2;
  int degA = 0, degB = 0, degC = 0;

  if (check_road(a, b)) ++degA; else ++degB;
  if (check_road(a, c)) ++degA; else ++degC;
  if (check_road(b, c)) ++degB; else ++degC;

  for (int i = 3; i < N; ++i) {
    int degI = 0;
    if (check_road(a, i)) ++degA; else ++degI;
    if (check_road(b, i)) ++degB; else ++degI;
    if (check_road(c, i)) ++degC; else ++degI;

    if (degA >= 2) a = i, degA = degI;
    else if (degB >= 2) b = i, degB = degI;
    else if (degC >= 2) c = i, degC = degI;
  }

  for (int u : {a, b, c}) {
    int deg = 0;
    for (int i = 0; i < N; ++i) {
      if (u == i) continue;
      if (check_road(u, i)) ++deg;
    }
    if (deg <= 1) return u;
  }
  return -1;
}
