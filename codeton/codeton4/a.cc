#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#define maxn 105
using namespace std;
 
int T, n;
int a[maxn];
 
bool solve() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }
    for (int i = 1; i <= n; i++) {
        if (a[i] <= i) {
            return true;
        }
    }
    return false;
}
 
int main() {
    scanf("%d", &T);
    while (T--) {
        if (solve()) {
            cout << "YES";
        } else {
            cout << "NO";
        }
        cout << endl;
    }
    return 0;
}
