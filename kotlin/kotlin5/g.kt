fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var (n, k) = nextInts()
	var a = nextInts()
	a = a.sorted()
 
	var ss = ArrayList<Int>()
	ss.add(0)
	for (i in 0..(n - 1)) {
		ss.add(ss[i] + a[i])
	}
 
	var dp = ArrayList<ArrayList<Int>>()
	var row = ArrayList<Int>()
	for (j in 0..k) {
		if (j == 0) {
			row.add(0)
		} else {
			row.add(-1000000000)
		}
	}
	dp.add(row)
 
	for (i in 1..n) {
		row = ArrayList<Int>();
		for (j in 0..k) {
			var x = dp[i - 1][j];
			for (t in 1..j) {
				if (i - 2 * t < 0) {
					break
				}
				var y = dp[i - 2 * t][j - t];
				y += (ss[i] - ss[i - t]);
				y -= (ss[i - t] - ss[i - 2 * t]);
				if (x < y) {
					x = y
				}
			}
			row.add(x)
		}
		dp.add(row)
	}
 
	println(dp[n][k])
}
