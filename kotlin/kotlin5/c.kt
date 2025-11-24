fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }
 
fun main () {
	var T = nextInt()
	for (it in 0..T - 1) {
		var (n, k) = nextInts()
		var a = nextInts()
 
		var ret = 0
		for (i in 1..n) if (i * k <= n) {
			var tmp = 0
			for (j in 1..i) {
				tmp += a[n - i * (k - 1) - j]
			}
			if (ret < tmp) {
				ret = tmp
			}
		}
		println(ret)
	}
}
