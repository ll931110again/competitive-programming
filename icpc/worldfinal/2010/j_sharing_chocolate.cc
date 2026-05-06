#include <bits/stdc++.h>
using namespace std;

/*
Sharing Chocolate (UVa / Valladolid OJ 1099)

We need to decide if an x*y rectangle can be partitioned by repeated straight
cuts along grid lines into n rectangles whose areas match the given parts[].

Classic DP over subsets:
- Let sum[mask] be total area of pieces in 'mask'.
- A state (mask, w, h) is feasible if sum[mask]==w*h and we can split mask into
  two submasks that correspond to a horizontal or vertical cut.
We canonicalize dimensions so w<=h to reduce states.
*/

struct Key {
  int mask;
  int w;
  int h;
  bool operator==(Key const &other) const {
    return mask == other.mask && w == other.w && h == other.h;
  }
};

struct KeyHash {
  size_t operator()(Key const &k) const noexcept {
    // Mix three integers.
    size_t x = std::hash<int>{}(k.mask);
    x ^= (std::hash<int>{}(k.w) + 0x9e3779b97f4a7c15ULL + (x << 6) + (x >> 2));
    x ^= (std::hash<int>{}(k.h) + 0x9e3779b97f4a7c15ULL + (x << 6) + (x >> 2));
    return x;
  }
};

static vector<int> partArea;
static vector<int> sumMask;
static unordered_map<Key, bool, KeyHash> memo;

static bool canFill(int mask, int w, int h) {
  if (w > h)
    swap(w, h);
  Key key{mask, w, h};
  auto it = memo.find(key);
  if (it != memo.end())
    return it->second;

  int area = sumMask[mask];
  if (area != w * h)
    return memo[key] = false;

  // Single requested piece: area already matches, so it's feasible.
  if ((mask & (mask - 1)) == 0)
    return memo[key] = true;

  // Enumerate proper submasks; fix one bit to avoid symmetric duplicates.
  int lsb = mask & -mask;
  for (int sub = (mask - 1) & mask; sub; sub = (sub - 1) & mask) {
    if ((sub & lsb) == 0)
      continue;

    int s = sumMask[sub];

    // Horizontal cut: width stays w, heights split.
    if (s % w == 0) {
      int h1 = s / w;
      if (0 < h1 && h1 < h) {
        if (canFill(sub, w, h1) && canFill(mask ^ sub, w, h - h1))
          return memo[key] = true;
      }
    }

    // Vertical cut: height stays h, widths split.
    if (s % h == 0) {
      int w1 = s / h;
      if (0 < w1 && w1 < w) {
        if (canFill(sub, w1, h) && canFill(mask ^ sub, w - w1, h))
          return memo[key] = true;
      }
    }
  }

  return memo[key] = false;
}

static bool solveCase(int x, int y, const vector<int> &parts) {
  int n = (int)parts.size();
  long long total = 0;
  for (int a : parts)
    total += a;
  if (total != 1LL * x * y)
    return false;

  partArea = parts;
  sumMask.assign(1 << n, 0);
  for (int i = 0; i < n; i++) {
    int bit = 1 << i;
    for (int mask = 0; mask < bit; mask++) {
      sumMask[mask | bit] = sumMask[mask] + partArea[i];
    }
  }

  memo.clear();
  int full = (1 << n) - 1;
  return canFill(full, x, y);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  int caseNum = 1;
  while (cin >> n) {
    if (n == 0)
      break;
    int x, y;
    cin >> x >> y;
    vector<int> parts(n);
    for (int i = 0; i < n; i++)
      cin >> parts[i];

    bool ok = solveCase(x, y, parts);
    cout << "Case " << caseNum++ << ": " << (ok ? "Yes" : "No") << "\n";
  }
  return 0;
}
