// Codeforces 2011 (Kotlin Heroes: Episode 11) — B. Shuffle
// Submission: https://codeforces.com/contest/2011/submission/283848771

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
	var T = nextInt()
	while (T > 0) {
		T -= 1
		var n = nextInt()
		print("1 ")
		for (i in n downTo 2) {
			print(i)
			print(" ")
		}
		println()
	}
}
