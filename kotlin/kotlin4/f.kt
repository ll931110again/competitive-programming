import kotlin.math.*
 
fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var (m, n, q) = nextInts()
	var arr = Array(m) {Array(n) {0}}
	var rows: Array<Long> = Array(m){0L}
	var cols: Array<Long> = Array(n){0L}
 
	for (i in 0..m - 1) {
		var r = nextInts()
		for (j in 0..n - 1) {
			arr[i][j] = r[j]
			rows[i] += 1L * r[j]
			cols[j] += 1L * r[j]
		}
	}
 
	for (it in 0..q) {
		var ret = 0L
 
		var ssum = 0L
		for (i in 0..m - 1) {
			ssum += rows[i]
		}
		ssum = (ssum + 1) / 2
 
		var idx = 0
		while (true) {
			ssum -= rows[idx]
			if (ssum <= 0) {
				break
			}
			idx += 1
		}
 
		for (i in 0..m - 1) {
			ret += 1L * rows[i] * abs(i - idx)
		}
 
		ssum = 0L
		for (i in 0..n - 1) {
			ssum += cols[i]
		}
		ssum = (ssum + 1) / 2
 
		idx = 0
		while (true) {
			ssum -= cols[idx]
			if (ssum <= 0) {
				break
			}
			idx += 1
		}
 
		for (i in 0..n - 1) {
			ret += 1L * cols[i] * abs(i - idx)
		}
 
		print("$ret ")
 
		if (it < q) {
			var (x, y, z) = nextInts()
			x -= 1
			y -= 1
			rows[x] += 1L * (z - arr[x][y])
			cols[y] += 1L * (z - arr[x][y])
			arr[x][y] = z			
		}
	}
	print("\n")
}
