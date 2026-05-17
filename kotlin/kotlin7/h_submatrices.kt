// Codeforces 1533 (Kotlin Heroes: Episode 7) — H. Submatrices
// Submission: https://codeforces.com/contest/1533/submission/121360335

import java.util.ArrayDeque

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun main() {
	var (m, n) = nextInts()
	var arr = Array<String>(m) { "" }
	for (i in 0 until m) {
		arr[i] = next()
	}

	var K = 5
	var TK = 32
	var cnt = LongArray(TK)
	var res = LongArray(K)

	for (mask in 1 until TK) {
		var height = IntArray(n)
		for (i in 0 until m) {
			var lpos = IntArray(n)
			var rpos = IntArray(n)
			var stack = ArrayDeque<Int>()

			for (j in 0 until n) {
				var c = arr[i][j].toInt() - 65
				if ((mask and (1 shl c)) > 0) {
					height[j] += 1
				} else {
					height[j] = 0
				}
			}

			for (j in 0 until n) {
				while (!stack.isEmpty() && height[stack.peek()] > height[j]) {
					stack.pop()
				}
				if (stack.isEmpty()) {
					lpos[j] = 0
				} else {
					lpos[j] = 1 + stack.peek()
				}
				stack.push(j)
			}

			while (!stack.isEmpty()) {
				stack.pop()
			}

			for (j in n - 1 downTo 0) {
				while (!stack.isEmpty() && height[stack.peek()] >= height[j]) {
					stack.pop()
				}
				if (stack.isEmpty()) {
					rpos[j] = n - 1
				} else {
					rpos[j] = stack.peek() - 1
				}
				stack.push(j)
			}

			for (j in 0 until n) {
				cnt[mask] += 1L * height[j] * (rpos[j] - j + 1) * (j - lpos[j] + 1)
			}
		}

		for (sub in 1 until mask) {
			if ((mask and sub) == sub) {
				cnt[mask] -= cnt[sub]
			}
		}

		var pc = 0
		for (k in 0 until K) {
			if ((mask and (1 shl k)) > 0) {
				pc++
			}
		}

		res[pc - 1] += cnt[mask]
	}

	println(res.joinToString(" "))
}
