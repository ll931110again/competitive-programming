// Codeforces 1958 (Kotlin Heroes: Episode 10) — F. Narrow Paths
// Submission: https://codeforces.com/contest/1958/submission/260839080

import java.util.*

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun get_power(
	x: Long,
	p: Long,
	mod: Long,
): Long {
	if (p == 0L) {
		return 1L
	}

	var q = get_power(x, p / 2L, mod)
	q = (q * q) % mod
	if (p % 2L > 0L) {
		q = (q * x) % mod
	}

	return q
}

fun main() {
	val (n, k) = nextInts()
	val mod = 1000000007L

	var fact = ArrayList<Long>()
	fact.add(1L)
	for (i in 1..(2 * n)) {
		fact.add(fact[i - 1] * 1L * i % mod)
	}

	var inv = ArrayList<Long>()
	for (i in 0..(2 * n)) {
		inv.add(0)
	}
	inv[2 * n] = get_power(fact[2 * n], mod - 2L, mod)
	var idx = 2 * n - 1
	while (idx >= 0) {
		inv[idx] = inv[idx + 1] * 1L * (idx + 1) % mod
		idx -= 1
	}

	var ans = ArrayList<Long>()
	for (i in 0..n) {
		ans.add(0L)
	}

	if (k == 0) {
		ans[n] = 1L
	} else {
		for (i in 1..(n - 1)) {
			// (n - i - 1) choose (k - 1)
			if (n - i - 1 >= k - 1) {
				var up = fact[n - i - 1]
				var down = inv[k - 1] * inv[(n - i - 1) - (k - 1)] % mod
				up = up * down % mod
				up = up * 2L % mod
				ans[i] = up
			}
			if (n >= i + 2 && n - i - 2 >= k - 2) {
				var up = fact[n - i - 2]
				var down = inv[k - 2] * inv[(n - i - 2) - (k - 2)] % mod
				up = up * down % mod
				up = up * 1L * (n - i - 1) % mod
				ans[i] = (ans[i] + up) % mod
			}
		}

		for (i in 1..(n - 1)) {
			// (2n - 2 - (i - 1)) choose (k - 1)
			if (2 * n - 3 - (i - 1) >= k - 1) {
				var up = fact[2 * n - 3 - (i - 1)]
				var down = inv[k - 1] * inv[2 * n - 3 - (i - 1) - (k - 1)] % mod
				up = up * down % mod
				ans[0] = (ans[0] + up) % mod
			}

			// minus n - 1 choose k - 1
			if (n - 2 >= k - 1) {
				var up = fact[n - 2]
				var down = inv[k - 1] * inv[n - 2 - (k - 1)] % mod
				up = up * down % mod
				ans[0] = (ans[0] + mod - up) % mod
			}
		}
	}

	println(ans.joinToString(" "))
}
