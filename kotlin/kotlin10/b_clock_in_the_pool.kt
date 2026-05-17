// Codeforces 1958 (Kotlin Heroes: Episode 10) — B. Clock in the Pool
// Submission: https://codeforces.com/contest/1958/submission/260816169

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
	val T = nextInt()
	for (i in 1..T) {
		var (k, m) = nextInts()
		m %= (3 * k)
		if (m >= 2 * k) {
			println(0)
		} else {
			println(2 * k - m)
		}
	}
}
