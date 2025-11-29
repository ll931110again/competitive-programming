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
#define maxn 5002
#define mod 1000000007
using namespace std;
 
int f[maxn][maxn][2];
string a,b;
 
int rec(int x,int y,int z) {
  if (x < 0 || y < 0) return z;
  int& ans = f[x][y][z];
  if (ans >= 0) return ans;
  ans = z;
  ans = (ans + rec(x,y - 1,z)) % mod;
  if (z) ans = (ans + mod - 1) % mod;
  if (a[x] == b[y]) ans = (ans + rec(x - 1,y - 1,1)) % mod;
  return ans;
}
 
int main() {
  cin >> a;
  cin >> b;
  memset(f,-1,sizeof(f));
  int ret = 0;
  for (int i = 0; i < a.size(); i++)
    ret = (ret + rec(i,b.size() - 1,0)) % mod;
  printf("%d\n", (ret + mod) % mod);
}
