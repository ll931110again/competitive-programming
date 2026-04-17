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
#include <unordered_set>
#include <set>
using namespace std;

set<int> organizers;
int T;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    const int organizers_count = 10;
    const int participant_count = 6;
    const int lucky_threshold = 3;

    for (int i = 0; i < organizers_count; i++) {
        int x;
        cin >> x;
        organizers.insert(x);
    }

    cin >> T;
    while (T--) {
        int lucky_count = 0;
        for (int i = 0; i < participant_count; i++) {
            int x;
            cin >> x;
            if (organizers.find(x) != organizers.end()) {
                lucky_count++;
            }
        }
        if (lucky_count >= lucky_threshold) {
            cout << "Lucky" << endl;
        } else {
            cout << "Unlucky" << endl;
        }
    }
 
    return 0;
}
