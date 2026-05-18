#include <bits/stdc++.h>
using namespace std;

class OverlappingConcatenation {
  int n, m;
  string A, B;

  long long limitSubset() const {
    return 1LL << n;
  }

  double probConstraints(const vector<int>& overlaps) {
    vector<int> parent(2 * n);
    iota(parent.begin(), parent.end(), 0);

    function<int(int)> find = [&](int x) {
      return parent[x] == x ? x : parent[x] = find(parent[x]);
    };
    auto unite = [&](int a, int b) {
      a = find(a);
      b = find(b);
      if (a != b)
        parent[a] = b;
    };

    for (int j : overlaps)
      for (int i = 0; i < (j); ++i)
        unite(n - j + i, n + i);

    map<int, pair<char, int>> comp;
    for (int id = 0; id < (2 * n); ++id) {
      int r = find(id);
      char ch = id < n ? A[id] : B[id - n];
      if (!comp.count(r))
        comp[r] = make_pair(0, 0);
      if (ch == '*')
        comp[r].second++;
      else {
        if (!comp[r].first)
          comp[r].first = ch;
        else if (comp[r].first != ch)
          return 0.0;
      }
    }

    double p = 1.0;
    for (map<int, pair<char, int>>::iterator it = comp.begin(); it != comp.end(); ++it) {
      char known = it->second.first;
      int unk = it->second.second;
      if (known)
        p *= pow(1.0 / m, unk);
      else if (unk > 0)
        p *= pow((double)m, 1 - unk);
    }
    return p;
  }

  double bySubsets() {
    double expectOverlap = 0.0;
    long long limit = 1LL << n;
    for (long long mask = 1; mask < limit; ++mask) {
      vector<int> overlaps;
      int minOv = n + 1;
      for (int j = 0; j < (n); ++j) {
        if (mask & (1LL << j)) {
          overlaps.push_back(j + 1);
          minOv = min(minOv, j + 1);
        }
      }
      double p = probConstraints(overlaps);
      if (!p)
        continue;
      int parity = __builtin_popcountll(mask) & 1;
      expectOverlap += (parity ? 1.0 : -1.0) * minOv * p;
    }
    return 2.0 * n - expectOverlap;
  }

  double byAssignments(const string& alphabet) {
    vector<pair<char, int>> pos;
    for (int i = 0; i < (n); ++i) {
      if (A[i] == '*')
        pos.push_back(make_pair(0, i));
      if (B[i] == '*')
        pos.push_back(make_pair(1, i));
    }
    int u = (int)pos.size();

    double sumMo = 0.0;
    vector<char> assign(u);
    function<void(int)> dfs = [&](int depth) {
      if (depth == u) {
        string a = A, b = B;
        for (int i = 0; i < (u); ++i) {
          if (pos[i].first == 0)
            a[pos[i].second] = assign[i];
          else
            b[pos[i].second] = assign[i];
        }
        int mo = 0;
        for (int k = n; k >= 0; --k) {
          if (k == 0 || a.compare(n - k, k, b, 0, k) == 0) {
            mo = k;
            break;
          }
        }
        sumMo += mo;
        return;
      }
      for (int i = 0; i < m; i++) {
        assign[depth] = alphabet[i];
        dfs(depth + 1);
      }
    };
    dfs(0);

    long long ways = 1;
    for (int i = 0; i < (u); ++i)
      ways *= m;
    return 2.0 * n - sumMo / (double)ways;
  }

public:
  double shortestExpected(string A_, string B_, string alphabet) {
    A = A_;
    B = B_;
    n = (int)A.size();
    m = (int)alphabet.size();

    int unknowns = 0;
    for (int i = 0; i < (n); ++i) {
      if (A[i] == '*')
        unknowns++;
      if (B[i] == '*')
        unknowns++;
    }

    long long assignWays = 1;
    bool tooManyAssign = false;
    for (int i = 0; i < (unknowns); ++i) {
      assignWays *= m;
      if (assignWays > 5000000)
        tooManyAssign = true;
    }

    if (!tooManyAssign && assignWays < limitSubset())
      return byAssignments(alphabet);

    return bySubsets();
  }
};

#ifdef LOCAL_TEST
bool eq(double a, double b) {
  return fabs(a - b) < 1e-4 * max(1.0, fabs(b));
}

int main() {
  OverlappingConcatenation oc;
  struct Case {
    const char *A, *B, *alpha;
    double want;
  };
  Case cases[] = {
      {"aa*aa", "a*aaa", "ab", 7.0},
      {"**", "**", "ab", 3.125},
      {"**a*cd", "cde***", "abcdefghijklmnopqrstuvwxyz", 10.0},
      {"h*phz*", "hzph*p", "zph", 10.555555555555557},
      {"a*a*a*a*a*a*", "*a*a*a*a*a*a", "abcdef", 23.6983063804134},
      {"********************************", "********************************",
       "abcdefghijklmnopqrstuvwxyz", 63.958459262501435},
      {"********************************", "********************************", "yz",
       62.358835095751274},
      {"o", "o", "o", 1.0},
      {"zzz", "zzz", "z", 3.0},
      {"**", "*s", "s", 2.0},
      {"******", "******", "oty", 11.31881431805224},
  };
  int fail = 0;
  for (int i = 0; i < (int)(sizeof(cases) / sizeof(cases[0])); i++) {
    double got = oc.shortestExpected(cases[i].A, cases[i].B, cases[i].alpha);
    if (!eq(got, cases[i].want)) {
      printf("FAIL case %d: got %.12f want %.12f\n", i, got, cases[i].want);
      fail++;
    }
  }
  printf(fail ? "FAILED %d\n" : "OK\n", fail);
  return fail;
}
#endif
