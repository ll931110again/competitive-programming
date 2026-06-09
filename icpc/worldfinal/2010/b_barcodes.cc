#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr double TOL = 0.05;

int char_weight(char ch) {
  if ('0' <= ch && ch <= '9')
    return ch - '0';
  if (ch == '-')
    return 10;
  return -1;
}

char weight_to_char(int w) {
  if (0 <= w && w <= 9)
    return char('0' + w);
  if (w == 10)
    return '-';
  return '?';
}

int compute_c(const string& msg) {
  int n = (int)msg.size();
  long long s = 0;
  for (int i = 0; i < n; i++) {
    int w = char_weight(msg[i]);
    int coef = ((n - 1 - i) % 10) + 1;
    s += 1LL * coef * w;
  }
  return int(s % 11);
}

int compute_k(const string& msg, char Cch) {
  int n = (int)msg.size();
  long long s = 0;
  for (int i = 0; i < n; i++) {
    int w = char_weight(msg[i]);
    int coef = ((n - i) % 9) + 1;
    s += 1LL * coef * w;
  }
  // include C as c_{n+1} with i=n+1 -> coef = ((n-(n))%9)+1 = 1
  s += 1LL * 1 * char_weight(Cch);
  return int(s % 11);
}

struct Interval {
  double lo, hi;
};

vector<Interval> intersect_sets(const vector<Interval>& set, const vector<Interval>& add) {
  vector<Interval> out;
  for (auto a : set) {
    for (auto b : add) {
      double lo = max(a.lo, b.lo);
      double hi = min(a.hi, b.hi);
      if (lo <= hi)
        out.push_back({lo, hi});
    }
  }
  sort(out.begin(), out.end(), [](const Interval& x, const Interval& y) { return x.lo < y.lo; });
  // merge
  vector<Interval> merged;
  for (auto iv : out) {
    if (merged.empty() || iv.lo > merged.back().hi)
      merged.push_back(iv);
    else
      merged.back().hi = max(merged.back().hi, iv.hi);
  }
  return merged;
}

vector<Interval> feasible_unit_intervals(const vector<int>& d) {
  // Unit width u must satisfy each di in union of:
  // narrow: di in [0.95u,1.05u] => u in [di/1.05, di/0.95]
  // wide  : di in [1.9u,2.1u]  => u in [di/2.1,  di/1.9]
  vector<Interval> cur = {{0.0, 1e18}};
  for (int di : d) {
    vector<Interval> uni;
    uni.push_back({di / (1.0 + TOL), di / (1.0 - TOL)});
    uni.push_back(
        {di / (2.0 + 2.0 * TOL), di / (2.0 - 2.0 * TOL)}); // 2u with +/-5% => factor 2*(1±TOL)
    cur = intersect_sets(cur, uni);
    if (cur.empty())
      return {};
  }
  return cur;
}

bool fits_narrow(int di, double u) {
  return di >= (1.0 - TOL) * u - 1e-9 && di <= (1.0 + TOL) * u + 1e-9;
}
bool fits_wide(int di, double u) {
  return di >= (2.0 - 2.0 * TOL) * u - 1e-9 && di <= (2.0 + 2.0 * TOL) * u + 1e-9;
}

unordered_map<int, char> build_pattern_map() {
  unordered_map<int, char> mp;
  auto add = [&](char ch, const string& bits) {
    int x = 0;
    for (char b : bits)
      x = (x << 1) | (b == '1');
    mp[x] = ch;
  };
  add('0', "00001");
  add('1', "10001");
  add('2', "01001");
  add('3', "11000");
  add('4', "00101");
  add('5', "10100");
  add('6', "01100");
  add('7', "00011");
  add('8', "10010");
  add('9', "10000");
  add('-', "00100");
  add('S', "00110"); // Start/Stop sentinel
  return mp;
}

