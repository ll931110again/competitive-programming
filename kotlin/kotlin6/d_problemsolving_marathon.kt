// Codeforces 1488 (Kotlin Heroes: Episode 6) — D. Problemsolving Marathon
// Submission: https://codeforces.com/contest/1488/submission/313497303

import kotlin.math.abs

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun solve(
	n: Long,
	s: Long,
): Long {
	if (n == 1L) {
		return s
	}

	var low = s / n
	if (s % n > 0) {
		low++
	}
	var high = s
	var ans = low

	while (low <= high) {
		var mid = (low + high) / 2

		var sumValue = mid
		var curValue = mid
		for (i in 1L until n) {
			curValue = (curValue + 1L) / 2L
			sumValue += curValue
			if (curValue == 1L) {
				sumValue += (n - i - 1)
				break
			}
		}

		if (sumValue <= s) {
			ans = mid
			low = mid + 1
		} else {
			high = mid - 1
		}
	}

	return ans
}

fun main() {
	var T = nextInt()
	for (it in 0 until T) {
		var (n, s) = nextLongs()
		println(solve(n, s))
	}
}
