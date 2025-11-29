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
#define maxn 100005
using namespace std;
 
int n,m,x,y;
pair<int,int> a[maxn],b[maxn];
 
int main() {
  scanf("%d %d %d %d", &n, &m, &x, &y);
  for (int i = 1; i <= n; i++) {
    scanf("%d", &a[i].first);
    a[i].second = i;
  }
  for (int i = 1; i <= m; i++) {
    scanf("%d", &b[i].first);
    b[i].second = i;
  }
  sort(a + 1,a + n + 1);
  sort(b + 1,b + m + 1);
 
  vector< pair<int,int> > ret;
  int fa = 1,fb = 1;
  while (fa <= n && fb <= m) {
    if (a[fa].first - x <= b[fb].first && b[fb].first <= a[fa].first + y) {
      ret.push_back(make_pair(a[fa].second,b[fb].second));
      fa++;
      fb++;
      continue;
    }
    if (a[fa].first - x > b[fb].first) fb++;
    else fa++;
  }
 
  printf("%d\n", (int) ret.size());
  for (int i = 0; i < ret.size(); i++) printf("%d %d\n", ret[i].first, ret[i].second);
}
