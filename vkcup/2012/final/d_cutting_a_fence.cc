#include <bits/stdc++.h>
#define maxn 1000005
using namespace std;

int n, h[maxn];
int L[maxn], R[maxn], Q;
long long d2[maxn], sum[maxn];

void calcL() {
  stack<int> s;
  for (int i = 1; i <= n; i++) {
    while (!s.empty() && h[s.top()] > h[i])
      s.pop();
    L[i] = (s.empty()) ? 1 : (1 + s.top());
    s.push(i);
  }
}

void calcR() {
  stack<int> s;
  for (int i = n; i > 0; i--) {
    while (!s.empty() && h[s.top()] >= h[i])
      s.pop();
    R[i] = (s.empty()) ? n : (s.top() - 1);
    s.push(i);
  }
}

int main() {
  cin >> n;
  for (int i = 1; i <= n; i++)
    cin >> h[i];
  calcL();
  calcR();
  for (int i = 1; i <= n; i++) {
    d2[1] += h[i];
    d2[i - L[i] + 2] -= h[i];
    d2[R[i] - i + 2] -= h[i];
    d2[R[i] - L[i] + 3] += h[i];
  }
  for (int i = 1; i <= n; i++) {
    d2[i] += d2[i - 1];
    sum[i] = sum[i - 1] + d2[i];
  }
  cin >> Q;
  while (Q--) {
    int x;
    cin >> x;
    cout << fixed << setprecision(18) << 1.0 * sum[x] / (n - x + 1) << '\n';
  }
}
