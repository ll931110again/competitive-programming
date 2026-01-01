#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
#define maxn 10005
#define mod 1000003
using namespace std;

int T,n,last;
string s;
int arr[maxn],temp[maxn],opt[maxn];

void mergeSort(int low,int high) {
    if (high - low <= 1) return;
    int mid = (low + high)/2;
    mergeSort(low,mid);
    mergeSort(mid,high);

    int la = low,lb = mid,pos = low;
    while (mid - la > 0 && high - lb > 0) {
        if (s[last] == '1') {
            temp[pos] = arr[la];
            la++;
        }
        else {
            temp[pos] = arr[lb];
            lb++;
        }
        pos++;
        last++;
    }
    for (int i = la; i < mid; i++) {
        temp[pos] = arr[i];  pos++;
    }
    for (int i = lb; i < high; i++) {
        temp[pos] = arr[i];  pos++;
    }
    for (int i = low; i < high; i++) arr[i] = temp[i];
}

int main() {
    freopen("seq.in.1","r",stdin);
    freopen("seq.out.1","w",stdout);
    scanf("%d", &T);
    for (int it = 1; it <= T; it++) {
        scanf("%d", &n);
        cin >> s;
        for (int i = 0; i < n; i++) arr[i] = i;
        last = 0;
        mergeSort(0,n);
        for (int i = 0; i < n; i++) opt[arr[i]] = i;
        long long ret = 1;
        for (int i = 0; i < n; i++) ret = (ret * 31 + opt[i] + 1) % mod;
        printf("Case #%d: %lld\n", it, ret);
    }
}
