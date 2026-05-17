// Codeforces 1211 (Kotlin Heroes: Episode 2) — A. Three Problems
// Submission: https://codeforces.com/contest/1211/submission/60223993

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
	var n = nextInt()
	var a = nextInts()
	var minIdx = 0
	var maxIdx = 0
	var midIdx = -1
	for (i in 0..n - 1) {
		if (a[minIdx] > a[i]) {
			minIdx = i
		}
		if (a[maxIdx] < a[i]) {
			maxIdx = i
		}
	}

	for (i in 0..n - 1) {
		if (a[i] > a[minIdx] && a[i] < a[maxIdx]) {
			midIdx = i
			break
		}
	}

	if (midIdx < 0) {
		println("-1 -1 -1")
	} else {
		println("${minIdx + 1} ${midIdx + 1} ${maxIdx + 1}")
	}
}
