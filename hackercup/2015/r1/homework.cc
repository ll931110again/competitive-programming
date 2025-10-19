#include <iostream>
#include <cstring>
#define MAXN 10000000
using namespace std;
 
bool isPrime[MAXN + 2];
int primacity[MAXN + 2];
int remains[MAXN + 2];
 
int main()
{
	memset(isPrime, true, sizeof(isPrime));
	for (int i = 2; i <= MAXN; i++)
		remains[i] = i;
 
	for (int i = 2; i <= MAXN; i++) if (isPrime[i])
		for (int j = i; j <= MAXN; j += i)
		{
			isPrime[j] = false;
			primacity[j]++;
			while (remains[j] % i == 0)
			{
				remains[j] /= i;
			}
		}
 
	int T;
	cin >> T;
	for (int it = 1; it <= T; it++)
	{
		int A, B, K;
		cin >> A >> B >> K;
		int result = 0;
		for (int i = A; i <= B; i++)
			if (primacity[i] == K)
				result++;
		printf("Case #%d: %d\n", it, result);
	}
}
