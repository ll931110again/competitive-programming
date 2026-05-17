// Codeforces 1488 (Kotlin Heroes: Episode 6) — E. Palindromic Doubles
// Submission: https://codeforces.com/contest/1488/submission/313515537

import kotlin.math.abs

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun solve(): Int {
	var n = nextInt()
	var a = nextInts()

	var opposite = ArrayList<Int>()
	var pos = ArrayList<Int>()

	for (i in 0 until n + 1) {
		opposite.add(-1)
		pos.add(-1)
	}

	for (i in n - 1 downTo 0) {
		if (pos[a[i]] >= 0) {
			opposite[i] = pos[a[i]]
		}
		pos[a[i]] = i
	}

	var maxIdx = ArrayList<Int>()
	for (i in 0 until n + 1) {
		maxIdx.add(-1)
	}
	maxIdx[0] = n + 1

	var maxLen = 0
	var ans = 1
	for (i in 0 until n + 1) {
		if (opposite[i] >= 0) {
			var low = 0
			var high = maxLen

			var len = 0
			while (low <= high) {
				var mid = (low + high) / 2
				if (opposite[i] < maxIdx[mid]) {
					len = mid
					low = mid + 1
				} else {
					high = mid - 1
				}
			}

			len += 1
			maxLen = maxOf(maxLen, len)
			maxIdx[len] = maxOf(maxIdx[len], opposite[i])

			ans = maxOf(ans, 2 * len)
			if (i + 1 < opposite[i]) {
				ans = maxOf(ans, 2 * len + 1)
			}
		}
	}

	return ans
}

fun main() {
	var T = nextInt()
	for (it in 0 until T) {
		println(solve())
	}
}
