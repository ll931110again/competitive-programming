// Codeforces 1488 (Kotlin Heroes: Episode 6) — B. RBS Deletion
// Submission: https://codeforces.com/contest/1488/submission/313487150

import kotlin.math.abs

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun solve(n: Int, K: Int, s: String): Int {
	var numPartitions = 0;
	var delta = 0;
	for (i in 0 until n) {
		if (s[i] == '(') {
			delta++;
		} else {
			delta--;
			if (delta == 0) {
				numPartitions++;
			}
		}
	}
	return minOf(numPartitions + K, n/2);
}

fun main() {
	var T = nextInt();
	for (it in 0 until T) {
		var (n, K) = nextInts();
		var s = readLine()!!;
		println(solve(n, K, s));
	}
}
