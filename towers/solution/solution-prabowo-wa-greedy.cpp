#include "towers.h"

#include <algorithm>
#include <utility>
#include <vector>

const int kInf = 1e9 + 5;

class Wavelet {
 private:
  std::vector<std::vector<int>> partitions, pos;
  std::vector<int> alphabet;
  void build(int idx, int l, int r, int ll, int rr, std::vector<std::pair<int, int>> &val) {
    int mid = (l + r) / 2;
    partitions[idx] = {0};
    for (int i = ll; i < rr; ++i) {
      partitions[idx].push_back(partitions[idx].back() + (val[i].first < alphabet[mid]));
      pos[idx].push_back(val[i].second);
    }
    if (l + 1 == r) return;
    int mmid = std::stable_partition(val.begin() + ll, val.begin() + rr, 
        [&](std::pair<int, int> v) { return v.first < alphabet[mid]; }) - val.begin();
    build(idx + 1, l, mid, ll, mmid, val);
    build(idx + (mid - l) * 2, mid, r, mmid, rr, val);
  }

  int range(int idx, int l, int r, int ll, int rr, int parL, int parR) {
    if (parL == parR || l >= rr || r <= ll) return -1;
    if (l >= ll && r <= rr) return pos[idx][parR - 1];
    int mid = (l + r) / 2;
    return std::max(range(idx + 1, l, mid, ll, rr, partitions[idx][parL], partitions[idx][parR]),
                    range(idx + (mid - l) * 2, mid, r, ll, rr, parL - partitions[idx][parL], parR - partitions[idx][parR]));
  }

 public:
  void build(std::vector<int> val) {
    alphabet = val;
    std::sort(alphabet.begin(), alphabet.end());
    alphabet.erase(std::unique(alphabet.begin(), alphabet.end()), alphabet.end());
    std::vector<std::pair<int, int>> valPos(val.size());
    for (int i = 0; i < static_cast<int>(val.size()); ++i) valPos[i] = {val[i], i};
    partitions.resize(alphabet.size() * 2); pos.resize(alphabet.size() * 2);
    build(0, 0, alphabet.size(), 0, val.size(), valPos);
  }

  // return max(i) in [l..r), s.t. a <= val[i] <= b
  int range(int l, int r, int a, int b) {
    return range(0, 0, alphabet.size(),
        std::lower_bound(alphabet.begin(), alphabet.end(), a) - alphabet.begin(),
        std::upper_bound(alphabet.begin(), alphabet.end(), b) - alphabet.begin(),    
        l, r);
  }
} waveletH;

std::vector<int> H;

void init(int, std::vector<int> _H) {
  H = _H;
  waveletH.build(H);
}

// From a leased tower, take the closest intermediary tower
//   then lease the closest tower, and so on
int max_towers(int L, int R, int D) {
  ++R;
  std::vector<int> dp0(R - L), dp1(R - L);
  for (int i = L; i < R; ++i) {
    int lft = waveletH.range(L, i, H[i] + D, kInf);
    dp0[i - L] = (lft < 0 ? 0 : dp1[lft - L]) + 1;
    lft = waveletH.range(L, i, 0, H[i] - D);
    dp1[i - L] = (lft < 0 ? 0 : dp0[lft - L]);
  }
  return *std::max_element(dp0.begin(), dp0.end());
}
