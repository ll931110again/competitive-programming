#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define mod 1000000009
#define maxn 15
#define maxk 115
#define max_alphabet 4
using namespace std;

int n, m;
string s[maxn];

int trie_idx[maxk][max_alphabet];
int jump_idx[maxk][max_alphabet];
unordered_map<string, int> prefix_idx;
int finish_len[maxk];

long long cur[maxk][maxn], nxt[maxk][maxn];
map<char, int> dna = {
	{'A', 0},
	{'C', 1},
	{'G', 2},
	{'T', 3},
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

	cin >> n >> m;
	for (int i = 0; i < m; i++) {
		cin >> s[i];
	}

	memset(trie_idx, -1, sizeof trie_idx);
	memset(jump_idx, -1, sizeof jump_idx);

	set<string> prefixes, all;

	int node_cnt = 1;
	prefixes.insert("");
	prefix_idx[""] = 0;
	for (int i = 0; i < m; i++) {
		all.insert(s[i]);
		string prefix = "";

		int node_idx = 0;
		for (int j = 0; j < s[i].size(); j++) {
			prefix += s[i][j];
			prefixes.insert(prefix);

			int dna_idx = dna[s[i][j]];
			if (trie_idx[node_idx][dna_idx] < 0) {
				trie_idx[node_idx][dna_idx] = node_cnt;
				prefix_idx[prefix] = node_cnt;
				node_cnt++;
			}

			node_idx = trie_idx[node_idx][dna_idx];
		}
	}

	for (auto prefix : prefixes) {
		for (auto it : dna) {
			string new_prefix = prefix + it.first, tmp = new_prefix;
			int dna_idx = it.second;

			while (prefix_idx.find(tmp) == prefix_idx.end()) {
				tmp = tmp.substr(1, tmp.size() - 1);
			}

			jump_idx[prefix_idx[prefix]][dna_idx] = prefix_idx[tmp];
		}
	}

	for (auto prefix : prefixes) {
		int idx = prefix_idx[prefix];
		for (auto s : all) {
			if (prefix.ends_with(s)) {
				finish_len[idx] = max(finish_len[idx], (int) s.size());
			}
		}
	}

	int max_length = 10;
	cur[0][0] = 1;
	for (int i = 0; i < n; i++) {
		memset(nxt, 0, sizeof nxt);
		for (int state = 0; state < node_cnt; state++) {
			for (int len = 0; len < max_length; len++) if (cur[state][len]) {
				for (int c = 0; c < max_alphabet; c++) {
					int nstate = jump_idx[state][c];
					if (nstate < 0) {
						continue;
					}

					int nlen = len + 1;
					if (finish_len[nstate] >= nlen) {
						nlen = 0;
					}
					if (nlen > max_length) {
						continue;
					}
					nxt[nstate][nlen] += cur[state][len];
					nxt[nstate][nlen] %= mod;
				}
			}
		}

		for (int state = 0; state < node_cnt; state++) {
			for (int len = 0; len < max_length; len++) {
				cur[state][len] = nxt[state][len];
				nxt[state][len] = 0;
			}
		}
	}

	long long ans = 0;
	for (int state = 0; state < node_cnt; state++) {
		ans += cur[state][0];
		ans %= mod;
	}

	cout << ans << endl;
    
    return 0;
}