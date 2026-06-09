#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 1000005;
int n, h[k_max_n];
int L[k_max_n], R[k_max_n], Q;
long long d2[k_max_n], sum[k_max_n];

void calc_l() {
  stack<int> s;
  for (int i = 1; i <= n; i++) {
    while (!s.empty() && h[s.top()] > h[i])
      s.pop();
    L[i] = (s.empty()) ? 1 : (1 + s.top());
    s.push(i);
  }
}

void calc_r() {
  stack<int> s;
  for (int i = n; i > 0; i--) {
    while (!s.empty() && h[s.top()] >= h[i])
      s.pop();
    R[i] = (s.empty()) ? n : (s.top() - 1);
    s.push(i);
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 1; i <= n; i++)
    cin >> h[i];
  calc_l();
  calc_r();
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
