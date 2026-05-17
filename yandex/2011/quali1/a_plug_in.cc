// Codeforces 81 (Yandex.Algorithm Open 2011: Qualification 1) — A. Plug-in
// Submission: https://codeforces.com/contest/81/submission/474429

#include <bits/stdc++.h>
using namespace std;

int main()
{
    string s;  cin >> s;
    vector<int> v;
    for (int i = 0; i < s.size(); i++)
      if (!v.empty() && v[v.size() - 1] == s[i]) v.pop_back(); else v.push_back(s[i]);
    for (int i = 0; i < v.size(); i++) printf("%c", v[i]);  printf("\n");
}
