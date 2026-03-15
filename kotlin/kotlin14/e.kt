import kotlin.math.*;

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }
 
fun main() {
	var (n, Q) = nextInts();
	var a = nextInts();
	var indices = nextLongs();

	var cuts = ArrayList<Long>();
	cuts.add(1L);
	for (i in 1 until n) {
		if (a[i] == a[i - 1]) {
			cuts.add(cuts[i - 1] + 2L);
		} else {
			cuts.add(cuts[i - 1] + 1L * abs(a[i] - a[i - 1]));
		}
	}

	for (i in 0 until Q) {
		var pos = indices[i];
		if (pos > cuts[n - 1]) {
			print(-1)
		} else {
			var low = 0;
			var high = n - 1;
			var choice = -1;
			while (low <= high) {
				var mid = (low + high) / 2;
				if (pos <= cuts[mid]) {
					choice = mid;
					high = mid - 1;
				} else {
					low = mid + 1;
				}
			}

			// if choice falls into a cut point, print the value at that cut point
			if (pos == cuts[choice]) {
				print(a[choice]);
			} else if (choice > 0 && a[choice] == a[choice - 1]) {
				// if a[choice] == a[choice - 1], then the interested position
				// can take either a[choice] + 1 or a[choice] - 1, hence we print 0
				print(0)
			} else {
				// interpolate from a[choice] to a[choice - 1] to get the value
				var diffs = cuts[choice] - pos;
				var ans = a[choice].toLong();
				if (a[choice] > a[choice - 1]) {
					ans -= diffs;
				} else {
					ans += diffs;
				}
				print(ans);
			}
		}

		if (i < Q - 1) {
			print(" ")
		} else {
			println()
		}
	}
}