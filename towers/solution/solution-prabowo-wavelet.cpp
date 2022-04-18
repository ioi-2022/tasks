#include "towers.h"

#include <algorithm>
#include <utility>
#include <vector>

const int kInf = 1e9 + 5;

struct Node {
  int cnt, max, min, maxmin, minmax;
  Node(int val=-1): cnt(1), max(val), min(val), maxmin(-1), minmax(-1) {
    if (val == -1) cnt = 0, max = -1, min = kInf;
  }
};
Node operator + (const Node &p, const Node &q) {
  Node ret;
  ret.cnt = p.cnt + q.cnt;
  ret.max = std::max(p.max, q.max); ret.min = std::min(p.min, q.min);
  ret.maxmin = std::max({p.maxmin, q.maxmin, p.max - q.min});
  ret.minmax = std::max({p.minmax, q.minmax, q.max - p.min});
  return ret;
}

int N;
std::vector<int> H;

std::vector<Node> tree;
Node query(int l, int r) {
  Node lft, rgt;
  for (l = std::max(0, l) + N, r = std::min(N, r) + N; l < r; l >>= 1, r >>= 1) {
    if (l & 1) lft = lft + tree[l++];
    if (r & 1) rgt = tree[--r] + rgt;
  }
  return lft + rgt;
}

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

  Node range(int idx, int l, int r, int ll, int rr, int parL, int parR) {
    if (parL == parR || l >= rr || r <= ll) return Node();
    if (l >= ll && r <= rr) {
      Node ret;
      ret.cnt = parR - parL; ret.max = pos[idx][parR - 1]; ret.min = pos[idx][parL];
      return ret;
    }
    int mid = (l + r) / 2;
    return range(idx + 1, l, mid, ll, rr, partitions[idx][parL], partitions[idx][parR]) +
           range(idx + (mid - l) * 2, mid, r, ll, rr, parL - partitions[idx][parL], parR - partitions[idx][parR]);
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

  // Correctly returns (cnt(i), min(i), max(i)) in val[l..r) s.t. a <= val[i] <= b
  Node range(int l, int r, int a, int b) {
    return range(0, 0, alphabet.size(),
        std::lower_bound(alphabet.begin(), alphabet.end(), a) - alphabet.begin(),
        std::upper_bound(alphabet.begin(), alphabet.end(), b) - alphabet.begin(),    
        l, r);
  }
} waveletH, waveletDelta;

void init(int _N, std::vector<int> _H) {
  N = _N; H = _H; tree.resize(N * 2);
  for (int i = 0; i < N; ++i) tree[i + N] = Node(H[i]);
  for (int i = N - 1; i > 0; --i) tree[i] = tree[i * 2] + tree[i * 2 + 1];

  waveletH.build(H);

  std::vector<int> delta(N);
  for (int i = 0; i < N; ++i) {
    delta[i] = kInf;
    int lft = waveletH.range(0, i, 0, H[i] - 1).max, rgt = waveletH.range(i, N, 0, H[i] - 1).min;
    if (lft >= 0) delta[i] = std::min(delta[i], query(lft + 1, i).max - H[i]);
    if (rgt < N) delta[i] = std::min(delta[i], query(i + 1, rgt).max - H[i]);
  }
  
  waveletDelta.build(delta);
}

int max_towers(int L, int R, int D) {
  Node res = waveletDelta.range(L, ++R, D, kInf);
  int cnt = res.cnt, lft = res.min, rgt = res.max;
  if (cnt == 0) { cnt = 1; lft = rgt = waveletH.range(L, R, 0, query(L, R).min).min; }
  cnt += query(L, waveletH.range(L, lft, D + H[lft], kInf).max + 1).minmax >= D;
  cnt += query(waveletH.range(rgt, R, D + H[rgt], kInf).min, R).maxmin >= D;
  return cnt;
}
