// Codeforces 1533 (Kotlin Heroes: Episode 7) — B. Nearest Point Function
// Submission: https://codeforces.com/contest/1533/submission/121346480

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun main() {
	var T = nextInt()
	for (i in 0..T - 1) {
		var n = nextInt()
		var a = nextInts()
		var bad = false
		for (j in 0..n - 2) {
			if ((a[j + 1] - a[j]) % 2 == 0) {
				bad = true
			}
		}
		if (bad) {
			println("YES")
		} else {
			println("NO")
		}
	}
}
