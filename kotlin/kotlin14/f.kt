/*
 * Solution for problem F: Self-Produced Sequences.
 *
 * A sequence with all zeroes is clearly a valid answer. There are 2^x such subsequences
 * where x is the number of zeroes.
 * 
 * Now consider the case where there are nonzero elements in the array.
 * Say the sequence is 0 0 x y z ... 0 0 (appending 0 to the beginning or the end
 * doesn't change the answer).
 * 
 * Now we see that, we need x = y + z + ... e.g. the sum of all elements from the right.
 * Then, for y, we have two choices:
 * 	either y = x, and the sequence becomes 0 .. 0 x x 0 .. 0
 *  or y = z + .. (this requires y < x, since the condition x = y + z + .. implies x >= y).
 * 
 * Then for z and beyond, it has to equal the sum of all elements to the right.
 * However, consider the last nonzero element, it must equal to the sum of all elements to its right
 * (which is zero, hence invalid), or the sum of all elements on its left (which is impossible
 * the sequence is strictly decreasing). Hence this case is impossible.
 * 
 * Hence the only valid nonzero case is 0 .. 0 x x 0 .. 0
 * and we'd like to count how many zeroes on the left and on the right for each nonzero position.
 */

import kotlin.math.*;

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }
 
fun main() {
	var MOD = 998244353;
	var MAXN = 200005;

	var p2 = ArrayList<Int>();
	p2.add(1);
	for (i in 1 until MAXN) {
		p2.add(p2[i - 1] * 2 % MOD);
	}

	var T = nextInt();
	for (it in 0 until T) {
		var n = nextInt();
		var a = nextInts();

		var lefts = IntArray(n) {0};
		var rights = IntArray(n) {0};

		for (i in 0 until n) {
			if (i > 0) {
				lefts[i] += lefts[i - 1];
			}
			if (a[i] == 0) {
				lefts[i]++;
			}
		}

		for (i in n - 1 downTo 0) {
			if (i < n - 1) {
				rights[i] = rights[i + 1];
			}
			if (a[i] == 0) {
				rights[i]++;
			}
		}

		var indices = mutableMapOf<Int, ArrayList<Int>>();
		for (i in 0 until n) {
			if (a[i] == 0) {
				continue;
			}
			if (!indices.containsKey(a[i])) {
				indices[a[i]] = ArrayList<Int>();
			}
			indices[a[i]]?.add(i);
		}

		var ans = p2[lefts[n - 1]];  // Number of subsequences that are all zeroes.
		for ((value, index) in indices) {
			var pright = 0;
			for (x in index) {
				pright += p2[rights[x]];
				pright %= MOD;
			}
			for (x in index) {
				pright -= p2[rights[x]];
				pright = (pright + MOD) % MOD;
				var tmp = 1L * p2[lefts[x]] * pright % MOD;
				ans = (ans + tmp.toInt()) % MOD;
			}
		}
		println(ans);
	}
}