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
#define maxn 100000
using namespace std;
 
bool prime[maxn + 3];
int numPrime[maxn + 3][6];
int board[8][8],T,digit;
long long ans;
 
inline void attempt(int x,int y,long long way) {
  if (x >= digit) {
    ans += way;
    return;
  }
  if (y >= digit) {
    int val = 0;
    for (int i = 0; i < digit; i++) val = val * 10 + board[x][i];
    attempt(x + 1,0,way * numPrime[val][digit - 1 - x]);
    return;
  }
  if (x >= y) {
    board[x][y] = board[y][x];
    attempt(x,y + 1,way);
  }
  else for (int k = 0; k < 10; k++) {
    board[x][y] = k;
    attempt(x,y + 1,way);
  }
}
 
long long solve(int n) {
  memset(board,0,sizeof(board));
  digit = 0;
  while (n) {
    board[0][digit] = n % 10;
    n /= 10;
    digit++;
  }
  for (int i = 0,j = digit - 1; i < j; i++,j--) swap(board[0][i],board[0][j]); 
  ans = 0;
  attempt(1,0,1);
  return ans;
}
 
int main() {
  prime[0] = prime[1] = true;
  for (int i = 2; i <= maxn; i++) if (!prime[i])
    for (int j = i + i; j <= maxn; j += i) prime[j] = true;
  for (int i = 0; i < maxn; i++) {
    int prod = 1;
    for (int j = 0; j < 6; j++) {      
      int x = i;
      for (int k = 0; k < 10; k++) {      
        if (x < maxn && !prime[x]) numPrime[i][j]++;
	x += prod;
      }
      prod *= 10;
    }    
  }    
  scanf("%d", &T);
  while (T--) {
    int n;
    scanf("%d", &n);
    cout << solve(n) << endl;
  }
}
