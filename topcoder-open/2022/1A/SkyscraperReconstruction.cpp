#include <bits/stdc++.h>
using namespace std;

class SkyscraperReconstruction {
public:
  vector<int> reconstruct(string visibility) {
    int N = visibility.size();
    vector<int> ans(N);
    int height = N;
    for (int i = N - 1; i >= 0; i--)
      if (visibility[i] == 'O') {
        for (int j = i; j < N; j++)
          if (ans[j] == 0) {
            ans[j] = height--;
          } else {
            break;
          }
      }
    return ans;
  }
};

// Powered by KawigiEdit 2.1.4 (beta) modified by pivanof!
