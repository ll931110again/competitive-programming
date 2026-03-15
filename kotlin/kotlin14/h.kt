/*
 * Solution for problem H: Sum of MEX.
 *
 * First, let's solve the problem for a single array.
 * The condition that there are at most 300 wildcard positions indicates a DP solution.
 * 
 * Say we'd want to target the MEX value of t, and in the array there are k unfilled positions
 * that is less than t.
 * 
 * Then f(x, y) be the number of ways to fill x wildcard positions, with y remaining unfilled positions
 * that is less than t.
 * 
 * Initialize: f(0, k) = 1.
 * Then for the next position:
 * 		f(x + 1, y - 1) += f(x, y) * y  # fill in one unfilled position
 * 		f(x + 1, y) += f(x, y) * (n - y)  # fill in one existing position that is not t.
 * 
 * Answer is f(K, 0), and we sum over f(K, 0) * t for each possible MEX value t (note there can be at most K + 1 such values).
 * 
 * Now note that the MEX value could be n + 1, in which case the second DP needs to be modified as
 * 		f(x + 1, y) += f(x, y) * (n + 1 - y).
 * 
 * The solution runs in O(K^3) where K is the number of wildcard positions in the array.
 * However, we now need to compute the answer for every prefix of the array.
 * 
 * The key insight is that, the above DP is independent of the MEX values. Hence, every time we add
 * a prefix, we simply update the target MEX values. The only tricky case is for the target MEX value n + 1,
 * where we have to recompute the dp array from scratch (which we'd rather be done by precomputing the
 * second DP array). This gives the time complexity of O(N * K).
 */

import kotlin.math.*;

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var MOD = 998244353;
	var n = nextInt();
	var a = nextInts();

	var K = 0;
	for (i in 0 until n) {
		if (a[i] == -1) {
			K++;
		}
	}

	var target = Array(K + 1) { LongArray(K + 1) {0L} };
	var target2 = Array(K + 1) { LongArray(K + 1) {0L} };

	for (unfilled in 0 until K + 1) {
		var dp = Array(K + 1) { LongArray(K + 1) {0L} };
		dp[0][unfilled] = 1L;

		for (i in 0 until K) {
			for (j in 0 until K + 1) if (dp[i][j] > 0) {
				if (j > 0) {
					dp[i + 1][j - 1] += dp[i][j] * j;
					dp[i + 1][j - 1] %= MOD;
				}
				dp[i + 1][j] += dp[i][j] * (n - j);
				dp[i + 1][j] %= MOD;
			}
		}

		for (i in 0 until K + 1) {
			target[i][unfilled] = dp[i][0];
		}

		var dp2 = Array(K + 1) { LongArray(K + 1) {0L} };
		dp2[0][unfilled] = 1L;

		for (i in 1 until K) {
			for (j in 0 until K + 1) {
				if (j > 0) {
					dp2[i + 1][j - 1] += dp2[i][j] * j;
					dp2[i + 1][j - 1] %= MOD;
				}
				dp2[i + 1][j] += dp2[i][j] * (n + 1 - j);
				dp2[i + 1][j] %= MOD;
			}
		}

		for (i in 0 until K + 1) {
			target2[i][unfilled] = dp2[i][0];
		}
	}

	var wildcards = 0;
	var nextUnfilled = IntArray(n + 2) {0};
	for (i in 0 until n + 2) {
		nextUnfilled[i] = i;
	}
	var numUnfilled = n + 1;

	for (i in 0 until n) {
		if (a[i] == -1) {
			wildcards++;
		} else {
			if (nextUnfilled[a[i]] == a[i]) {
				numUnfilled--;
			}
			nextUnfilled[a[i]] = nextUnfilled[a[i] + 1];
		}

		var ans = 0L;
		var mex = 0;
		for (j in 0 until wildcards + 1) {
			var reassigned = ArrayList<Int>();
			while (mex <= n && mex != nextUnfilled[mex]) {
				reassigned.add(mex);
				mex = nextUnfilled[mex];
			}
			for (x in reassigned) {
				nextUnfilled[x] = mex;
			}

			if (mex > n) {
				break;
			}

			ans += target[wildcards][j] * mex;
			ans %= MOD;
			mex++;
		}

		if (numUnfilled <= wildcards) {
			ans += target2[numUnfilled][wildcards] * (n + 1);
			ans %= MOD;
		}
		
		print(ans);
		if (i < n - 1) {
			print(" ");
		} else {
			println();
		}
	}
}