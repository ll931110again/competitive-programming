#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <map>
#include <queue>
#include <set>
#include <vector>
#define maxn 20
using namespace std;

int query(set<int> idx) {
    cout << "? ";
    for (auto x : idx) {
        cout << x << ' ';
    }
    cout << endl;
    cout << flush;

    int x;
    cin >> x;
    return x;
}

void output(vector<int> idx) {
    cout << "! ";
    for (auto x : idx) {
        cout << x << ' ';
    }
    cout << endl;
    cout << flush;
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector< set<int> > buckets;
        vector<int> leaders;

        for (int i = 0; i < n; i++) {
            buckets.push_back({});
            for (int j = 0; j < n; j++) {
                buckets.back().insert(i * n + j + 1);
            }
        }
        
        for (int i = 0; i < n; i++) {
            leaders.push_back(query(buckets[i]));
        }

        vector<int> ans;
        while (ans.size() < n * n - 2 * n + 1) {
            set<int> query_across_buckets;
            for (int i = 0; i < n; i++) {
                query_across_buckets.insert(leaders[i]);
            }

            auto best_val = query(query_across_buckets);
            ans.push_back(best_val);

            int bucket_idx = -1;
            for (int i = 0; i < n; i++) {
                if (leaders[i] == best_val) {
                    bucket_idx = i;
                    break;
                }
            }

            buckets[bucket_idx].erase(best_val);
            while (buckets[bucket_idx].size() < n) {
                bool proceed = false;
                for (int i = 0; i < n; i++) if (i != bucket_idx && buckets[i].size() > 1) {
                    for (auto it : buckets[i]) {
                        if (it != leaders[i]) {
                            proceed = true;
                            buckets[bucket_idx].insert(it);
                            buckets[i].erase(it);
                            break;
                        }
                    }
                }
                if (!proceed) {
                    break;
                }
            }

            if (buckets[bucket_idx].size() < n) {
                break;
            }

            auto new_leader = query(buckets[bucket_idx]);
            leaders[bucket_idx] = new_leader;
        }

        set<int> fast, slow;
        for (int i = 0; i < n; i++) {
            fast.insert(leaders[i]);
        }
        for (int i = 0; i < n; i++) {
            for (auto x : buckets[i]) {
                if (fast.find(x) == fast.end()) {
                    slow.insert(x);
                }
            }
        }

        while (ans.size() < n * n - n) {
            auto x = query(fast);
            ans.push_back(x);
            fast.erase(x);
            x = *slow.begin();
            fast.insert(x);
            slow.erase(x);
       }

       for (int i = 0; i < n; i++) if (fast.find(leaders[i]) != fast.end()) {
         ans.push_back(leaders[i]);
       }
       
       output(ans);
    }
    return 0;
}