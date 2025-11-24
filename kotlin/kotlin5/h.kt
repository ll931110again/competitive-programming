fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }
 
fun main() {
	var (m, n, k) = nextInts()
	var a = nextLongs()
	var b = nextLongs()
	var c = Array(m) { nextLongs() }

	var events = Array(m + n) {
		if ( it < m ) {
			longArrayOf(a[it], 0L, it.toLong())
		} else {
			longArrayOf(b[it - m], 1L, (it - m).toLong())
		}
	}

	events.sortWith(compareBy({ it[0] }))
	var vals = LongArray(m + n) { 0L }

	for (e in 0 until m + n) {
		if (events[e][1] == 0L) {
			var idx = events[e][2].toInt()
			vals[e] = b.indices.filter { b[it] <= events[e][0] }.map { c[idx][it] }.max()!!
		} else {
			var idx = events[e][2].toInt()
			vals[e] = a.indices.filter { a[it] <= events[e][0] }.map { c[it][idx] }.max()!!
		}
	}

	var ans = 1e18.toLong()
	for (bonus in 1 until m + n) {
		vals[bonus] += k.toLong()
		var res = 0L
		var cur = 0L
		var mx = 0L

		var i = 0
		while (true) {
			while (i < m + n && events[i][0] <= cur) {
				mx = Math.max(mx, vals[i])
				i++
			}
			if (i == m + n) {
				break
			}
			var delta = events[i][0] - cur
			var need = (delta + mx - 1) / mx
			res += need
			cur += need * mx
		}
		
		ans = Math.min(ans, res)
		vals[bonus] -= k.toLong()
	}

	println(ans)
}
