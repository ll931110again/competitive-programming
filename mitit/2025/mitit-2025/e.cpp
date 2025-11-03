#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
#define maxn 200005
using namespace std;

int T;
string s;

bool is_winner(string s) {
    int n0 = 0, n1 = 0;
    int n = s.size();
    for (auto c : s) {
        if (c == '0') {
            n0++;
        } else {
            n1++;
        }
    }

    int p0 = n0 - 1, p1 = n0;
    while (p0 >= 0 && p1 < n) {
        if (p0 == '0' && p1 == '1') {
            p0--;
            p1++;
        } else {
            break;
        }
    }

    string ts = "";
    for (int i = 0; i < n; i++) {
        if (i <= p0 || i >= p1) {
            ts += s[i];
        }
    }

    n = ts.size();
    p0 = 0;
    p1 = n - 1;
    while (p0 < n && ts[p0] == '0') {
        p0++;
    }
    while (p1 >= 0 && ts[p1] == '1') {
        p1--;
    }

    string ps = "";
    for (int i = 0; i < n; i++) if (p0 <= i && i <= p1) {
        ps += ts[i];
    }

    if (ps.empty() || ps == "10") {
        return false;
    }

    return true;
}

bool is_sorted(string s) {
    for (int i = 1; i < s.size(); i++) if (s[i] < s[i - 1]) {
        return false;
    }
    return true;
}

vector<int> make_move(string s) {
    int n = s.size();
    int n0 = 0, n1 = 0;
    for (auto c : s) {
        if (c == '0') {
            n0++;
        } else {
            n1++;
        }
    }

    int p0 = n0 - 1, p1 = n0;
    while (p0 >= 0 && p1 < n) {
        if (p0 == '0' && p1 == '1') {
            p0--;
            p1++;
        } else {
            break;
        }
    }

    vector<int> pos;
    for (int i = 0; i < n; i++) {
        if (i < p0 || i > p1) {
            pos.push_back(i + 1);
        } else {
            if (i == p0 && s[p0] != '1') {
                pos.push_back(i + 1);
            } else if (i == p1 && s[p1] != '0') {
                pos.push_back(i + 1);
            }
        }
    }

    return pos;
}

void play(string s) {
    bool to_play = is_winner(s);
    if (is_winner(s)) {
        cout << "First" << endl;
    } else {
        cout << "Second" << endl;
    }
    cout << flush;

    if (!to_play) {
        cin >> s;
    }

    while (!is_sorted(s)) {
        auto moves = make_move(s);
        cout << moves.size();
        for (auto move : moves) {
            cout << ' ' << move;
        }
        cout << endl;
        cout << flush;

        cin >> s;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int T;
    cin >> T;
    while (T--) {
        cin >> s;
        play(s);
    }
	return 0;
}
