fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLong() = next().toLong()
fun nextLongs() = next().split(" ").map { it.toLong() }
 
fun main() {
	var T = nextInt();
	for (it in 0 until T) {
		var (_n, k) = nextLongs();
		var n = _n.toInt();
		var a = nextLongs();
 
		var maxVar = a[0];
		var minVar = a[0];
		var used = false;
		for (i in 0 until n) {
			if (maxVar < a[i]) {
				maxVar = a[i];
			}
			if (minVar > a[i]) {
				minVar = a[i];
			}
		}
 
		var minOps = 0L;
		for (i in 0 until n) {
			minOps += (maxVar - a[i]);
		}
 
		if (minOps > k) {
			println(-1);
		} else {
			var target = maxVar + (k - minOps) / n;
			var ans = 0L;
 
			for (i in 0 until n) {
				if (a[i] == minVar) {
					if (!used) {
						used = true;
					} else if (a[i] < target) {
						ans += (target - a[i] - 1L);
					}
				} else {
					ans += (target - a[i]);
				}
			}
 
			println(ans);
		}
	} 
}