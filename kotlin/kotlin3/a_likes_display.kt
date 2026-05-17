// Codeforces 1297 (Kotlin Heroes: Episode 3) — A. Likes Display
// Submission: https://codeforces.com/contest/1297/submission/313150329

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
	var T = nextInt()
	for (it in 0..(T - 1)) {
		var n = nextInt()
		if (n < 1000) {
			println(n)
		} else if (n < 999500) {
			var thousand = n / 1000
			var modulo = n % 1000
			if (modulo >= 500) {
				thousand += 1
			}
			print(thousand)
			println("K")
		} else {
			var million = n / 1000000
			var modulo = n % 1000000
			if (modulo >= 500000) {
				million += 1
			}
			print(million)
			println("M")
		}
	}
}
