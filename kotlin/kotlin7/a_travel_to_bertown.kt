// Codeforces 1533 (Kotlin Heroes: Episode 7) — A. Travel to Bertown
// Submission: https://codeforces.com/contest/1533/submission/121347083

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun main () {
	var T = nextInt()
	for (it in 0..T - 1) {
		var (n, k) = nextInts()
		var ans = k - 1
		for (i in 0..n - 1) {
			var (l, r) = nextInts()
			if (l <= k && ans < r) {
				ans = r
			}
		}
		println(ans - k + 1)
	}
}
