// Codeforces 1958 (Kotlin Heroes: Episode 10) — D. Staircase
// Submission: https://codeforces.com/contest/1958/submission/260820805

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun main() {
	val T = nextInt()
	for (it in 1..T) {
		val n = nextInt()
		val a = nextLongs()

		var ret = 0L
		var i = 0
		while (i < n) {
			while (i < n && a[i] == 0L) {
				i += 1
			}
			if (i >= n) {
				break
			}

			var j = i + 1
			while (j < n && a[j] > 0L) {
				j += 1
			}

			var pos = -1
			if ((j - i) % 2 > 0) {
				var k = i
				while (k < j) {
					if (pos < 0 || a[pos] < a[k]) {
						pos = k
					}
					k += 2
				}
			}

			for (kt in i..(j - 1)) {
				if (kt != pos) {
					ret += 2L * a[kt]
				} else {
					ret += a[kt]
				}
			}

			i = j
		}
		println(ret)
	}
}
