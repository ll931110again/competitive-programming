// Codeforces 1297 (Kotlin Heroes: Episode 3) — C. Dream Team
// Submission: https://codeforces.com/contest/1297/submission/313151940

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var T = nextInt();
	for (it in 0..(T - 1)) {
		var n = nextInt();
		var a = nextInts();

		var masks = ArrayList<Int>();
		for (i in 0..(n - 1)) {
			if (a[i] > 0) {
				masks.add(1);
			} else {
				masks.add(0);
			}
		}

		var positive_idx = -1;
		var negative_idx = -1;
		for (i in 0..(n - 1)) {
			if (a[i] > 0) {
				if (positive_idx < 0 || a[positive_idx] > a[i]) {
					positive_idx = i;
				}
			} else if (a[i] < 0) {
				if (negative_idx < 0 || a[negative_idx] < a[i]) {
					negative_idx = i;
				}
			}
		}

		if (negative_idx >= 0 && -a[negative_idx] < a[positive_idx]) {
			masks[negative_idx] = 1;
		} else {
			masks[positive_idx] = 0;
		}

		var ss = 0;
		for (i in 0..(n - 1)) {
			ss += a[i] * masks[i];
		}

		println(ss);
		for (i in 0..(n - 1)) {
			print(masks[i]);
		}
		println();
	}
}
