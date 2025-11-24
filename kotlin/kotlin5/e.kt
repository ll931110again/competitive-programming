import kotlin.math.abs
 
fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun solve() {
	var n = nextInt()
	var a = nextInts()
	var b = nextInts()
 
	var perms = ArrayList<Int>()
	var ret = -1
 
	// match the first k elements of A to the last k elements of B
	for (k in 0..(n - 1)) {
		var value = -1;
		for (i in 0..(k - 1)) {
			var x = abs(a[i] - b[n - k + i]);
			if (value < 0 || value > x) {
				value = x
			}
		}
		for (i in k..(n - 1)) {
			var x = abs(a[i] - b[i - k]);
			if (value < 0 || value > x) {
				value = x
			}
		}
 
		if (ret < value) {
			ret = value;
			var ans = ArrayList<Int>();
			for (i in 0..(k - 1)) {
				ans.add(n + 1 - k + i)
			}
			for (i in k..(n - 1)) {
				ans.add(i - k + 1)
			}
 
			perms = ans;
		}
	}
 
	for (x in perms) {
		print(x)
		print(' ')
	}
	println()
}
 
fun main() {
	var T = nextInt()
	while (T > 0) {
		T -= 1;
		solve();
	}
}
