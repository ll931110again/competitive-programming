// Codeforces 1171 (Kotlin Heroes: Practice 1) — E. Doggo Recoloring
// Submission: https://codeforces.com/contest/1171/submission/54505656

fun solve(
	s: String,
	n: Int,
): Boolean {
	if (n == 1) {
		return true
	}
	
	var allChars: MutableSet<Char> = mutableSetOf()
	for (i in 0..(n - 1)) {
		allChars.add(s.get(i))
	}
	return (allChars.size < n)
}

fun main() {
	var n = readLine()!!.toInt()
	var s = readLine()!!.toString()
	if (solve(s, n)) {
		println("Yes")
	} else {
		println("No")
	}
}
