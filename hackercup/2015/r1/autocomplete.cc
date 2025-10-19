#include <vector>
#include <cstdio>
#include <iostream>
#define MAXK 1000005
using namespace std;
 
vector<int> trie[MAXK];
int T, N, K;
 
void solveCase(int it)
{
	K = 2;
	trie[1] = vector<int>(26);
	cin >> N;
	int result = 0;
 
	for (int i = 0; i < N; i++)
	{
		string input;
		bool isTyped = false;
		cin >> input;
 
		int root = 1;
		for (int j = 0; j < input.size(); j++)
		{
			int trieNode = trie[root][input[j] - 'a'];
			if (trieNode <= 0 && !isTyped)
			{
				result += j + 1;
				isTyped = true;
			}
			if (trieNode <= 0)
			{
				trie[K] = vector<int>(26);
				trie[root][input[j] - 'a'] = K++;
			}
			root = trie[root][input[j] - 'a'];
		}
 
		if (!isTyped)
			result += input.size();
	}
	printf("Case #%d: %d\n", it, result);
}
 
int main()
{
	cin >> T;
	for (int it = 1; it <= T; it++)
		solveCase(it);
}
