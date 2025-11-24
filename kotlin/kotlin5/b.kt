fun next() = readLine()!!
fun nextInt() = next().toInt()
 
fun main () {
	var T = nextInt()
	for (it in 0..T - 1) {
		var s = next()
		var n = s.length
 
		var ret = 0
		var cnt = 0
		for (i in 0..n - 1) {
			if (s.get(i) == 'v') {
				cnt += 1
				if (cnt % 2 == 0) {
					ret += 1
				}
			} else {
				ret += 1
				cnt = 0
			}
		}
 
		println(ret)
	}
}
