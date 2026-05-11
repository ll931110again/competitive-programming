// Codeforces 1297 (Kotlin Heroes: Episode 3) — F. Movie Fan
// Submission: https://codeforces.com/contest/1297/submission/313185321

import java.math.BigInteger
import java.util.PriorityQueue

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

class Item(val idx: Int, val start: Int, val end: Int)

fun solve() {
	var (n, m) = nextInts();
	var a = ArrayList<Item>();

	for (i in 0 until n) {
		var (x, y) = nextInts();
		a.add(Item(i, x, y));
	}

	a.sortWith(compareBy({it.start}, {it.end}));

	var ans = ArrayList<Int>();
	for (i in 0 until n) {
		ans.add(0);
	}

	var gap = 0;
	var ts = 0;
	var i = 0;

	var unusedItems = PriorityQueue< Pair<Int,Int> >(compareBy { it.first });

	while (i < n || unusedItems.isNotEmpty()) {
		while (i < n && a[i].start <= ts) {
			unusedItems.add(Pair(a[i].end, a[i].idx));
			i += 1;
		}

		for (j in 0 until m) {
			if (unusedItems.isEmpty()) {
				break;
			}
			var item = unusedItems.poll();
			gap = maxOf(gap, ts - item.first);
			ans[item.second] = ts;
		}

		ts += 1;
		if (i < n && unusedItems.isEmpty() && ts < a[i].start) {
			ts = a[i].start;
		}
	}

	println(gap);
	for (i in 0 until n) {
		print(ans[i]);
		print(" ");
	}
	println();
}

fun main() {
	var T = nextInt();
	for (it in 0..(T - 1)) {
		solve();
	}
}
