// Codeforces 1171 (Kotlin Heroes: Practice 1) — B. New Year and the Christmas Ornament
// Submission: https://codeforces.com/contest/1171/submission/54505037

fun main() {
	val (a, b, c) = readLine()!!.split(' ').map(String::toInt)
	var x = a
	if (x > b - 1) {
		x = b - 1
	}
	if (x > c - 2) {
		x = c - 2
	}
	println(3 * (x + 1))
}
