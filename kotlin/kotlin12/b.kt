fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var T = nextInt();
	for (it in 0 until T) {
		var n = nextInt();
		var _a = nextInts();
		var a = _a.sorted();
 
		var ok = true;
		for (i in 0 until 2 * n) {
			var diff = if (i % 2 == 0) a[i + 1] - a[i] else a[i] - a[i - 1];
			var minDiff = diff;
			if (i > 0) {
				minDiff = minOf(minDiff, a[i] - a[i - 1]);
			}
			if (i + 1 < 2 * n) {
				minDiff = minOf(minDiff, a[i + 1] - a[i]);
			}
 
			if (minDiff < diff) {
				ok = false;
			}
		}
 
		if (ok) {
			println("YES");
		} else {
			println("NO");
		}
	} 
}