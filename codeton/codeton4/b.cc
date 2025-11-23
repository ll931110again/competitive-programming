#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;
 
int T, n;
 
vector<int> solve(int n) {
    if (n % 2 == 0) {
        return {-1};
    }
 
    vector<int> binaryRep;
    while (n > 0) {
        binaryRep.push_back(n % 2);
        n /= 2;
    }
 
    vector<int> ans = {2};
    for (int i = binaryRep.size() - 2; i > 0; i--) {
        if (binaryRep[i] == 0) {
            ans.push_back(1);
        } else {
            ans.push_back(2);
        }
    }
    return ans;
}
 
int main() {
    scanf("%d", &T);
    while (T--) {
        cin >> n;
        auto ans = solve(n);
        if (ans[0] == -1) {
            cout << -1 << endl;
        } else {
            cout << ans.size() << ' ';
            for (auto x : ans) {
                cout << x << ' ';
            }
            cout << endl;
        }
    }
    return 0;
}
