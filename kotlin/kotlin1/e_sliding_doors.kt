// Codeforces 1170 (Kotlin Heroes: Episode 1) — E. Sliding Doors
// Submission: https://codeforces.com/contest/1170/submission/54808079

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
	val (n, m) = nextInts()
	val a = nextInts()

	var s = ArrayList<Int>()
	var ts = 0
	for (i in 0..n - 1) {
		ts += a[i]
		s.add(ts)
	}

	val Q = nextInt()
	for (i in 1..Q) {
		val q = nextInts()
		var nxt = 0

		for (j in 1..q[0]) {
			var gap = q[j] - 1
			if (j > 1) {
				gap -= q[j - 1]
			}
			var low = nxt
			var high = n - 1
			var pos = nxt - 1
			while (low <= high) {
				val mid = (low + high) / 2
				var mygap = s[mid]
				if (nxt > 0) {
					mygap -= s[nxt - 1]
				}
				if (mygap <= gap) {
					pos = mid
					low = mid + 1
				} else {
					high = mid - 1
				}
			}
			if (pos >= nxt) {
				nxt = pos + 1
				if (nxt >= n) {
					break
				}
			}
		}

		if (nxt < n) {
			var lastgap = m - q[q[0]]
			var mygap = s[n - 1]
			if (nxt > 0) {
				mygap -= s[nxt - 1]
			}
			if (mygap <= lastgap) {
				println("YES")
			} else {
				println("NO")
			}
		} else {
			println("YES")
		}
	}
}
