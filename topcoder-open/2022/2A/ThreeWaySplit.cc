#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
 
using namespace std;
 
map<long long, pair<long long, long long> > mp_a, mp_b;
 
void build(vector<int> values, map<long long, pair<long long, long long> >& ans) {
  ans.clear();
  int N = values.size();
  
  int p3 = 1;
  for (int i = 0; i < N; i++) {
    p3 *= 3;
  }
  for (int mask = 0; mask < p3; mask++) {
    long long delta = 0, a = 0;
    int m = mask;
    for (int i = 0; i < N; i++) {
      int mi = m % 3;
      m /= 3;
      if (mi == 1) {
        delta += values[i];
        a += values[i];
      } else if (mi == 2) {
        delta -= values[i];
      }
    }
    if (!ans.count(delta) || ans[delta].first < a) {
      ans[delta] = make_pair(a, mask);
    }
  }
}
 
 
class ThreeWaySplit {
public:
  string split(vector <int> loot) {
    int N = loot.size();
    vector<int> part_a, part_b;
    for (int i = 0; i < N; i++) if (i < N/2) {
      part_a.push_back(loot[i]);
    } else {
      part_b.push_back(loot[i]);
    }
    
    build(part_a, mp_a);
    build(part_b, mp_b);
    
    long long ret = 0, pos_a = 0, pos_b = 0;
    for (auto it : mp_a) {
      long long d = it.first;
      if (mp_b.count(-d)) {
        long long xa = mp_a[d].first, pa = mp_a[d].second;
        long long xb = mp_b[-d].first, pb = mp_b[-d].second;
        // cout << d << ' ' << xa << ' ' << pa << ' ' << xb << ' ' << pb << endl;
        if (xa + xb > ret) {
          ret = xa + xb;
          pos_a = pa;
          pos_b = pb;
        }
      }
    }
    
    auto getChar = [](int mi) {
      if (mi == 0) {
        return 'C';
      } else if (mi == 1) {
        return 'A';
      } else {
        return 'B';
      }
    };
    
    string ans;
    for (int i = 0; i < part_a.size(); i++) {
      int mi = pos_a % 3;
      pos_a /= 3;
      ans += getChar(mi);
    }
    for (int i = 0; i < part_b.size(); i++) {
      int mi = pos_b % 3;
      pos_b /= 3;
      ans += getChar(mi);
    }
    
    return ans;
  }
};
 
 
 
//Powered by KawigiEdit 2.1.4 (beta) modified by pivanof!
