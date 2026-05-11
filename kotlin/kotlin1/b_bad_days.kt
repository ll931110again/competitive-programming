// Codeforces 1170 (Kotlin Heroes: Episode 1) — B. Bad Days
// Submission: https://codeforces.com/contest/1170/submission/54756546

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
	val n = nextInt()
	val a = nextInts()
	var t1 = 0
	var t2 = 0
	var ret = 0
	for (i in 0..n - 1) {
		if (a[i] < t2) {
			ret += 1
		}
		if (a[i] >= t1) {
			t2 = t1
			t1 = a[i]
		} else if (a[i] >= t2) {
			t2 = a[i]
		}
	}
	println(ret)
}
