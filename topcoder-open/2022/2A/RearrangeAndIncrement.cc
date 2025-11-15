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
 
vector<int> toDigits(int x) {
  vector<int> d;
  while (x > 0) {
    d.push_back(x % 10);
    x /= 10;
  }
  return d;
}
 
int fromDigits(vector<int> d) {
  int x = 0;
  for (int i = d.size() - 1; i >= 0; i--) {
    x = x * 10 + d[i];
  }
  return x;
}
 
vector<int> toOne(int x) {
  vector<int> steps;
  steps.push_back(x);
 
  if (x == 1) {
    return steps;
  }
  
  while (x > 1) {
    // add something to make x ends at 0
    while (x % 10) x++;
    steps.push_back(x);
    
    // rearrange the digits to have zeroes in the front
    vector<int> d = toDigits(x);
    sort(d.rbegin(), d.rend());
    x = fromDigits(d);
    steps.push_back(x);
  }
  
  return steps;
}
 
vector<int> toTarget(int y) {
  if (y == 1) {
    return vector<int>();
  }
  if (y < 10) {
    return {y};
  }
  
  vector<int> dy = toDigits(y);
  vector<int> dx = {1};
  
  vector<int> steps;
  // make 100..00
  while (dx.size() < dy.size()) {
    if (dx[0] == 0) {
      dx[0] = 9;
      steps.push_back(fromDigits(dx));
      for (int i = dx.size() - 1; i >= 0; i--) if (dx[i] == 0) {
        swap(dx[i], dx[0]);
        steps.push_back(fromDigits(dx));
        break;
      }
    } else {
      swap(dx[dx.size() - 1], dx[0]);
      int x = fromDigits(dx);
      steps.push_back(x);
      while (x % 10) x++;
      steps.push_back(x);
      dx = toDigits(x);
    }
  }
    
  // make x00..000
  if (dx[dx.size() - 1] != dy[dy.size() - 1]) {
    while (dx[0] == 0) {
      dx[0] = 9;
      steps.push_back(fromDigits(dx));
      for (int i = dx.size() - 1; i >= 0; i--) if (dx[i] == 0) {
        swap(dx[i], dx[0]);
        steps.push_back(fromDigits(dx));
        break;
      }
    }
    swap(dx[0], dx[dx.size() - 1]);
    steps.push_back(fromDigits(dx));
    dx[0] = dy[dy.size() - 1] - 1;
    steps.push_back(fromDigits(dx));
    swap(dx[0], dx[dx.size() - 1]);
    steps.push_back(fromDigits(dx));
    
    int x = fromDigits(dx);
    x++;
    steps.push_back(x);
    dx = toDigits(x);
  }
  
  // make dx and dy equal
  for (int i = dx.size() - 1; i >= 0; i--) if (dx[i] != dy[i]) {
    dx[0] = dy[i];
    steps.push_back(fromDigits(dx));
    swap(dx[0], dx[i]);
    steps.push_back(fromDigits(dx));
  }
  
  return steps;
}
 
 
class RearrangeAndIncrement {
public:
  vector <int> change(int X, int Y) {
    vector<int> a = toOne(X), b = toTarget(Y);
    for (int i = 0; i < b.size(); i++) {
      a.push_back(b[i]);
    }
    return a;
  }
};
 
 
 
//Powered by KawigiEdit 2.1.4 (beta) modified by pivanof!
