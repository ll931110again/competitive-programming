#include <bits/stdc++.h>
using namespace std;

class FloodFill3D {
public:
  long long countBlack(vector<string> SArray, vector<string> TArray, vector<string> UArray) {
    string S, T, U;
    for (const string& p : SArray)
      S += p;
    for (const string& p : TArray)
      T += p;
    for (const string& p : UArray)
      U += p;
    const int n = (int)S.size(), m = (int)T.size(), l = (int)U.size();
    long long total = 1LL * n * m * l;

    long long white = 0;
    long long interiorWhite = 0;

    for (char c : {'o', 'x'}) {
      long long cntS = 0, cntT = 0, cntU = 0;
      long long inS = 0, inT = 0, inU = 0;

      auto scan = [&](const string& X, int len, long long& cnt, long long& interiorSum) {
        for (int i = 0; i < len;) {
          if (X[i] != c) {
            ++i;
            continue;
          }
          int j = i;
          while (j < len && X[j] == c)
            ++j;
          int L = i, R = j - 1;
          int segLen = R - L + 1;
          cnt += segLen;
          if (L > 0 && R < len - 1)
            interiorSum += segLen;
          i = j;
        }
      };

      scan(S, n, cntS, inS);
      scan(T, m, cntT, inT);
      scan(U, l, cntU, inU);

      white += cntS * cntT * cntU;
      interiorWhite += inS * inT * inU;
    }

    return total - white + interiorWhite;
  }
};
