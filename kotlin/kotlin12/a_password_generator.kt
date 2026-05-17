// Codeforces 2087 (Kotlin Heroes: Episode 12) — A. Password Generator
// Submission: https://codeforces.com/contest/2087/submission/314385004

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
	var T = nextInt()
	for (it in 0 until T) {
		var (a, b, c) = nextInts()
		for (i in 0 until a) {
			if (i % 2 == 0) {
				print('0')
			} else {
				print('1')
			}
		}
		for (i in 0 until b) {
			if (i % 2 == 0) {
				print('A')
			} else {
				print('B')
			}
		}
		for (i in 0 until c) {
			if (i % 2 == 0) {
				print('a')
			} else {
				print('b')
			}
		}
		println()
	}
}
