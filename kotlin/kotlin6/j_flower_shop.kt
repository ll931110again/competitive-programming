// Codeforces 1488 (Kotlin Heroes: Episode 6) — J. Flower Shop

import java.io.BufferedInputStream
import java.io.InputStream
import java.util.Arrays

const val MOD = 998244353
const val MAXK = 5000

private class FastScanner(
	stream: InputStream,
) {
	private val input = BufferedInputStream(stream)
	private val buffer = ByteArray(1 shl 16)
	private var len = 0
	private var ptr = 0

	private fun readByte(): Int {
		if (ptr >= len) {
			len = input.read(buffer)
			ptr = 0
			if (len <= 0) return -1
		}
		return buffer[ptr++].toInt()
	}

	fun nextInt(): Int {
		var c = readByte()
		while (c <= 32) {
			if (c < 0) return 0
			c = readByte()
		}
		var neg = false
		if (c == '-'.code) {
			neg = true
			c = readByte()
		}
		var x = c - '0'.code
		while (true) {
			c = readByte()
			if (c <= 32) break
			x = x * 10 + c - '0'.code
		}
		return if (neg) -x else x
	}
}

fun main() {
	val fs = FastScanner(System.`in`)
	val n = fs.nextInt()
	val m = fs.nextInt()
	val w = IntArray(n) { fs.nextInt() }
	val cnt = IntArray(n)
	val dp = IntArray(MAXK + 1)
	val ndp = IntArray(MAXK + 1)
	val out = StringBuilder()

	for (qi in 0 until m) {
		when (fs.nextInt()) {
			1 -> {
				val i = fs.nextInt() - 1
				cnt[i] += fs.nextInt()
			}

			2 -> {
				val i = fs.nextInt() - 1
				cnt[i] -= fs.nextInt()
			}

			else -> {
				val l = fs.nextInt() - 1
				val r = fs.nextInt()
				val k = fs.nextInt()

				var totalCost = 0
				for (idx in l until r) totalCost += cnt[idx] * w[idx]
				val limit = if (totalCost < k) totalCost else k

				var cur = dp
				var nxt = ndp
				cur[0] = 1
				if (limit > 0) Arrays.fill(cur, 1, limit + 1, 0)

				for (idx in l until r) {
					var c = cnt[idx]
					if (c == 0) continue
					val wi = w[idx]
					if (wi > k) continue
					val maxTake = k / wi
					if (c > maxTake) c = maxTake

					when {
						wi == 1 -> {
							var sum = 0
							for (s in 0..limit) {
								sum += cur[s]
								if (sum >= MOD) sum -= MOD
								if (s > c) {
									sum -= cur[s - c - 1]
									if (sum < 0) sum += MOD
								}
								nxt[s] = sum
							}
						}

						c == 1 -> {
							for (s in wi..limit) {
								var v = cur[s] + cur[s - wi]
								if (v >= MOD) v -= MOD
								nxt[s] = v
							}
							for (s in 0 until wi) nxt[s] = cur[s]
						}

						else -> {
							System.arraycopy(cur, 0, nxt, 0, limit + 1)
							for (t in 1..c) {
								val cost = t * wi
								if (cost > limit) break
								for (s in limit downTo cost) {
									var v = nxt[s] + cur[s - cost]
									if (v >= MOD) v -= MOD
									nxt[s] = v
								}
							}
						}
					}
					val tmp = cur
					cur = nxt
					nxt = tmp
				}

				var ans = 0
				for (s in 1..limit) {
					ans += cur[s]
					if (ans >= MOD) ans -= MOD
				}
				out.append(ans).append('\n')
			}
		}
	}
	print(out)
}
