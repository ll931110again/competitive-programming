#include <bits/stdc++.h>
using namespace std;

namespace {

string form = "HACKERCUP";
int T;
int a[256], b[256];
char s[1002];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("soup.in.1.txt", "r", stdin);
  freopen("soup.out.1", "w", stdout);
  for (int i = 0; i < form.size(); i++)
    b[form[i]]++;
  cin >> T;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  for (int it = 1; it <= T; it++) {
    cin.getline(s, 1002);
    memset(a, 0, sizeof(a));
    for (int i = 0; i < strlen(s); i++)
      a[s[i]]++;
    int ret = 1000;
    for (int i = 'A'; i <= 'Z'; i++)
      if (b[i])
        ret = min(ret, a[i] / b[i]);
    cout << "Case #" << it << ": " << ret << '\n';
  }
}
