// Codeforces 1958 (Kotlin Heroes: Episode 10) — A. 1-3-5
// Submission: https://codeforces.com/contest/1958/submission/260817864

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var s = ArrayList<Int>()
	s.add(0)
	for (i in 1..100) {
		var idx = 1;
		var delta = 1;
		if (i >= 3 && (s[i - idx] + delta > s[i - 3])) {
			idx = 3; delta = 0;
		}
		if (i >= 5 && (s[i - idx] + delta > s[i - 5])) {
			idx = 5; delta = 0;
		}
		s.add(s[i - idx] + delta)
	}
	val T = nextInt()
	for (i in 1..T) {
		val n = nextInt()
		println(s[n])
	}
}
