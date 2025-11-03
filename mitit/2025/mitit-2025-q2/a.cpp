#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
#include <queue>
#define maxn 500005
using namespace std;

int T, n;
int hold[maxn], cap[maxn];

bool solve() {
    queue<int> fulls;
    priority_queue< pair<int,int> > partials;

    for (int i = 0; i < n; i++) {
        if (hold[i] == cap[i]) {
            fulls.push(i);
        } else {
            partials.push({hold[i] - cap[i], i});
        }
    }

    while (!fulls.empty()) {
        int u = fulls.front();
        fulls.pop();

        int rem = cap[u];
        while (rem > 0 && !partials.empty()) {
            int v = partials.top().second;
            partials.pop();

            if (hold[v] != cap[v]) {
                int added = min(rem, cap[v] - hold[v]);
                rem -= added;
                hold[v] += added;

                if (hold[v] < cap[v]) {
                    partials.push({hold[v] - cap[v], v});
                } else {
                    fulls.push(v);
                }
            }
        }
    }

    for (int i = 0; i < n; i++) if (hold[i] < cap[i]) {
        return false;
    }

    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> hold[i];
        }
        for (int i = 0; i < n; i++) {
            cin >> cap[i];
        }
        cout << (solve() ? "YES" : "NO") << endl;
    }

	return 0;
}