struct DecodeRes {
  bool ok;
  vector<char> chars;
  DecodeRes() : ok(false), chars() {}
  explicit DecodeRes(const vector<char>& v) : ok(true), chars(v) {}
};

DecodeRes try_decode_with_unit(const vector<int>& d, double u) {
  unordered_map<int, char> pat = build_pattern_map();

  int m = (int)d.size();
  if ((m + 1) % 6 != 0)
    return DecodeRes();
  int t = (m + 1) / 6; // total characters including start/stop and checks
  if (t < 5)
    return DecodeRes();

  vector<char> chars;
  chars.reserve(t);

  for (int k = 0; k < t; k++) {
    int base = 6 * k;
    int code = 0;
    for (int j = 0; j < 5; j++) {
      int di = d[base + j];
      bool n = fits_narrow(di, u);
      bool w = fits_wide(di, u);
      if (!n && !w)
        return DecodeRes();
      if (n && w) {
        // ambiguous width at this u; treat as invalid at this u to keep
        // deterministic
        return DecodeRes();
      }
      code = (code << 1) | (w ? 1 : 0);
    }
    auto it = pat.find(code);
    if (it == pat.end())
      return DecodeRes();
    chars.push_back(it->second);

    if (k != t - 1) {
      int sep = d[base + 5];
      if (!fits_narrow(sep, u))
        return DecodeRes(); // separator must be narrow
    }
  }

  if (chars.front() != 'S' || chars.back() != 'S')
    return DecodeRes();
  return DecodeRes(chars);
}

DecodeRes decode_either_direction(const vector<int>& d) {
  struct TryOneDir {
    DecodeRes run(const vector<int>& dd) {
      auto intervals = feasible_unit_intervals(dd);
      for (const auto& iv : intervals) {
        // Use midpoint unit; if that causes ambiguity, try endpoints-ish too.
        vector<double> candidates;
        candidates.push_back((iv.lo + iv.hi) * 0.5);
        candidates.push_back(iv.lo);
        candidates.push_back(iv.hi);
        for (double u : candidates) {
          if (u <= 0)
            continue;
          DecodeRes res = try_decode_with_unit(dd, u);
          if (res.ok)
            return res;
        }
      }
      return DecodeRes();
    }
  };

  DecodeRes fwd = TryOneDir::run(d);
  if (fwd.ok)
    return fwd;
  vector<int> rev = d;
  reverse(rev.begin(), rev.end());
  DecodeRes bwd = TryOneDir::run(rev);
  if (bwd.ok)
    return bwd;
  return DecodeRes();
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int m;
  int case_num = 1;
  while (cin >> m) {
    if (m == 0)
      break;
    vector<int> d(m);
    for (int i = 0; i < m; i++)
      cin >> d[i];

    string result;
    DecodeRes decoded = decode_either_direction(d);
    if (!decoded.ok) {
      result = "bad code";
    } else {
      const vector<char>& seq = decoded.chars; // S + msg + C + K + S
      int t = (int)seq.size();
      if (t < 5) {
        result = "bad code";
      } else {
        string msg;
        for (int i = 1; i <= t - 4; i++) {
          if (seq[i] == 'S') {
            msg.clear();
            break;
          }
          msg.push_back(seq[i]);
        }
        if (msg.empty()) {
          result = "bad code";
        } else {
          char Cch = seq[t - 3];
          char Kch = seq[t - 2];
          if (Cch == 'S' || Kch == 'S') {
            result = "bad code";
          } else {
            int c_w = compute_c(msg);
            char expected_c = weight_to_char(c_w);
            if (expected_c != Cch) {
              result = "bad C";
            } else {
              int k_w = compute_k(msg, Cch);
              char expected_k = weight_to_char(k_w);
              if (expected_k != Kch)
                result = "bad K";
              else
                result = msg;
            }
          }
        }
      }
    }

    cout << "Case " << case_num++ << ": " << result << "\n";
  }
  return 0;
}
