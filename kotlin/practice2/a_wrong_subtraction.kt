// Codeforces 1212 (Kotlin Heroes: Practice 2) — A. Wrong Subtraction
// Submission: https://codeforces.com/contest/1212/submission/60050378

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
	var (x, y) = nextInts()
	while (y > 0) {
		y -= 1
		if (x % 10 == 0) {
			x /= 10
		} else {
			x -= 1
		}
	}
	print(x)
	println()
}
