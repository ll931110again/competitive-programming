#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 1000005
using namespace std;

int T;
int n, m;
int scores[maxn], stockpile[maxn], a[maxn], cnt_a;

vector<int> score_bucket;
map<int,int> score_cnt;
int prefix[maxn];

bool get(int mid) {
    if (m < mid) {
        return false;
    }

    long long balance = 0;
    for (int i = mid; i < m; i++) {
        balance += stockpile[i];
    }

    int ss = 0;
    for (int i = 0; i < mid; i++) {
        ss += score_bucket[i];
        balance += stockpile[mid - i - 1];
        balance -= ss;
        if (balance < 0) {
            return false;
        }
    }

    return true;
}

int solve() {
    int low = 1, high = score_bucket.size(), ans = 0;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (get(mid)) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    int output = 0;
    for (int i = 0; i < ans; i++) {
        output += score_bucket[i];
    }
    return output;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
    for (int it = 1; it <= T; it++) {
        cerr << it << endl;
        cin >> n >> m;
        for (int i = 0; i < n; i++) {
            cin >> scores[i];
        }
        sort(scores, scores + n);
        reverse(scores, scores + n);

        score_bucket.clear();
        for (int i = 0; i < n; ) {
            int j = i + 1;
            while (j < n && scores[j] == scores[i]) {
                j++;
            }
            score_bucket.push_back(j - i);
            i = j;
        }

        for (int i = 0; i < m; i++) {
            cin >> stockpile[i];
        }
        sort(stockpile, stockpile + m);
        reverse(stockpile, stockpile + m);

        cout << "Case #" << it << ": " << solve() << endl;
    }

    return 0;
}