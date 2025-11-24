fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }
 
fun main () {
	var T = nextInt()
	for (it in 0..T - 1) {
		var n = nextInt()
		var a = nextInts()
 
		var pos = ArrayList<Int>()
		for (i in 0..n - 1) {
			pos.add(i)
		}
		for (i in 0..n - 1) {
			for (j in i + 1..n - 1) {
				if (a[pos[i]] > a[pos[j]]) {
					pos[i] = pos[j].also { pos[j] = pos[i]}
				}
			}
		}
 
		var ret = 1
		var orders = ArrayList<Int>()
		var lastIdx = n - 1 - a[pos[0]]
		var toBeIncluded = n - 1
 
		while (toBeIncluded >= 0 && toBeIncluded > lastIdx) {
			orders.add(pos[toBeIncluded])
			toBeIncluded -= 1
		}
 
		for (i in 0..n - 1) {
			if (i > lastIdx) {
				break
			}
			ret += 1
			if (i + 1 < n) {
				lastIdx -= (a[pos[i + 1]] - 1)
			}
			orders.add(pos[i])
			while (toBeIncluded >= 0 && toBeIncluded > lastIdx && toBeIncluded > i) {
				orders.add(pos[toBeIncluded])
				toBeIncluded -= 1
			}
		}
		for (i in 0..n - 1) {
			orders[i] += 1
		}
		println(orders.joinToString(separator=" "))
	}
}
