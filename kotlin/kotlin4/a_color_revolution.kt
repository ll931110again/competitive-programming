// Codeforces 1346 (Kotlin Heroes: Episode 4) — A. Color Revolution
// Submission: https://codeforces.com/contest/1346/submission/81895474

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
	var T = nextInt()
	for (i in 0..T - 1) {
		var (n, k) = nextInts()
		var L1 = n / (1 + k + k * k + k * k * k)
		var L2 = L1 * k
		var L3 = L2 * k
		var L4 = L3 * k
		println("$L1 $L2 $L3 $L4")
	}	
}
