// Codeforces 1571 (Kotlin Heroes: Episode 8) — B. Epic Novel
// Submission: https://codeforces.com/contest/1571/submission/131349606

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun main() {
	var T = nextInt()
	for (it in 0..T - 1) {
		var n = nextInt()
		var (a, va) = nextInts()
		var (c, vc) = nextInts()
		var b = nextInt()

		while (a < b && va < vc) {
			a += 1
			va += 1
		}

		println(va)
	}
}
