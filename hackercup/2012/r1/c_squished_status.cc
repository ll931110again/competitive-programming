#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

int T, m;
char s[1005];
string encode;
i64 f[1005];
i64 mod = 4207849484LL;

int convert(string s) {
  stringstream w;
  w << s;
  int x;
  w >> x;
  return x;
}

i64 rec(int pos) {
  if (pos < 0)
    return 1;
  if (f[pos] >= 0)
    return f[pos];
  f[pos] = 0;

  string store = "";
  for (int i = pos; i > pos - 3 && i >= 0; i--) {
    store = s[i] + store;
    if (s[i] == '0')
      continue;
    if (convert(store) <= m)
      f[pos] = (f[pos] + rec(i - 1)) % mod;
  }
  return f[pos];
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("encode.in.1", "r", stdin);
  freopen("encode.out.1", "w", stdout);
  cin >> T;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  for (int it = 1; it <= T; it++) {
    cin >> m;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(s, 1005);
    encode = "";
    for (int i = 0; i < strlen(s); i++)
      if ('0' <= s[i] && s[i] <= '9')
        encode += s[i];
    memset(f, -1, sizeof(f));
    cerr << m << '\n';
    cout << "Case #" << it << ": " << rec(encode.size() - 1) << '\n';
  }
}
