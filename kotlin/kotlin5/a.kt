fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }
 
fun main () {
	var T = nextInt()
	for (i in 0..T - 1) {
		var n = nextInt()
		var a = nextLongs()
		var arr = ArrayList<Long>()
		for (id in 0..n - 1) {
			arr.add(a[id])
		}
		arr.sortDescending()
		var ret = 0L
		for (id in 0..n - 1) {
			var tmp = 1L * (id + 1) * arr[id]
			if (ret < tmp) {
				ret = tmp
			}
		}
		println(ret)
	}
}
