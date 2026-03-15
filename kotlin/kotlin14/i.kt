/*
 * Solution for problem I: Strange Process.
 */

import java.util.*;
import kotlin.math.*;

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

var MOD = 998244353;

fun getPower(x: Long, e: Int): Long {
	if (e == 0) {
		return 1L
	}
	var q = getPower(x, e/2);
	q = q * q % MOD;
	if (e % 2 > 0) {
		q = (q * x) % MOD;
	}
	return q;
}

fun main() {
	var (n, m) = nextInts();
	var c = nextInts();

	var MAXK = 55;
	var primes = BooleanArray(MAXK) {true};
	primes[0] = false;
	primes[1] = false;
	for (i in 2 until MAXK) if (primes[i]) {
		var j = i + i;
		while (j < MAXK) {
			primes[j] = false;
			j += i;
		}
	}

	var dists = Array(MAXK) { IntArray(MAXK) { -1} };
	for (u in 1 until MAXK) {
		var q: Queue<Int> = LinkedList();
		q.add(u);
		dists[u][u] = 0;

		while (!q.isEmpty()) {
			var x = q.remove();
			for (y in 1 until MAXK) if (primes[y]) {
				var z = x * y;
				if (z < MAXK && (dists[u][z] < 0 || dists[u][z] > 1 + dists[u][x])) {
					dists[u][z] = 1 + dists[u][x];
					q.add(z);
				}
				if (x % y == 0) {
					z = x / y;
					if (dists[u][z] < 0 || dists[u][z] > 1 + dists[u][x]) {
						dists[u][z] = 1 + dists[u][x];
						q.add(z);
					}
				}
			}
		}
	}

	var K = 8;
	var numStates = 1 shl K;
	// dp[i][j] denotes the number of arrays up to index i and the
	// current mask of pinned elements are j.
	var dp = Array(m + 1) { LongArray(numStates) {0L} };
	
	dp[0][0] = 1L;
	for (i in 0 until m) {
		for (mask in 0 until numStates) {
			// First case: do not update b_i altogether.
			var _mask = (mask shl 1) and ((1 shl (K - 1)) - 1);
			dp[i + 1][_mask] += dp[i][mask];
			dp[i + 1][_mask] %= MOD;

			// Second case: update b_i using wildcard elements (effectively 1).
			_mask = _mask or 1;
			if (dists[1][c[i]] <= i + 1) {
				var contextWindow = min(i, K);
				var unfilledSlots = n - contextWindow;
				for (k in 0 until K) if (i > k) {
					if ((mask and (1 shl k)) == 0) {
						unfilledSlots++;
					}
				}

				dp[i + 1][_mask] += dp[i][mask] * unfilledSlots;
				dp[i + 1][_mask] %= MOD;
			}			

			// Third case: update b_i using pinned elements, then we need
			// to check the distance from pinned element to the current one.
			for (k in 0 until K) if (i > k) {
				if ((mask and (1 shl k)) > 0) {
					if (dists[c[i - k - 1]][c[i]] <= k + 1) {
						var newMask = mask xor (1 shl k);
						newMask = (newMask shl 1) and ((1 shl (K - 1)) - 1);
						newMask = newMask or 1;
						dp[i + 1][newMask] += dp[i][mask];
						dp[i + 1][newMask] %= MOD;
					}
				}
			}
		}
	}

	var ans = 0L;
	for (mask in 0 until numStates) {
		ans = (ans + dp[m][mask]) % MOD;
	}
	println(ans);
}