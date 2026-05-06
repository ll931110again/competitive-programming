#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

class RaceCondition {
public:
  string minimize(vector<int> gates) {
    string ret;
    int nc = 0, pos = -1;
    for (int i = 0; i < gates.size(); i++)
      if (gates[i] > 0) {
        pos = i;
        nc++;
      }

    // zero lines
    if (nc == 0) {
      return ret;
    }

    // one line
    if (nc == 1) {
      for (int i = 0; i < gates[pos]; i++) {
        ret += ('a' + pos);
        ret += ('A' + pos);
      }
      return ret;
    }

    // one line has one person
    for (int i = 0; i < gates.size(); i++)
      if (gates[i] == 1) {
        ret += ('a' + i);
        for (int j = 0; j < gates.size(); j++)
          if (j != i) {
            for (int k = 0; k < gates[j]; k++) {
              ret += ('a' + j);
              ret += ('A' + j);
            }
          }
        ret += ('A' + i);
        return ret;
      }

    // general case, count = 2
    int a = -1, b = -1;
    for (int i = 0; i < gates.size(); i++)
      if (gates[i] > 0) {
        if (a < 0) {
          a = i;
        } else {
          b = i;
        }
      }

    gates[a]--;
    gates[b]--;
    ret += ('a' + a);
    for (int i = 0; i < gates.size(); i++)
      if (i != a) {
        for (int j = 0; j < gates[i]; j++) {
          ret += ('a' + i);
          ret += ('A' + i);
        }
        gates[i] = 0;
      }
    ret += ('A' + a);
    ret += ('a' + b);
    for (int i = 0; i < gates.size(); i++)
      if (i != b) {
        for (int j = 0; j < gates[i]; j++) {
          ret += ('a' + i);
          ret += ('A' + i);
        }
      }
    ret += ('A' + b);
    return ret;
  }
};

// Powered by KawigiEdit 2.1.4 (beta) modified by pivanof!