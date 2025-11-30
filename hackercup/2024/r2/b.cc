#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <queue>
#define maxn 7
using namespace std;

int T;
string input[maxn];
int M = 6, N = 7;
int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

bool is_winning(vector<int> v, char ch) {
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			for (int d = 0; d < 8; d++) {
				int _i = i, _j = j;
				bool ok = true;
				for (int k = 0; k < 4; k++) {
					if (_i < 0 || _i >= M || _j < 0 || _j >= N || _i < v[_j] || input[_i][_j] != ch) {
						ok = false;
						break;
					}
					_i += dx[d];
					_j += dy[d];
				}
				if (ok) {
					return true;
				}
			}
		}
	}
	return false;
}

char solve() {
	for (int i = 0; i < M; i++) {
		cin >> input[i];
	}

	set< vector<int> > start_reachable;
	queue< vector<int> > q;

	set< vector<int> > end_reachable;

	auto initial = {0, 0, 0, 0, 0, 0, 0};
	end_reachable.insert(initial);
	q.push(initial);

	while (!q.empty()) {
		auto u = q.front();
		q.pop();

		int turn = 0;
		for (auto x : u) {
			turn = (turn + x) % 2;
		}

		char next_turn = (turn == 0) ? 'F' : 'C';
		for (int j = 0; j < N; j++) {
			if (u[j] == M) {
				continue;
			}
			if (input[u[j]][j] != next_turn) {
				continue;
			}

			auto v = u;
			v[j]++;

			if (end_reachable.find(v) == end_reachable.end()) {
				end_reachable.insert(v);
				q.push(v);
			}
		}
	}

	initial = {M, M, M, M, M, M, M};
	start_reachable.insert(initial);
	q.push(initial);

	bool is_c_win = false;
	bool is_f_win = false;

	while (!q.empty()) {
		auto u = q.front();
		q.pop();

		int turn = 0;
		for (auto x : u) {
			turn = (turn + x) % 2;
		}

		char next_turn = (turn == 0) ? 'C' : 'F';
		for (int j = 0; j < N; j++) {
			if (u[j] == 0) {
				continue;
			}
			if (input[u[j] - 1][j] != next_turn) {
				continue;
			}

			auto v = u;
			v[j]--;

			if (end_reachable.find(v) == end_reachable.end()) {
				continue;
			}

			if (start_reachable.find(v) == start_reachable.end()) {
				bool c_win = is_winning(v, 'C');
				bool f_win = is_winning(v, 'F');

				if (c_win && !f_win) {
					is_c_win = true;
				}
				if (!c_win && f_win) {
					is_f_win = true;
				}

				if (c_win || f_win) {
					continue;
				}

				start_reachable.insert(v);
				q.push(v);
			}
		}
	}

	if (!is_c_win && !is_f_win) {
		return '0';
	}
	if (is_c_win && is_f_win) {
		return '?';
	}
	if (is_c_win) {
		return 'C';
	}
	return 'F';
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	
	cin >> T;
	for (int it = 1; it <= T; it++) {
		cout << "Case #" << it << ": " << solve() << endl;
	}
	return 0;
}