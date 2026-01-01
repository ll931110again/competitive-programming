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
using namespace std;

string form = "HACKERCUP";
int T;
int a[256],b[256];
char s[1002];

int main() {
    freopen("soup.in.1.txt","r",stdin);
    freopen("soup.out.1","w",stdout);
    for (int i = 0; i < form.size(); i++) b[form[i]]++;
    scanf("%d\n", &T);
    for (int it = 1; it <= T; it++) {
        gets(s);
        memset(a,0,sizeof(a));
        for (int i = 0; i < strlen(s); i++) a[s[i]]++;
        int ret = 1000;
        for (int i = 'A'; i <= 'Z'; i++) if (b[i]) ret = min(ret,a[i]/b[i]);
        printf("Case #%d: %d\n", it, ret);
    }
}
