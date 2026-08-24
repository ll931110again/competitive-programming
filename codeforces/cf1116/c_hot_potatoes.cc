#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int &x : a) cin >> x;
        int mx = *max_element(a.begin(), a.end());
        int cnt = count(a.begin(), a.end(), mx);
        cout << cnt << '\n';
    }
    return 0;
}