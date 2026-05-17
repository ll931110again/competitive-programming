// Codeforces 1488 (Kotlin Heroes: Episode 6) — F. Dogecoin
// Submission: https://codeforces.com/contest/1488/submission/313542301

import kotlin.math.abs

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun main() {
	var n = nextInt()
	var a = nextLongs()
	var K = 18

	var pos = ArrayList<ArrayList<Int>>()
	var sumValue = ArrayList<ArrayList<Long>>()
	
	var row = ArrayList<Int>()
	var sumRow = ArrayList<Long>()

	var lidx = ArrayList<Int>()
	for (i in 0 until n) {
		while (lidx.isNotEmpty() && a[lidx[lidx.size - 1]] <= a[i]) {
			lidx.removeLast()
		}
		if (lidx.isEmpty()) {
			row.add(-1)
			sumRow.add(1L * (i + 1) * a[i])
		} else {
			var idx = lidx[lidx.size - 1]
			row.add(idx)
			sumRow.add(1L * (i - idx) * a[i])
		}

		lidx.add(i)
	}
	pos.add(row)
	sumValue.add(sumRow)

	for (k in 1 until K) {
		row = ArrayList<Int>()
		sumRow = ArrayList<Long>()

		for (i in 0 until n) {
			var prev = pos[k - 1][i]
			if (prev >= 0) {
				row.add(pos[k - 1][prev])
				sumRow.add(sumValue[k - 1][i] + sumValue[k - 1][prev])
			} else {
				row.add(-1)
				sumRow.add(sumValue[k - 1][i])
			}
		}

		pos.add(row)
		sumValue.add(sumRow)
	}

	var Q = nextInt()
	for (iq in 0 until Q) {
		var (L, R) = nextInts()
		L--
		R--
		var ans = 0L
		var currentPos = R
		for (k in K - 1 downTo 0) {
			if (pos[k][currentPos] >= 0 && pos[k][currentPos] >= L - 1) {
				ans += sumValue[k][currentPos]
				currentPos = pos[k][currentPos]
			}
		}
		if (currentPos >= L) {
			ans += a[currentPos] * (currentPos - L + 1)
		}

		print(ans)
		if (iq == Q - 1) {
			println()
		} else {
			print(" ")
		}
	}
}
