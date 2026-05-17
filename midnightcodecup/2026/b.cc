// Midnight Code Cup 2026 - B. Lost Cursor
//
// Read n lines of length n: '.' or '0' = white, '#' or '1' = black.
// Output one string of U,D,L,R that guarantees every white start hits black
// (grid moves clamp at borders). Length <= 5000.
//
// Strategy: sync all positions to a corner using exactly (n-1) moves on each
// of the two axis directions (any order with that multiset lands everyone on
// the corner), then Manhattan path to a black cell. Try block orders,
// alternating interleavings (ab)^k / (ba)^k, and row-first vs column-first
// suffix tie-breaking. Score = shortest prefix where every white has hit black.

#include <bits/stdc++.h>
using namespace std;

static int n;
static vector<string> g;
static vector<pair<int, int>> whites;
static vector<pair<int, int>> blacks;

static bool is_black(int r, int c) {
  char x = g[r][c];
  return x == '1' || x == '#';
}

static void step(int& r, int& c, char ch) {
  if (ch == 'U')
    r = max(0, r - 1);
  else if (ch == 'D')
    r = min(n - 1, r + 1);
  else if (ch == 'L')
    c = max(0, c - 1);
  else if (ch == 'R')
    c = min(n - 1, c + 1);
}

// 0: a^k b^k, 1: b^k a^k, 2: (ab)^k, 3: (ba)^k — each has k of each axis move.
static string sync_corner_variant(const string& corner, int variant) {
  int k = n - 1;
  char a = 'U', b = 'L';
  if (corner == "TL") {
    a = 'U';
    b = 'L';
  } else if (corner == "TR") {
    a = 'U';
    b = 'R';
  } else if (corner == "BL") {
    a = 'D';
    b = 'L';
  } else if (corner == "BR") {
    a = 'D';
    b = 'R';
  }
  string s;
  s.reserve(2 * k);
  if (variant == 0) {
    s.append(k, a);
    s.append(k, b);
  } else if (variant == 1) {
    s.append(k, b);
    s.append(k, a);
  } else if (variant == 2) {
    for (int i = 0; i < k; ++i) {
      s += a;
      s += b;
    }
  } else {
    for (int i = 0; i < k; ++i) {
      s += b;
      s += a;
    }
  }
  return s;
}

static pair<int, int> corner_rc(const string& corner) {
  if (corner == "TL")
    return {0, 0};
  if (corner == "TR")
    return {0, n - 1};
  if (corner == "BL")
    return {n - 1, 0};
  return {n - 1, n - 1};
}

static string manhattan_path(int r, int c, int tr, int tc, bool row_first) {
  string out;
  while (r != tr || c != tc) {
    if (row_first) {
      if (r < tr) {
        out += 'D';
        step(r, c, 'D');
      } else if (r > tr) {
        out += 'U';
        step(r, c, 'U');
      } else if (c < tc) {
        out += 'R';
        step(r, c, 'R');
      } else {
        out += 'L';
        step(r, c, 'L');
      }
    } else {
      if (c < tc) {
        out += 'R';
        step(r, c, 'R');
      } else if (c > tc) {
        out += 'L';
        step(r, c, 'L');
      } else if (r < tr) {
        out += 'D';
        step(r, c, 'D');
      } else {
        out += 'U';
        step(r, c, 'U');
      }
    }
  }
  return out;
}

static string build_candidate(const string& corner, int sync_variant, bool row_first_manhattan,
                              int br, int bc) {
  auto [cr, cc] = corner_rc(corner);
  string s = sync_corner_variant(corner, sync_variant);
  int r = cr, c = cc;
  s += manhattan_path(r, c, br, bc, row_first_manhattan);
  return s;
}

// Returns nullopt if some white never hits black. Otherwise returns the
// effective length (max consumed over whites).
// If max_eff_cap < INT_MAX, abandons when effective length would exceed
// max_eff_cap (cannot beat current best strict length; ties handled by caller).
static optional<int> analyze(const string& moves, int max_eff_cap = INT_MAX) {
  int global_need = 0;
  for (auto [r0, c0] : whites) {
    int r = r0, c = c0;
    int consumed = 0;
    for (char ch : moves) {
      if (is_black(r, c))
        break;
      step(r, c, ch);
      ++consumed;
      if (max_eff_cap < INT_MAX && consumed > max_eff_cap && !is_black(r, c))
        return nullopt;
    }
    if (!is_black(r, c))
      return nullopt;
    if (max_eff_cap < INT_MAX && consumed > max_eff_cap)
      return nullopt;
    global_need = max(global_need, consumed);
    if (max_eff_cap < INT_MAX && global_need > max_eff_cap)
      return nullopt;
  }
  return global_need;
}

static string truncate_to(const string& moves, int t) {
  if (t <= 0)
    return "";
  return moves.substr(0, min(t, (int)moves.size()));
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string line;
  while (cin >> line)
    g.push_back(line);
  n = (int)g.size();
  if (n == 0) {
    cout << "\n";
    return 0;
  }

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) {
      if (is_black(i, j))
        blacks.push_back({i, j});
      else
        whites.push_back({i, j});
    }

  const int kMaxLen = 5000;
  // Nearest black minimizes full string length, but NOT always effective
  // length after truncation — probe several closest targets per layout.
  const int kProbeBlacks = 28;
  const vector<string> corners = {"TL", "TR", "BL", "BR"};
  string best_out;
  int best_eff = INT_MAX;

  for (const string& cor : corners) {
    pair<int, int> crc = corner_rc(cor);
    int cr = crc.first;
    int cc = crc.second;
    vector<pair<int, int>> ranked = blacks;
    sort(ranked.begin(), ranked.end(), [&](const pair<int, int>& a, const pair<int, int>& b) {
      int da = abs(a.first - cr) + abs(a.second - cc);
      int db = abs(b.first - cr) + abs(b.second - cc);
      return da < db;
    });
    for (int sync_variant = 0; sync_variant < 4; ++sync_variant) {
      for (bool row_first : {true, false}) {
        int tried = 0;
        for (auto [br, bc] : ranked) {
          if (tried >= kProbeBlacks)
            break;
          ++tried;
          string cand = build_candidate(cor, sync_variant, row_first, br, bc);
          if ((int)cand.size() > kMaxLen)
            continue;
          auto eff_opt = analyze(cand, best_eff);
          if (!eff_opt)
            continue;
          int eff = *eff_opt;
          string out = truncate_to(cand, eff);
          if (eff < best_eff || (eff == best_eff && out < best_out)) {
            best_eff = eff;
            best_out = std::move(out);
          }
        }
      }
    }
  }

  if (best_out.empty())
    cout << "\n";
  else
    cout << best_out << "\n";
  return 0;
}
