fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var (n, m) = nextInts()
	var lstart = IntArray(n)
	var rstart = IntArray(n)
 
	for (i in 0..n - 1) {
		var (x, y) = nextInts()
		lstart[i] = x
		rstart[i] = y
	}
 
	var maxK = 2e6.toInt() + 1
	var INF = 1e9.toInt()
 
	var segments = Array(maxK) { ArrayList<Int>() }
	for (i in 0..m - 1) {
		var (x, y) = nextInts()
		segments[x + y].add(y - x)
	}
 
	for (i in 0..maxK - 1) {
		segments[i].sort()
	}
 
	for (i in 0..n - 1) {
		var x = lstart[i]
		var y = rstart[i]
		var len = y - x
 
		var ret = INF
 
		// find the longest segment of length not exceeding len
		var low = 0
		var high = segments[x + y].size - 1
		var pos = -1
 
		while (low <= high) {
			var mid = (low + high) shr 1
			if (segments[x + y][mid] <= len) {
				pos = mid
				low = mid + 1
			} else {
				high = mid - 1
			}
		}
 
		if (pos >= 0) {
			ret = len - segments[x + y][pos]
		}
 
		// find two segments
		pos = -1
		low = 0
		high = segments[x + y - 2].size - 1
		while (low <= high) {
			var mid = (low + high) shr 1
			if (segments[x + y - 2][mid] <= len - 2) {
				pos = mid
				low = mid + 1
			} else {
				high = mid - 1
			}
		}
 
		var pos2 = -1
		low = 0
		high = segments[x + y + 2].size - 1
		while (low <= high) {
			var mid = (low + high) shr 1
			if (segments[x + y + 2][mid] <= len - 2) {
				pos2 = mid
				low = mid + 1
			} else {
				high = mid - 1
			}
		}
 
		// only update answer if both exists
		if (pos >= 0 && pos2 >= 0) {
			ret = minOf(ret, len - minOf(segments[x + y - 2][pos], segments[x + y + 2][pos2]))
		}
 
		if (ret < INF) {
			ret /= 2
		} else {
			ret = -1
		}
 
		print("$ret ")
	}
	print('\n')
}
