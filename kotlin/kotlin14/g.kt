/*
 */

import kotlin.math.*;

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }
 
fun isqrt(a : Long) : Long {
	var s = max(0L, sqrt(a.toDouble()).toLong() - 2)
	while ((s + 1) * (s + 1) <= a)
		s++
	return s
}

fun main() {
	var T = nextInt();
	for (it in 0 until T) {
		var (n, m, r) = nextLongs();
		if (n > m) {
			var tmp = n;
			n = m;
			m = tmp;
		}

		var ans = 0L;
		for (x in 0..n) {
			var lower_bound = 0L;
			if (x <= r) {
				lower_bound = 1 + isqrt(r * r - x * x);
			} else {
				lower_bound = max(0L, m - r);
			}

			var upper_bound = 0L;
			if (n - x <= r) {
				upper_bound = m - 1 - isqrt(r * r - (n - x) * (n - x));
			} else {
				upper_bound = min(m, r);
			}

			ans += max(0L, upper_bound - lower_bound + 1);
		}
		println(ans);		
	}
}