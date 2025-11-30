#ifdef ONLINE_JUDGE
  #include <bits/stdc++.h>
#endif
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#define maxn 105
using namespace std;
 
long long f[maxn], g[maxn];
int n;
string s;

void populate() {
  for (int i = 0; i + 1 < n; i++) {
    if (s[i] == 'A' && s[i + 1] == 'A') {
      f[i + 1] += f[i];
      f[i + 1] += g[i];
    } else if (s[i] == 'A' && s[i + 1] == 'B') {
      f[i + 1] += f[i];
      f[i + 1] += g[i];
      g[i + 1] += g[i];
    } else if (s[i] == 'B' && s[i + 1] == 'A') {
      g[i + 1] += f[i];
      f[i + 1] += g[i];
    } else if (s[i] == 'B' && s[i + 1] == 'B') {
      f[i + 1] += g[i];
      g[i + 1] += g[i];
    }
  }
}

int main() {
  cin >> s;
  n = s.size();

  long long ans = 0;
  f[0] = 1;
  populate();

  if (s[n - 1] == 'B') {
    ans += g[n - 1];
  } else {
    ans += f[n - 1] + g[n - 1];
  }

  memset(f, 0, sizeof f);
  memset(g, 0, sizeof g);
  g[0] = 1;
  populate();

  if (s[0] == 'A' && s[n - 1] == 'B') {
    ans += f[n - 1];
  } else if (s[0] == 'B') {
    ans += g[n - 1];
  }

  cout << ans << endl;
  return 0;
}
