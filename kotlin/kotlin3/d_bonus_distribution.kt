// Codeforces 1297 (Kotlin Heroes: Episode 3) — D. Bonus Distribution 
// Submission: https://codeforces.com/contest/1297/submission/313153704

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var T = nextInt();
	for (it in 0..(T - 1)) {
		var (n, K) = nextInts();
		var a = nextInts();

		var orders = ArrayList<Int>();
		for (i in 0..(n - 1)) {
			orders.add(i);
		}

		var valueComparator = Comparator { i : Int, j : Int -> a[j] - a[i]};
		orders.sortWith(valueComparator);

		var addedValue = ArrayList<Int>();
		for (i in 0..(n - 1)) {
			addedValue.add(0);
		}

		var low = 0;
		var high = 1000000000;
		var ans = high;
		while (low <= high) {
			var tmps = ArrayList<Int>();
			for (i in 0..(n - 1)) {
				tmps.add(0);
			}

			var mid = (low + high) / 2;
			tmps[orders[0]] = minOf(K, mid);

			var rem = maxOf(K - mid, 0);
			var bound = a[orders[0]] + mid;
			for (i in 1..(n - 1)) {
				bound -= 1;
				tmps[orders[i]] = minOf(rem, bound - a[orders[i]]);
				rem -= tmps[orders[i]];
			}

			if (rem == 0) {
				for (i in 0..(n - 1)) {
					addedValue[i] = tmps[i];
				}
				high = mid - 1;
			} else {
				low = mid + 1;
			}
		}

		for (i in 0..(n - 1)) {
			print(addedValue[i]);
			print(" ");
		}
		println();
	}
}
