// Codeforces 1346 (Kotlin Heroes: Episode 4) — B. Boot Camp
// Submission: https://codeforces.com/contest/1346/submission/81896514

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
	var T = nextInt()
	for (i in 0..T - 1) {
		var (n, k1, k2) = nextInts()
		var arr = readLine()!!.toString()

		var ret = 0
		var prev = 0
		for (j in 0..n - 1) {
			var delta = 0
			if (arr[j] == '1') {
				delta = k1
				if (delta > k2 - prev) {
					delta = k2 - prev
				}
			}
			ret += delta
			prev = delta
		}

		println(ret)
	}	
}
