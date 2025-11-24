fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var T = nextInt()
	while (T > 0) {
		T -= 1;
		var n = nextInt();
		var a = nextInts();
 
		var rightIdx = IntArray(n);
		var minValue = IntArray(n);
		var maxValue = IntArray(n);
 
		rightIdx[n - 1] = n - 1;
		minValue[n - 1] = a[n - 1];
		maxValue[n - 1] = a[n - 1];
 
		for (i in (n - 2) downTo 0) {
			rightIdx[i] = i;
			minValue[i] = a[i];
			maxValue[i] = a[i];
			
			while (true) {
				var r_idx = rightIdx[i] + 1;
				if (r_idx >= n) {
					break;
				}
 
				// Check if the two intervals can be merged
				if (maxValue[i] + 1 < a[r_idx] || a[r_idx] + 1 < minValue[i]) {
					break;
				}
 
				// Now merge the two
				rightIdx[i] = rightIdx[r_idx];
				minValue[i] = minOf(minValue[i], minValue[r_idx]);
				maxValue[i] = maxOf(maxValue[i], maxValue[r_idx]);
			}
		}
 
		var ret = 0L;
		for (i in 0..(n - 1)) {
			ret += 1L * (rightIdx[i] - i + 1);
		}
 
		println(ret)
	}
}
