#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#define maxn 100005
using namespace std;

int T;
int par[maxn];

int root(int x) {
    if (x != par[x]) {
        par[x] = root(par[x]);
    }
    return par[x];
}

void link(int u, int v) {
    u = root(u);
    v = root(v);
    if (u != v) {
        par[v] = u;
    }
}

int solve(string s) {
    int n = s.size();
    for (int i = 0; i < n; i++) {
        par[i] = i;
    }

    for (int i = 1; i + 1 < n; i++) if (s[i] == 'I') {
        if (s[i - 1] != 'I' && s[i + 1] != 'I') {
            link(i - 1, i + 1);
        }
    }

    map<int, vector<int>> pos;
    for (int i = 0; i < n; i++) {
        pos[root(i)].push_back(i);
    }

    int ans = 0;
    for (int i = 0; i < n; i++) {
        bool is_valid = true;
        int m_count = 0, t_count = 0;
        for (auto x : pos[i]) {
            if (s[x] == 'I') {
                is_valid = false;
            }
            if (s[x] == 'M') {
                m_count++;
            } else {
                t_count++;
            }
        }
        if (!is_valid) {
            continue;
        }
        ans += min(m_count, t_count / 2);
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    while (T--) {
        string s;
        cin >> s;
        cout << solve(s) << endl;
    }
	return 0;
}
