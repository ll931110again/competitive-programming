fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var T = nextInt()
	for (it in 0..T - 1) {
		var (n, k, x, y) = nextInts()
		var arr = nextInts()
		arr = arr.sorted()
 
		var ret = 0L
		var ts = 0L
		for (i in 0..n - 1) {
			ts += arr[i]
		}
		var i = n - 1
		while (ts > 1L * n * k) {
			ts -= arr[i]
			ret += x
			i -= 1
		}
 
		var exceed = false
		for (j in 0..i) {
			if (arr[j] > k) {
				exceed = true
			}
		}
		if (exceed == true) {
			ret += y
		}
 
		var ret2 = 0L
		i = n - 1
		while (i >= 0 && arr[i] > k) {
			ret2 += x
			i -= 1
		}
 
		if (ret > ret2) {
			ret = ret2
		}
 
		println(ret)
	}
}
