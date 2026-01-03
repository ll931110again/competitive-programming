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
#define maxn 1000005
using namespace std;

int n, a[maxn], freq[maxn];
int dp[maxn];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  
  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> a[i];
    freq[a[i]]++;
  }

  int ans = 0;
  for (int i = 1; i < maxn; i++) if (freq[i]) {
    dp[i] = max(dp[i], freq[i]);
    ans = max(ans, dp[i]);

    for (int j = i + i; j < maxn; j += i) if (freq[j]) {
      dp[j] = max(dp[j], dp[i] + freq[j]);
    }
  }

  cout << ans << endl;
  return 0;
}
