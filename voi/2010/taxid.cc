#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<int> c;
long long pw[40][60];
long long n,q;
int m,p;
char a[36];

long long calc(long long x,int base)
{
    vector<long long> digit;
    while (1)
    {
        digit.push_back(x % 36); x /= 36;
        if (!x) break;
    }
    reverse(digit.begin(),digit.end());
    int sz = digit.size();
    for (int i = 0; i < sz; i++) if (digit[i] >= base)
        for (int j = i; j < sz; j++) digit[j] = base - 1;

    long long ans = 0;
    for (int i = 0; i < sz; i++) ans += 1LL * digit[i] * pw[base][sz - i - 1];
    ans++;  
    return ans;
}

long long ascending(int x)
{
    long long low = 1,high = n,ans = -1;
    while (low <= high)
    {
        long long med = (low + high)/2;
        long long tmp = calc(med,c[x]) - calc(med,c[x - 1]);
        if (tmp >= q)
        {
            ans = med;  high = med - 1;
        }
        else low = med + 1;
    }
    return ans;
}

long long descending(int x)
{
    long long s1 = calc(n,c[x]),s2 = calc(n,c[x - 1]);
    long long low = 1,high = n,ans = -1;
    while (low <= high)
    {
        long long med = (low + high)/2;
        long long t1 = calc(med - 1,c[x]),t2 = calc(med - 1,c[x - 1]);
        long long tmp = (s1 - t1) - (s2 - t2);
        if (tmp >= q)
        {
            ans = med;  low = med + 1;
        }
        else high = med - 1;
    }
    return ans;
}

int main()
{
//    freopen("taxid.in","r",stdin);
//    freopen("taxid.ou","w",stdout);
    cin >> n >> m >> p >> q;
    c.push_back(1);
    for (int i = 0; i < (m - 1)/2; i++) 
    {
        int x;
        scanf("%d", &x);  c.push_back(x);
    }
    c.push_back(36);

    for (int i = 2; i <= 36; i++)
    {
        pw[i][0] = 1;
        for (int j = 1; pw[i][j - 1] <= n/i; j++) pw[i][j] = pw[i][j - 1] * i;
    }

    for (int i = 0; i < 10; i++) a[i] = i + '0';
    for (int i = 10; i < 36; i++) a[i] = i - 10 + 'a';

    long long ret = (p & 1) ? ascending( (p + 1)/2 ) : descending( (p + 1)/2 );
    string fin;
    while (ret)
    {
        fin += a[ret % 36];  ret /= 36;
    }    
    for (int i = fin.size() - 1; i >= 0; i--) printf("%c", fin[i]);
    printf("\n");
}
