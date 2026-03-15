import kotlin.math.*;

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var T = nextInt();
	for (i in 0 until T) {
		var (n, m) = nextInts();
		var a = nextInts();
		var b = nextInts();
		a = a.sorted();
		b = b.sorted();

		var possible = false;
		for (i in 0 until n) {
			// If a[i] is the boundary element, it's invalid.
			if (n > 1 && (i == 0 || i == n - 1)) {
				continue;
			}

			// If there's only one element in b, it's valid if it matches a[i].
			if (m == 1) {
				if (a[i] == b[0]) {
					possible = true;
				}
				continue;
			}

			// Otherwise binary search on the non-boundary elements of b.
			var low = 1;
			var high = m - 2;
			while (low <= high) {
				var mid = (low + high) / 2;
				if (a[i] == b[mid]) {
					possible = true;
					break;
				} else if (a[i] < b[mid]) {
					high = mid - 1;
				} else {
					low = mid + 1;
				}
			}
		}

		if (possible) {
			println("YES");
		} else {
			println("NO");
		}
	}
}