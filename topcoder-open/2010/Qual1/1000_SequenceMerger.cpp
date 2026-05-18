#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;
typedef __int128 i128;

static const long long LIM = 1000000000LL;

static i128 parse128(const string& s) {
  i128 v = 0;
  for (char c : s)
    v = v * 10 + (c - '0');
  return v;
}

struct Seq {
  char type;
  i128 a, b;
  long long c;
  vector<i128> vals;
};

class SequenceMerger {
  vector<Seq> seqs;

  long long arithCount(const Seq& s, i128 v) const {
    if (s.c == 0)
      return 0;
    if (s.b == 0)
      return (s.a <= v) ? s.c : 0;
    if (v < s.a)
      return 0;
    i128 cnt = (v - s.a) / s.b + 1;
    if (cnt > (i128)s.c)
      cnt = s.c;
    return (long long)cnt;
  }

  long long geomCount(const Seq& s, i128 v) const {
    if (s.c == 0)
      return 0;
    if (s.a > v)
      return 0;
    if (s.b == 1)
      return s.c;
    long long cnt = 0;
    i128 term = s.a;
    for (long long i = 0; i < s.c; ++i) {
      if (term > v)
        break;
      ++cnt;
      if (term > v / s.b)
        break;
      term *= s.b;
    }
    return cnt;
  }

  long long explicitCount(const Seq& s, i128 v) const {
    return upper_bound(s.vals.begin(), s.vals.end(), v) - s.vals.begin();
  }

  long long countLE(i128 v) const {
    long long sum = 0;
    for (const Seq& s : seqs) {
      long long add = 0;
      if (s.type == 'A')
        add = arithCount(s, v);
      else if (s.type == 'G')
        add = geomCount(s, v);
      else
        add = explicitCount(s, v);
      sum += add;
      if (sum > LIM * 2)
        return sum;
    }
    return sum;
  }

  long long totalSize() const {
    long long sum = 0;
    for (const Seq& s : seqs) {
      long long len = (s.type == 'E') ? (long long)s.vals.size() : s.c;
      if (sum + len < sum || sum + len > (long long)2e18)
        return (long long)2e18;
      sum += len;
    }
    return sum;
  }

  long long kth(long long k) const {
    long long lo = 1, hi = LIM;
    while (lo < hi) {
      long long mid = lo + (hi - lo) / 2;
      if (countLE((i128)mid) >= k)
        hi = mid;
      else
        lo = mid + 1;
    }
    return lo;
  }

  void parse(const string& line) {
    Seq s;
    s.type = line[0];
    stringstream ss(line.substr(2));
    if (s.type == 'E') {
      string tok;
      while (ss >> tok)
        s.vals.push_back(parse128(tok));
      sort(s.vals.begin(), s.vals.end());
    } else {
      string t1, t2, t3;
      ss >> t1 >> t2 >> t3;
      s.a = parse128(t1);
      s.b = parse128(t2);
      s.c = 0;
      for (char c : t3)
        s.c = s.c * 10 + (c - '0');
    }
    seqs.push_back(s);
  }

public:
  vector<int> getVal(vector<string> seqLines, vector<int> positions) {
    seqs.clear();
    for (const string& line : seqLines)
      parse(line);

    long long total = totalSize();
    vector<int> ans;
    ans.reserve(positions.size());

    for (int pos : positions) {
      long long k = pos;
      if (k > total) {
        ans.push_back(-1);
        continue;
      }
      if (countLE((i128)LIM) < k) {
        ans.push_back(-1);
        continue;
      }
      long long val = kth(k);
      ans.push_back((int)val);
    }
    return ans;
  }
};

#ifdef LOCAL_TEST
static bool eq(const vector<int>& a, const vector<int>& b) {
  if (a.size() != b.size())
    return false;
  for (size_t i = 0; i < a.size(); ++i)
    if (a[i] != b[i])
      return false;
  return true;
}

static void run(const vector<string>& seqs, const vector<int>& pos, const vector<int>& want) {
  SequenceMerger sm;
  vector<int> got = sm.getVal(seqs, pos);
  if (!eq(got, want)) {
    cerr << "FAIL\n  got:";
    for (int x : got)
      cerr << ' ' << x;
    cerr << "\n  want:";
    for (int x : want)
      cerr << ' ' << x;
    cerr << '\n';
    exit(1);
  }
}

int main() {
  run({"E 1 1000000000 1000000001"}, {1, 2, 3, 4}, {1, 1000000000, -1, -1});
  run({"A 1 1 10", "G 1 2 4"}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
      {1, 1, 2, 2, 3, 4, 4, 5, 6, 7, 8, 8, 9, 10, -1});
  run({"A 1000000000 1000000000 1000000000", "G 100000000000000000 1000000000000 100000000000000",
       "E 1000000000000000 10000000 10 1111"},
      {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, {10, 1111, 10000000, 1000000000, -1, -1, -1, -1, -1, -1});
  run({"G 1 1 999999999", "E 2"}, {1, 999999999, 1000000000}, {1, 1, 2});
  run({"A 100 341 1524", "G 1 3 45235", "E 653 87 12341 8785 123 543"},
      {100000, 1, 10, 10000, 100, 1000}, {-1, 1, 441, -1, 28403, 334621});
  run({"A 59376271 6172 30", "A 4 409311 99", "A 7896904 786 75461", "A 418468085 43 2405851",
       "A 9967 8 34735", "A 50560826 2594 7"},
      {4944559, 60996, 2930449, 470}, {-1, 28482244, -1, 13711});
  run({"A 44029644 8 96", "A 162 19737 4", "A 14 1000000003 535900374",
       "A 661 699464693586970551511315520929901211 6849", "A 99147881 5988 511",
       "A 363059805 8167921 9", "A 737905192 223 11727", "A 696 1151 905",
       "A 574754 1343950992 999999994", "A 618 444307815782187537082992354 66693689"},
      {6, 999999999, 8, 3230174, 950, 9, 2647554, 976743859, 9, 779687},
      {1847, -1, 4149, -1, 44029932, 5300, -1, -1, 5300, -1});
  cerr << "all local examples OK\n";
  return 0;
}
#endif
