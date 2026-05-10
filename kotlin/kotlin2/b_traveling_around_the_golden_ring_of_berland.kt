// Codeforces 1211 (Kotlin Heroes: Episode 2) — B. Traveling Around the Golden Ring of Berland
// Submission: https://codeforces.com/contest/1211/submission/60224413

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
	var n = nextInt()
	var a = nextInts()

	var maxi = 0;
	var pos = 0
	for (i in 0..n - 1) {
		if (a[pos] <= a[i]) {
			pos = i;
			maxi = a[i];
		}
	}

	if (maxi == 0) {
		println("1");
	} else {
		var ret = 1L * n * (maxi - 1) + (pos + 1);
		println(ret);
	}
}
