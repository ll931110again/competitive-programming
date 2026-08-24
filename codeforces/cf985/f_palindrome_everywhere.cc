#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        string c;
        cin >> n >> c;

        if (n % 2 == 1) {
            // For odd n, answer is always YES
            cout << "YES\n";
            continue;
        }

        // For even n: count R's and B's
        int cntR = 0;
        for (char ch : c) {
            if (ch == 'R') cntR++;
        }
        int cntB = n - cntR;

        // Answer is YES if at most one R or at most one B
        if (cntR <= 1 || cntB <= 1) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
    return 0;
}