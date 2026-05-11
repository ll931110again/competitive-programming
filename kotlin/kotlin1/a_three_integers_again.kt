// Codeforces 1170 (Kotlin Heroes: Episode 1) — A. Three Integers Again
// Submission: https://codeforces.com/contest/1170/submission/54755525

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
	val t = nextInt()
	for (i in 1..t) {
		val (ab, ac) = nextInts()
		if (ab <= ac) {
			println("${ab - 1} 1 ${ac - (ab - 1)}")
		} else {
			println("${ac - 1} ${ab - (ac - 1)} 1")
		}
	}
}
