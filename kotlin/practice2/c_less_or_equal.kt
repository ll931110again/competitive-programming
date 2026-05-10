// Codeforces 1212 (Kotlin Heroes: Practice 2) — C. Less or Equal
// Submission: https://codeforces.com/contest/1212/submission/60050561

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
	var (n, k) = nextInts()
	var a = ArrayList(nextInts())
	a.sort()

	var ans = 0

	if (k == n) {
		ans = a[k - 1]
	} else if (k == 0) {
		ans = a[0] - 1
	} else if (k < n && a[k - 1] == a[k]) {
		ans = -1
	} else {
		ans = a[k - 1]
	}

	if (ans > 1000000000 || ans < 1) {
		ans = -1
	}
	println(ans)
}
