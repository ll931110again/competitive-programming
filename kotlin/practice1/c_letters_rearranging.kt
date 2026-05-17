// Codeforces 1171 (Kotlin Heroes: Practice 1) — C. Letters Rearranging
// Submission: https://codeforces.com/contest/1171/submission/54505449

fun solve(x: String): String {
	var m = x.length
	var allEqual = true
	for (i in 1..(m - 1)) {
		if (x.get(0) != x.get(i)) {
			allEqual = false
		}
	}
	if (allEqual) {
		return "-1"
	}
	var chars: MutableList<Char> = mutableListOf()
	for (i in 0..(m - 1)) {
		chars.add(x.get(i))
	}
	for (i in 1..(m - 1)) {
		if (chars.elementAt(0) != chars.elementAt(i)) {
			var tmp = chars.elementAt(i)
			chars.set(i, chars.elementAt(m - 1))
			chars.set(m - 1, tmp)
			break
		}
	}
	return chars.joinToString(prefix = "", postfix = "", separator = "")
}

fun main() {
	var T = readLine()!!.toInt()
	for (i in 1..T) {
		var s = readLine()!!.toString()
		println(solve(s))
	}
}
