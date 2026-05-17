// Codeforces 1910 (Kotlin Heroes: Episode 9 (Unrated, T-Shirts + Prizes!)) — C. Poisonous Swamp
// Submission: https://codeforces.com/contest/1910/submission/324554385

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun solve(s: String): Int {
	var n = s.length
	var i = 0
	var j = 0
	var ans = 0
	
	while (i < n) {
		if (s[i] == '.') {
			i += 1
			continue
		}
		var j = i
		while (j < n && s[j] == '*') {
			j += 1
		}
		// Skip one lily pad
		ans += j - i - 1
		i = j + 1
	}

	return ans
}

fun main() {
	var T = nextInt()
	for (it in 0 until T) {
		var n = nextInt()
		var s1 = readLine()!!
		var s2 = readLine()!!
		var ans = solve(s1) + solve(s2)
		println(ans)
	}
}
