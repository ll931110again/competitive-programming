// Codeforces 1958 (Kotlin Heroes: Episode 10) — E. Yet Another Permutation Constructive
// Submission: https://codeforces.com/contest/1958/submission/260827297

import java.util.*

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun main() {
	val T = nextInt()
	for (it in 1..T) {
		val (n, k) = nextInts()
		var s = ArrayList<Int>()
		for (i in 1..n) {
			s.add(0)
		}

		var num_steps = 0
		var nl = n
		while (nl > 1) {
			num_steps += 1
			nl = (nl + 1) / 2
		}

		if (num_steps < k) {
			println(-1)
			continue
		} else if (k == 1) {
			for (i in 0..(n - 1)) {
				s[i] = n - i
			}
		} else {
			var min_required = 1
			for (i in 2..k) {
				min_required = min_required * 2
			}
			min_required += 1

			var queue: Queue<Pair<Int, Int>> = LinkedList()
			queue.add(Pair(0, min_required - 1))

			s[0] = n
			s[min_required - 1] = n - 1

			var nxt = n - 2
			while (!queue.isEmpty()) {
				var (low, high) = queue.remove()
				if (low + 1 >= high) {
					continue
				}

				var pos = (low + high) / 2
				s[pos] = nxt
				nxt -= 1

				if (nxt < 0) {
					break
				}

				if (low != high) {
					queue.add(Pair(low, pos))
					queue.add(Pair(pos, high))
				}
			}

			for (i in min_required..(n - 1)) {
				s[i] = nxt
				nxt -= 1
			}
		}

		println(s.joinToString(" "))
	}
}
