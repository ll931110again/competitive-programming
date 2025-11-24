import kotlin.math.*
 
fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var (n, m, k) = nextInts()
	var maxSteps = Array(n + 1){-1}
	var lastTouch = Array(n + 1){0}
	maxSteps[k] = 0
 
	for (i in 1..m) {
		var (x, y) = nextInts()
		if (maxSteps[x] >= 0) {
			maxSteps[x] += (i - 1 - lastTouch[x])
		}
		if (maxSteps[y] >= 0) {
			maxSteps[y] += (i - 1 - lastTouch[y])
		}
		lastTouch[x] = i
		lastTouch[y] = i
 
		var px = maxSteps[x]
		var py = maxSteps[y]
		var qx = maxSteps[x]
		var qy = maxSteps[y]
 
		if (px >= 0) {
			qy = max(maxSteps[y], px + 1)
		}
		if (py >= 0) {
			qx = max(maxSteps[x], py + 1)
		}
		maxSteps[x] = qx
		maxSteps[y] = qy
	}
 
	for (i in 1..n) {
		var ans = maxSteps[i]
		if (ans >= 0) {
			ans = m - (maxSteps[i] + (m - lastTouch[i]))
		}
		print("$ans ")
	}
	print('\n')
}
