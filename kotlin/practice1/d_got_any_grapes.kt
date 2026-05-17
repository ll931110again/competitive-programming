// Codeforces 1171 (Kotlin Heroes: Practice 1) — D. Got Any Grapes?
// Submission: https://codeforces.com/contest/1171/submission/54505528

fun solve(
	x: Int,
	y: Int,
	z: Int,
	a: Int,
	b: Int,
	c: Int,
): Boolean {
	if (a < x) {
		return false
	}
	if (a + b < x + y) {
		return false
	}
	if (a + b + c < x + y + z) {
		return false
	}
	return true
}

fun main() {
	val (x, y, z) = readLine()!!.split(' ').map(String::toInt)
	val (a, b, c) = readLine()!!.split(' ').map(String::toInt)
	if (solve(x, y, z, a, b, c)) {
		println("YES")
	} else {
		println("NO")
	}
}
