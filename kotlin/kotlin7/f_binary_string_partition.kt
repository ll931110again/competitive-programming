// Codeforces 1533 (Kotlin Heroes: Episode 7) — F. Binary String Partition
// Submission: https://codeforces.com/contest/1533/submission/121346343

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun main () {
	var s = next()
	var n = s.length

	var p0 = IntArray(n + 1)
	var p1 = IntArray(n + 1)
	p0[0] = 0
	p1[0] = 0
	for (i in 1..n) {
		if (s[i - 1] == '0') {
			p0[i] = p0[i - 1] + 1
			p1[i] = p1[i - 1]
		} else {
			p0[i] = p0[i - 1]
			p1[i] = p1[i - 1] + 1
		}
	}

	for (k in 1..n) {
		var pos = 0
		var ans = 0

		while (pos < n) {
			ans += 1
			var low = pos + 1
			var high = n
			var newpos = pos + k

			while (low <= high) {
				var mid = (low + high) / 2
				if (p0[mid] - p0[pos] <= k || p1[mid] - p1[pos] <= k) {
					newpos = mid
					low = mid + 1
				} else {
					high = mid - 1
				}
			}

			pos = newpos
		}

		print(ans)
		print(' ')
	}
	println()
}
