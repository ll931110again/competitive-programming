// Codeforces 1488 (Kotlin Heroes: Episode 6) — G. Painting Numbers
// Submission: https://codeforces.com/contest/1488/submission/313532520

import java.util.PriorityQueue

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun main() {
	var n = nextInt()
	var reds = ArrayList<Boolean>()
	var values = ArrayList<Int>()

	for (i in 0 until (n + 1)) {
		reds.add(false)
		values.add(0)
	}

	for (i in 1 until (n + 1)) {
		var j = 2 * i
		while (j <= n) {
			values[j]++
			j += i
		}
	}

	var pq = PriorityQueue<Pair<Int, Int>>(compareByDescending { it.first })
	for (i in 1 until (n + 1)) {
		pq.add(Pair(values[i], i))
	}

	var ans = 0
	for (it in 1 until (n + 1)) {
		while (true) {
			var tmp = pq.poll()
			var u = tmp.second

			if (reds[u]) {
				continue
			}

			if (tmp.first != values[u]) {
				pq.add(Pair(values[u], u))
				continue
			}

			reds[u] = true
			ans += values[u]
			print(ans)
			if (it < n) {
				print(" ")
			} else {
				println()
			}

			var i = 1
			while (i * i <= u) {
				if (u % i == 0) {
					values[i]--
					if (i < u / i) {
						values[u / i]--
					}
				}
				i++
			}

			break
		}
	}
}
