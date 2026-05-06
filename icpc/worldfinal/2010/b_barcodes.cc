#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

static constexpr double TOL = 0.05;

static int charWeight(char ch) {
    if ('0' <= ch && ch <= '9') return ch - '0';
    if (ch == '-') return 10;
    return -1;
}

static char weightToChar(int w) {
    if (0 <= w && w <= 9) return char('0' + w);
    if (w == 10) return '-';
    return '?';
}

static int computeC(const string& msg) {
    int n = (int)msg.size();
    long long s = 0;
    for (int i = 0; i < n; i++) {
        int w = charWeight(msg[i]);
        int coef = ((n - 1 - i) % 10) + 1;
        s += 1LL * coef * w;
    }
    return int(s % 11);
}

static int computeK(const string& msg, char Cch) {
    int n = (int)msg.size();
    long long s = 0;
    for (int i = 0; i < n; i++) {
        int w = charWeight(msg[i]);
        int coef = ((n - i) % 9) + 1;
        s += 1LL * coef * w;
    }
    // include C as c_{n+1} with i=n+1 -> coef = ((n-(n))%9)+1 = 1
    s += 1LL * 1 * charWeight(Cch);
    return int(s % 11);
}

struct Interval {
    double lo, hi;
};

static vector<Interval> intersectSets(const vector<Interval>& set, const vector<Interval>& add) {
    vector<Interval> out;
    for (auto a : set) {
        for (auto b : add) {
            double lo = max(a.lo, b.lo);
            double hi = min(a.hi, b.hi);
            if (lo <= hi) out.push_back({lo, hi});
        }
    }
    sort(out.begin(), out.end(), [](const Interval& x, const Interval& y) { return x.lo < y.lo; });
    // merge
    vector<Interval> merged;
    for (auto iv : out) {
        if (merged.empty() || iv.lo > merged.back().hi) merged.push_back(iv);
        else merged.back().hi = max(merged.back().hi, iv.hi);
    }
    return merged;
}

static vector<Interval> feasibleUnitIntervals(const vector<int>& d) {
    // Unit width u must satisfy each di in union of:
    // narrow: di in [0.95u,1.05u] => u in [di/1.05, di/0.95]
    // wide  : di in [1.9u,2.1u]  => u in [di/2.1,  di/1.9]
    vector<Interval> cur = {{0.0, 1e18}};
    for (int di : d) {
        vector<Interval> uni;
        uni.push_back({di / (1.0 + TOL), di / (1.0 - TOL)});
        uni.push_back({di / (2.0 + 2.0 * TOL), di / (2.0 - 2.0 * TOL)}); // 2u with +/-5% => factor 2*(1±TOL)
        cur = intersectSets(cur, uni);
        if (cur.empty()) return {};
    }
    return cur;
}

static bool fitsNarrow(int di, double u) { return di >= (1.0 - TOL) * u - 1e-9 && di <= (1.0 + TOL) * u + 1e-9; }
static bool fitsWide(int di, double u) { return di >= (2.0 - 2.0 * TOL) * u - 1e-9 && di <= (2.0 + 2.0 * TOL) * u + 1e-9; }

static unordered_map<int, char> buildPatternMap() {
    unordered_map<int, char> mp;
    auto add = [&](char ch, const string& bits) {
        int x = 0;
        for (char b : bits) x = (x << 1) | (b == '1');
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

static DecodeRes tryDecodeWithUnit(const vector<int>& d, double u) {
    static unordered_map<int, char> pat = buildPatternMap();

    int m = (int)d.size();
    if ((m + 1) % 6 != 0) return DecodeRes();
    int t = (m + 1) / 6; // total characters including start/stop and checks
    if (t < 5) return DecodeRes();

    vector<char> chars;
    chars.reserve(t);

    for (int k = 0; k < t; k++) {
        int base = 6 * k;
        int code = 0;
        for (int j = 0; j < 5; j++) {
            int di = d[base + j];
            bool n = fitsNarrow(di, u);
            bool w = fitsWide(di, u);
            if (!n && !w) return DecodeRes();
            if (n && w) {
                // ambiguous width at this u; treat as invalid at this u to keep deterministic
                return DecodeRes();
            }
            code = (code << 1) | (w ? 1 : 0);
        }
        auto it = pat.find(code);
        if (it == pat.end()) return DecodeRes();
        chars.push_back(it->second);

        if (k != t - 1) {
            int sep = d[base + 5];
            if (!fitsNarrow(sep, u)) return DecodeRes(); // separator must be narrow
        }
    }

    if (chars.front() != 'S' || chars.back() != 'S') return DecodeRes();
    return DecodeRes(chars);
}

static DecodeRes decodeEitherDirection(const vector<int>& d) {
    struct TryOneDir {
        static DecodeRes run(const vector<int>& dd) {
        auto intervals = feasibleUnitIntervals(dd);
        for (const auto& iv : intervals) {
            // Use midpoint unit; if that causes ambiguity, try endpoints-ish too.
            vector<double> candidates;
            candidates.push_back((iv.lo + iv.hi) * 0.5);
            candidates.push_back(iv.lo);
            candidates.push_back(iv.hi);
            for (double u : candidates) {
                if (u <= 0) continue;
                DecodeRes res = tryDecodeWithUnit(dd, u);
                if (res.ok) return res;
            }
        }
        return DecodeRes();
        }
    };

    DecodeRes fwd = TryOneDir::run(d);
    if (fwd.ok) return fwd;
    vector<int> rev = d;
    reverse(rev.begin(), rev.end());
    DecodeRes bwd = TryOneDir::run(rev);
    if (bwd.ok) return bwd;
    return DecodeRes();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    int caseNum = 1;
    while (cin >> m) {
        if (m == 0) break;
        vector<int> d(m);
        for (int i = 0; i < m; i++) cin >> d[i];

        string result;
        DecodeRes decoded = decodeEitherDirection(d);
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
                        int cW = computeC(msg);
                        char expectedC = weightToChar(cW);
                        if (expectedC != Cch) {
                            result = "bad C";
                        } else {
                            int kW = computeK(msg, Cch);
                            char expectedK = weightToChar(kW);
                            if (expectedK != Kch) result = "bad K";
                            else result = msg;
                        }
                    }
                }
            }
        }

        cout << "Case " << caseNum++ << ": " << result << "\n";
    }
    return 0;
}

