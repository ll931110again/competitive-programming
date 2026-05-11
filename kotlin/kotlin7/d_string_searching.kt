// Codeforces 1533 (Kotlin Heroes: Episode 7) — D. String Searching
// Submission: https://codeforces.com/contest/1533/submission/121361075

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun main () {
	var (n, m) = nextInts()
	var dict = HashSet<String>()
	for (i in 0 until n) {
		var x = readLine()!!
		dict.add(x)
	}

	var q = nextInt()
	for (it in 0 until q) {
		var x = readLine()!!
		var ans = 0
		var removed = HashSet<String>()

		for (i in 0 until m + 1) {
			var sub = x.substring(0, i) + x.substring(i + 1, m + 1)
			if (!removed.contains(sub) && dict.contains(sub)) {
				ans++
			}
			removed.add(sub)
		}
		println(ans)
	}
}
