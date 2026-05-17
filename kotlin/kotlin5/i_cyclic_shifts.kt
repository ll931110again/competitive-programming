// Codeforces 1431 (Kotlin Heroes 5: ICPC Round) — I. Cyclic Shifts
// Submission: https://codeforces.com/contest/1431/submission/350435481

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun main() {
	var (n, m, q) = nextInts()
	var s = Array(n) { readLine()!! }

	var c = Array(m + 1) { IntArray(n) { 0 } }
	var rc = Array(m + 1) { IntArray(n) { 0 } }

	for (j in m - 1 downTo 0) {
		var cur = Array(n) { intArrayOf(s[it][j].code, c[j + 1][it], it) }
		cur.sortWith(compareBy({ it[0] }, { it[1] }))
		for (i in 0 until n) {
			rc[j][i] = cur[i][2]
		}

		c[j][cur[0][2]] = 0
		for (i in 1 until n) {
			var add = if (cur[i][0] == cur[i - 1][0] && cur[i][1] == cur[i - 1][1]) 0 else 1
			c[j][cur[i][2]] = c[j][cur[i - 1][2]] + add
		}
	}

	while (q > 0) {
		q -= 1
		var t = readLine()!!
		var ans = 0
		var j = 0

		while (j < m) {
			var (nj, L, R) = intArrayOf(j, 0, n - 1)
			while (nj < m) {
				var (l1, r1, nL) = intArrayOf(L, R, R + 1)
				while (l1 <= r1) {
					var mid = (l1 + r1) / 2
					if (s[rc[j][mid]][nj] >= t[nj]) {
						nL = mid
						r1 = mid - 1
					} else {
						l1 = mid + 1
					}
				}

				var (l2, r2, nR) = intArrayOf(nL, R, nL - 1)
				while (l2 <= r2) {
					var mid = (l2 + r2) / 2
					if (s[rc[j][mid]][nj] <= t[nj]) {
						nR = mid
						l2 = mid + 1
					} else {
						r2 = mid - 1
					}
				}

				if (nL > nR) {
					break
				}

				L = nL
				R = nR
				nj++
			}

			if (j == nj) {
				ans = 0
				break
			}

			ans += 1
			j = nj
		}

		println(ans - 1)
	}
}
