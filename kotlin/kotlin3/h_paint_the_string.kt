// Codeforces 1297 (Kotlin Heroes: Episode 3) — H. Paint the String
// Submission: https://codeforces.com/contest/1297/submission/313265985

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun solve(s: String): String {
	var n = s.length

	var dp = ArrayList<ArrayList<String>>()
	var trace = ArrayList<ArrayList<Int>>()

	for (i in 0 until n + 2) {
		var row = ArrayList<String>()
		for (j in 0 until n + 2) {
			if (i + j == 0) {
				row.add("")
			} else {
				var sb = StringBuilder("")
				sb.append(('z'.code + 1).toChar())
				row.add(sb.toString())
			}
		}
		dp.add(row)

		var traceRow = ArrayList<Int>()
		for (j in 0 until n + 1) {
			traceRow.add(-1)
		}
		trace.add(traceRow)
	}
	trace[0][0] = 0

	var ans = ""
	var configuration = ""

	fun buildState(
		xi: Int,
		xj: Int,
		appendChar: Char,
	) {
		var choice = dp[xi][xj]
		if (ans.isNotEmpty() && ans <= choice) {
			return
		}

		var i = xi
		var j = xj
		var myConfig = StringBuilder("")
		while (i + j > 0) {
			if (trace[i][j] == 0) {
				myConfig.append('R')
				i--
			} else {
				myConfig.append('B')
				j--
			}
		}

		myConfig.reverse()
		while (myConfig.length < n) {
			myConfig.append(appendChar)
		}

		ans = choice
		configuration = myConfig.toString()
	}

	for (i in 0 until n + 1) {
		for (j in 0 until i + 1) {
			if (trace[i][j] < 0 || i + j >= n) {
				continue
			}

			if (i + j > 0 && (dp[i][j].isEmpty() || dp[i][j][0] > 'z')) {
				continue
			}

			if (i + j == 0) {
				var tmp = StringBuilder(dp[i][j])
				tmp.append(s[0])
				dp[1][0] = tmp.toString()
				trace[1][0] = 0
				continue
			}

			if (i > j && s[i + j] < dp[i][j][j]) {
				buildState(i, j, 'B')
			} else {
				var tmp = StringBuilder(dp[i][j]).append(s[i + j]).toString()
				if (dp[i + 1][j] > tmp) {
					dp[i + 1][j] = tmp
					trace[i + 1][j] = 0
				}

				if (i > j && s[i + j] == dp[i][j][j]) {
					if (dp[i][j + 1] > dp[i][j]) {
						dp[i][j + 1] = dp[i][j]
						trace[i][j + 1] = 1
					}
				}
			}
		}
	}

	for (i in 0 until (n + 1)) {
		var j = n - i
		if (i >= j && trace[i][j] >= 0) {
			buildState(i, j, 'B')
		}
	}

	return configuration
}

fun main() {
	var T = nextInt()
	for (it in 0 until T) {
		var s = readLine()!!
		println(solve(s))
	}
}
