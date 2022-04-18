#ifndef ATCODER_INTERNAL_BITOP_HPP
#define ATCODER_INTERNAL_BITOP_HPP 1

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace atcoder {

namespace internal {

// @param n `0 <= n`
// @return minimum non-negative `x` s.t. `n <= 2**x`
int ceil_pow2(int n) {
    int x = 0;
    while ((1U << x) < (unsigned int)(n)) x++;
    return x;
}

// @param n `1 <= n`
// @return minimum non-negative `x` s.t. `(n & (1 << x)) != 0`
constexpr int bsf_constexpr(unsigned int n) {
    int x = 0;
    while (!(n & (1 << x))) x++;
    return x;
}

// @param n `1 <= n`
// @return minimum non-negative `x` s.t. `(n & (1 << x)) != 0`
int bsf(unsigned int n) {
#ifdef _MSC_VER
    unsigned long index;
    _BitScanForward(&index, n);
    return index;
#else
    return __builtin_ctz(n);
#endif
}

}  // namespace internal

}  // namespace atcoder

#endif  // ATCODER_INTERNAL_BITOP_HPP

#ifndef ATCODER_SEGTREE_HPP
#define ATCODER_SEGTREE_HPP 1

#include <algorithm>
#include <cassert>
#include <vector>

namespace atcoder {

template <class S, S (*op)(S, S), S (*e)()> struct segtree {
  public:
    segtree() : segtree(0) {}
    explicit segtree(int n) : segtree(std::vector<S>(n, e())) {}
    explicit segtree(const std::vector<S>& v) : _n(int(v.size())) {
        log = internal::ceil_pow2(_n);
        size = 1 << log;
        d = std::vector<S>(2 * size, e());
        for (int i = 0; i < _n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }

    void set(int p, S x) {
        assert(0 <= p && p < _n);
        p += size;
        d[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    S get(int p) const {
        assert(0 <= p && p < _n);
        return d[p + size];
    }

    S prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= _n);
        S sml = e(), smr = e();
        l += size;
        r += size;

        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return op(sml, smr);
    }

    S all_prod() const { return d[1]; }

    template <bool (*f)(S)> int max_right(int l) const {
        return max_right(l, [](S x) { return f(x); });
    }
    template <class F> int max_right(int l, F f) const {
        assert(0 <= l && l <= _n);
        assert(f(e()));
        if (l == _n) return _n;
        l += size;
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(op(sm, d[l]))) {
                while (l < size) {
                    l = (2 * l);
                    if (f(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }

    template <bool (*f)(S)> int min_left(int r) const {
        return min_left(r, [](S x) { return f(x); });
    }
    template <class F> int min_left(int r, F f) const {
        assert(0 <= r && r <= _n);
        assert(f(e()));
        if (r == 0) return 0;
        r += size;
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(op(d[r], sm))) {
                while (r < size) {
                    r = (2 * r + 1);
                    if (f(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

  private:
    int _n, size, log;
    std::vector<S> d;

    void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
};

}  // namespace atcoder

#endif  // ATCODER_SEGTREE_HPP

#include "towers.h"

#include <bits/stdc++.h>
using namespace std;

struct Node {
  int max;
  int min;
  int max_right_min_left;
  int max_left_min_right;
};

typedef optional<Node> OptionalNode;

OptionalNode Empty() {
  return {};
}

OptionalNode Merge(OptionalNode l, OptionalNode r) {
  if (!l.has_value()) {
    return r;
  }
  if (!r.has_value()) {
    return l;
  }
  return (Node) {
    max(l.value().max, r.value().max),
    min(l.value().min, r.value().min),
    max(max(l.value().max_right_min_left, r.value().max_right_min_left),
        r.value().max - l.value().min),
    max(max(l.value().max_left_min_right, r.value().max_left_min_right),
        l.value().max - r.value().min)
  };
}

typedef atcoder::segtree<OptionalNode, Merge, Empty> Segtree;

int N;
vector<int> H;
Segtree st;

vector<int> maxD;
vector<int> maxDs;
map<int, int> ixMaxD;
int szMaxD;

struct PersistentNode {
  PersistentNode(int _val, PersistentNode* _l, PersistentNode* _r)
      : val(_val), l(_l), r(_r) {}
  
  int val;
  PersistentNode* l;
  PersistentNode* r;
};

vector<PersistentNode*> persistent_st;

PersistentNode* empty(int L, int R) {
  if (L == R) {
    return new PersistentNode(0, NULL, NULL);
  }
  int M = (L + R) >> 1;
  return new PersistentNode(0, empty(L, M), empty(M + 1, R));
}

PersistentNode* update(PersistentNode* node, int L, int R, int pos, int val) {
  if (pos < L || pos > R) {
    return node;
  }
  if (L == R) {
    return new PersistentNode(node->val + val, NULL, NULL);
  }
  int M = (L + R) >> 1;
  return new PersistentNode(
      node->val + val,
      update(node->l, L, M, pos, val),
      update(node->r, M + 1, R, pos, val));
}

int query(
    PersistentNode* add, PersistentNode* sub, int L, int R, int x, int y) {
  if (x <= L && R <= y) {
    return add->val - sub->val;
  }
  if (y < L || R < x) {
    return 0;
  }
  int M = (L + R) >> 1;
  return
      query(add->l, sub->l, L, M, x, y) + query(add->r, sub->r, M + 1, R, x, y);
}

void init(int _N, std::vector<int> _H) {
  N = _N;
  H = _H;
  st = Segtree(N);
  for (int i = 0; i < N; ++i) {
    st.set(i, (Node) {H[i], H[i], INT_MIN, INT_MIN});
  }
  for (int i = 0; i < N; ++i) {
    int lower_height_left = st.min_left(
        i, [&] (OptionalNode node) {
          return !node.has_value() || node.value().min >= H[i];
        }) - 1;
    int lower_height_right = st.max_right(
        i + 1, [&] (OptionalNode node) {
          return !node.has_value() || node.value().min >= H[i];
        });
    int ret = INT_MAX;
    if (lower_height_left >= 0) {
      ret = min(ret, st.prod(lower_height_left + 1, i + 1).value().max - H[i]);
    }
    if (lower_height_right < N) {
      ret = min(ret, st.prod(i, lower_height_right).value().max - H[i]);
    }
    maxD.push_back(ret);
    maxDs.push_back(ret);
  }
  sort(maxDs.begin(), maxDs.end());
  maxDs.erase(unique(maxDs.begin(), maxDs.end()), maxDs.end());
  for (int i = 0; i < static_cast<int>(maxDs.size()); ++i) {
    ixMaxD[maxDs[i]] = i;
  }
  szMaxD = maxDs.size();
  persistent_st.push_back(empty(0, szMaxD - 1));
  for (int i = 0; i < N; ++i) {
    persistent_st.push_back(
        update(persistent_st[i], 0, szMaxD - 1, ixMaxD[maxD[i]], 1));
  }
}

int max_towers(int L, int R, int D) {
  int ixD = lower_bound(maxDs.begin(), maxDs.end(), D) - maxDs.begin();
  int answer = query(
    persistent_st[R + 1], persistent_st[L], 0, szMaxD - 1, ixD, szMaxD - 1);
  if (answer == 0) {
    answer = 1;
    int minH = st.prod(L, R + 1).value().min;
    int minH_index = st.max_right(
      L, [&] (OptionalNode node) {
        return !node.has_value() || node.value().min > minH;
      });
    OptionalNode node_left = st.prod(L, minH_index);
    if (node_left.has_value() && node_left.value().max_right_min_left >= D) {
      ++answer;
    }
    OptionalNode node_right = st.prod(minH_index + 1, R + 1);
    if (node_right.has_value() && node_right.value().max_left_min_right >= D) {
      ++answer;
    }
    assert(answer <= 2);
    return answer;
  }
  int leftmost_answer = L, rightmost_answer = R;
  {
    int lo = L;
    int hi = R;
    while (lo <= hi) {
      int mid = (lo + hi) >> 1;
      if (query(persistent_st[mid + 1], persistent_st[L],
                0, szMaxD - 1, ixD, szMaxD - 1) > 0) {
        leftmost_answer = mid;
        hi = mid - 1;
      } else {
        lo = mid + 1;
      }
    }
  }
  {
    int lo = L;
    int hi = R;
    while (lo <= hi) {
      int mid = (lo + hi) >> 1;
      if (query(persistent_st[R + 1], persistent_st[mid],
                0, szMaxD - 1, ixD, szMaxD - 1) > 0) {
        rightmost_answer = mid;
        lo = mid + 1;
      } else {
        hi = mid - 1;
      }
    }
  }
  OptionalNode node_left = st.prod(L, leftmost_answer);
  if (node_left.has_value() && node_left.value().max_right_min_left >= D) {
    ++answer;
  }
  OptionalNode node_right = st.prod(rightmost_answer + 1, R + 1);
  if (node_right.has_value() && node_right.value().max_left_min_right >= D) {
    ++answer;
  }
  return answer;
}
