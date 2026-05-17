// Codeforces 1170 (Kotlin Heroes: Episode 1) — C. Minus and Minus Give Plus
// Submission: https://codeforces.com/contest/1170/submission/54757250

fun next() = readLine()!!
fun nextInt() = next().toInt()

fun solve(
	s: String,
	t: String,
): Boolean {
	var i = 0
	var j = 0
	var m = s.length
	var n = t.length

	while (i < m && j < n) {
		if (s[i] == t[j]) {
			i += 1
			j += 1
			continue
		}
		if (s[i] == '+' && t[j] == '-') {
			return false
		}
		if (s[i] == '-' && t[j] == '+') {
			if (i + 1 >= m || s[i + 1] == '+') {
				return false
			}
			i += 2
			j += 1
			continue
		}
	}

	return (i >= m && j >= n)
}

fun main() {
	val T = nextInt()
	for (i in 1..T) {
		var s = readLine()!!.toString()
		var t = readLine()!!.toString()
		if (solve(s, t)) {
			println("YES")
		} else {
			println("NO")
		}
	}
}
