// Codeforces 1533 (Kotlin Heroes: Episode 7) — E. Chess Team Forming
// Submission: https://codeforces.com/contest/1533/submission/121357905

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun main() {
	var n = nextInt()
	var myTeam = nextInts().toMutableList()
	var oppTeam = nextInts().toMutableList()
	myTeam.sort()
	oppTeam.sort()

	var prefix = IntArray(n)
	for (i in 0..n - 1) {
		prefix[i] = oppTeam[i] - myTeam[i]
		if (i > 0) {
			prefix[i] = maxOf(prefix[i], prefix[i - 1])
		}
	}

	var suffix = IntArray(n)
	for (i in n - 1 downTo 0) {
		suffix[i] = oppTeam[i + 1] - myTeam[i]
		if (i < n - 1) {
			suffix[i] = maxOf(suffix[i], suffix[i + 1])
		}
	}

	var q = nextInt()
	var queries = nextInts()
	var res = IntArray(q)
	for (i in 0..q - 1) {
		var pos = myTeam.binarySearch(queries[i])
		if (pos < 0) {
			pos = -pos - 1
		}

		var ans = oppTeam[pos] - queries[i]
		if (pos > 0) {
			ans = maxOf(ans, prefix[pos - 1])
		}
		if (pos < n) {
			ans = maxOf(ans, suffix[pos])
		}
		res[i] = ans
	}
	println(res.joinToString(" "))
}
