import kotlin.math.*
 
fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var T = nextInt()
	for (it in 0..T - 1) {
		var (n, m) = nextInts()
		var weight = Array(n + 1){0}
		var arrX = Array(m){0}
		var arrY = Array(m){0}
		var arrW = Array(m){0}
 
		for (i in 0..m - 1) {
			var (x, y, w) = nextInts()
			arrX[i] = x
			arrY[i] = y
			arrW[i] = w
 
			weight[x] = max(weight[x], w)
			weight[y] = max(weight[y], w)
		}
 
		var ok = true
		for (i in 0..m - 1) {
			if (min(weight[arrX[i]], weight[arrY[i]]) != arrW[i]) {
				ok = false
			}
		}
		if (ok) {
			println("YES")
			for (i in 1..n) {
				var w = weight[i]
				print("$w ")
			}
			print('\n')
		} else {
			println("NO")
		}
	}
}
