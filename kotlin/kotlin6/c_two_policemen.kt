// Codeforces 1488 (Kotlin Heroes: Episode 6) — C. Two Policemen
// Submission: https://codeforces.com/contest/1488/submission/313482705

import kotlin.math.abs

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun solveBoundary(n: Int, x: Int, y: Int, m1: Int, m2: Int): Int {
	var costX = minOf(2 * (x - 1) + (m1 - x), 2 * abs(m1 - x) + (x - 1));
	var costY = minOf(2 * (n - y) + abs(m2 - y), 2 * abs(m2 - y) + (n - y));
	return maxOf(costX, costY);
}

fun solveFn(n: Int, x: Int, y: Int, mid: Int): Int {
	return minOf(solveBoundary(n, x, y, mid, mid), solveBoundary(n, x, y, mid, mid + 1));
}

fun solve(n: Int, _x: Int, _y: Int): Int {
	var x = minOf(_x, _y);
	var y = maxOf(_x, _y);

	var ans = 2 * n;
	for (i in 1 until (n + 1)) {
		ans = minOf(ans, solveFn(n, x, y, i));
	}
	return ans;
}

fun main() {
	var T = nextInt();
	for (it in 0 until T) {
		var (n, x, y) = nextInts();
		println(solve(n, x, y));
	}
}
