#include <bits/stdc++.h>
using namespace std;

class RepresentableNumbers {
  static const int ODD[5];
  static const int SUM2[25];
  static const int NSUM2;

  struct Step {
    int dig, carry;
  };
  vector<Step> both[3], one[3], carryOnly[3];

  void initTrans() {
    for (int c = 0; c < 3; c++) {
      both[c].clear();
      one[c].clear();
      carryOnly[c].clear();
      for (int i = 0; i < NSUM2; i++) {
        int t = SUM2[i] + c;
        both[c].push_back({t % 10, t / 10});
      }
      for (int i = 0; i < 5; i++) {
        int t = ODD[i] + c;
        one[c].push_back({t % 10, t / 10});
      }
      if (c)
        carryOnly[c].push_back({c % 10, c / 10});
    }
  }

  const vector<Step>& edges(int carry, int al, int bl) const {
    if (al > 0 && bl > 0)
      return both[carry];
    if (al > 0 || bl > 0)
      return one[carry];
    return carryOnly[carry];
  }

  bool canRepresent(const vector<int>& lsb) const {
    int L = (int)lsb.size();
    for (int la = 1; la <= L + 1; la++) {
      for (int lb = 1; lb <= L + 1; lb++) {
        vector<char> seen((L + 2) * 3 * 12 * 12, 0);
        deque<int> q;
        auto enc = [&](int pos, int carry, int al, int bl) {
          return (((pos * 3 + carry) * 12 + al) * 12 + lb);
        };
        seen[enc(0, 0, la, lb)] = 1;
        q.push_back(enc(0, 0, la, lb));
        while (!q.empty()) {
          int st = q.front();
          q.pop_front();
          int pos = st / (3 * 15 * 15);
          int carry = (st / (15 * 15)) % 3;
          int al = (st / 15) % 15;
          int bl = st % 15;
          if (pos == L)
            return carry == 0;
          int d = lsb[pos];
          for (const Step& e : edges(carry, al, bl)) {
            if (e.dig != d)
              continue;
            int nal = al, nbl = bl;
            if (al > 0 && bl > 0)
              nal--, nbl--;
            else if (al > 0)
              nal--;
            else if (bl > 0)
              nbl--;
            int nst = enc(pos + 1, e.carry, nal, nbl);
            if (!seen[nst]) {
              seen[nst] = 1;
              q.push_back(nst);
            }
          }
        }
      }
    }
    return false;
  }

  bool minSuffix(vector<int>& msb, int pos, int carry, int al, int bl, bool tight) const {
    int n = (int)msb.size();
    if (pos == n)
      return carry == 0;
    int lo = tight ? msb[pos] : 0;
    for (int dig = lo; dig <= 9; dig++) {
      bool ntight = tight && dig == msb[pos];
      for (const Step& e : edges(carry, al, bl)) {
        if (e.dig != dig)
          continue;
        int nal = al, nbl = bl;
        if (al > 0 && bl > 0)
          nal--, nbl--;
        else if (al > 0)
          nal--;
        else if (bl > 0)
          nbl--;
        int saved = msb[pos];
        msb[pos] = dig;
        if (minSuffix(msb, pos + 1, e.carry, nal, nbl, ntight)) {
          return true;
        }
        msb[pos] = saved;
      }
    }
    return false;
  }

  bool fillMinSuffix(vector<int>& msb, int pos) const {
    for (int la = 1; la <= (int)msb.size() - pos + 1; la++) {
      for (int lb = 1; lb <= (int)msb.size() - pos + 1; lb++) {
        vector<int> tmp = msb;
        if (minSuffix(tmp, pos, 0, la, lb, false)) {
          msb = tmp;
          return true;
        }
      }
    }
    return false;
  }

  int toInt(const vector<int>& msb) const {
    int v = 0;
    for (int d : msb)
      v = v * 10 + d;
    return v;
  }

  vector<int> toLsb(int x) const {
    vector<int> lsb;
    if (x == 0)
      lsb.push_back(0);
    while (x) {
      lsb.push_back(x % 10);
      x /= 10;
    }
    return lsb;
  }

public:
  RepresentableNumbers() {
    initTrans();
  }

  int getNext(int X) {
    if (canRepresent(toLsb(X)))
      return X;

    string s = to_string(X);
    int n = (int)s.size();
    vector<int> msb(n);
    for (int i = 0; i < n; i++)
      msb[i] = s[i] - '0';

    int ans = 2000000000;

    for (int i = n - 1; i >= 0; i--) {
      for (int dig = msb[i] + 1; dig <= 9; dig++) {
        vector<int> cand = msb;
        cand[i] = dig;
        if (fillMinSuffix(cand, i + 1))
          ans = min(ans, toInt(cand));
      }
    }

    long long p10 = 1;
    for (int i = 0; i < n; i++)
      p10 *= 10;
    if (p10 >= X && p10 <= 2000000000LL && canRepresent(toLsb((int)p10)))
      ans = min(ans, (int)p10);

    return ans;
  }
};

const int RepresentableNumbers::ODD[5] = {1, 3, 5, 7, 9};
const int RepresentableNumbers::SUM2[25] = {2,  4,  6, 8,  10, 4,  6,  8,  10, 12, 6,  8, 10,
                                            12, 14, 8, 10, 12, 14, 16, 10, 12, 14, 16, 18};
const int RepresentableNumbers::NSUM2 = 25;
