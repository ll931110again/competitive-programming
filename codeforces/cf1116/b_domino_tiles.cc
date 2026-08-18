#include <iostream>
#include <string>
using namespace std;

const int MOD = 998244353;

int countValidPatterns(const string& seq) {
    int n = seq.size();
    if (n == 0) return 1;

    // Try pattern starting with '0': 0, 1, 0, 1, ...
    bool valid0 = true;
    for (int i = 0; i < n; i++) {
        char expected = (i % 2 == 0) ? '0' : '1';
        if (seq[i] != '?' && seq[i] != expected) {
            valid0 = false;
            break;
        }
    }

    // Try pattern starting with '1': 1, 0, 1, 0, ...
    bool valid1 = true;
    for (int i = 0; i < n; i++) {
        char expected = (i % 2 == 0) ? '1' : '0';
        if (seq[i] != '?' && seq[i] != expected) {
            valid1 = false;
            break;
        }
    }

    return (valid0 ? 1 : 0) + (valid1 ? 1 : 0);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        string s;
        cin >> s;

        string evenSeq, oddSeq;
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) evenSeq += s[i];
            else oddSeq += s[i];
        }

        long long evenWays = countValidPatterns(evenSeq);
        long long oddWays = countValidPatterns(oddSeq);

        long long ans = (evenWays * oddWays) % MOD;
        cout << ans << "\n";
    }

    return 0;
}