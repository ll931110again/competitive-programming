#include <bits/stdc++.h>
using namespace std;

class EqualizeStrings {
public:
  string getEq(string s, string t) {
    int n = s.size();
    string ret(n, 'a');
    for (int i = 0; i < n; i++) {
      int bestCost = 100;
      char best = 'a';
      for (char c = 'a'; c <= 'z'; c++) {
        int cost = dist(s[i], c) + dist(t[i], c);
        if (cost < bestCost || (cost == bestCost && c < best)) {
          bestCost = cost;
          best = c;
        }
      }
      ret[i] = best;
    }
    return ret;
  }

private:
  int dist(char a, char b) {
    int d = abs(a - b);
    return min(d, 26 - d);
  }
};

#ifdef LOCAL_TEST
int main() {
  auto test = [](const string& s, const string& t, const string& expected) {
    string got = EqualizeStrings().getEq(s, t);
    if (got != expected) {
      cerr << "FAIL s=\"" << s << "\" t=\"" << t << "\": got \"" << got << "\" expected \""
           << expected << "\"" << endl;
      exit(1);
    }
  };
  test("cat", "dog", "caa");
  test("abcdefghijklmnopqrstuvwxyz", "bcdefghijklmnopqrstuvwxyza", "abcdefghijklmnopqrstuvwxya");
  test("programmingcompetitionsrule", "programmingcompetitionsrule", "programmingcompetitionsrule");
  test("topcoderopen", "onlinerounds", "onlcndaoondn");
  cerr << "all tests passed" << endl;
}
#endif
