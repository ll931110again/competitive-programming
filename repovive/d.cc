
#include <bits/stdc++.h>
using namespace std;

int T, n;
string s;
vector<int> openings, closings;

bool ok(int mid) {
    string tmp = s;
    for (int i = 0; i < mid; i++) {
        tmp[openings[i]] = ')';
        tmp[closings[i]] = '(';
    }

    int delta = 0;
    for (auto c : tmp) {
        if (c == '(') {
            delta++;
        } else {
            delta--;
        }
        if (delta < 0) {
            return false;
        }
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    while (T--) {
        cin >> n;
        cin >> s;

        openings.clear();
        closings.clear();
        for (int i = 0; i < s.size(); i++) if (s[i] == '(') {
            openings.push_back(i);
        } else {
            closings.push_back(i);
        }
        reverse(openings.begin(), openings.end());

        int low = 0, high = n, ans = 0;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (ok(mid)) {
                ans = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        cout << 2 * ans << endl;
    }
	return 0;
}
