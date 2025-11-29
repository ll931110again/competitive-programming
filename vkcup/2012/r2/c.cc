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
 
int n,l,v1,v2;
int pos[maxn];
double ret[maxn];
vector< pair<double,int> > event;
 
int main() {
  scanf("%d %d %d %d", &n, &l, &v1, &v2);
  for (int i = 0; i < n; i++) scanf("%d", &pos[i]);
  double limit = 1.0 * l * v2/(v1 + v2);
  for (int i = 0; i < n; i++) {
    double low = pos[i],high = pos[i] + limit;
    if (high <= 2 * l) {
      event.push_back(make_pair(low,-1));
      event.push_back(make_pair(high,1));
    }
    else {
      event.push_back(make_pair(low,-1));
      event.push_back(make_pair(2.0 * l,1));
      event.push_back(make_pair(0.0,-1));
      event.push_back(make_pair(high - 2 * l,1));
    }
  }
  event.push_back(make_pair(2 * l,-1));
  sort(event.begin(),event.end());
  double last = 0;
  int cnt = 0;
  for (int i = 0; i < event.size(); i++) {
    ret[cnt] += event[i].first - last;
    cnt -= event[i].second;
    last = event[i].first;
  }
  for (int i = 0; i <= n; i++) printf("%.11lf\n", ret[i] * 0.5/l);
}
