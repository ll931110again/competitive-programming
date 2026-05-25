// Codeforces 1488 (Kotlin Heroes: Episode 6) — H. Build From Suffixes
// DP idea: dp[i][c] = ways for suffix i starting with letter c; transition matches
// constraint a[i] and defers equal-char comparisons to a[i+1] (user's update loop).

import java.io.BufferedInputStream
import java.io.InputStream

const val MOD = 998244353
const val B = 4
const val MSZ = B * B

private fun modAdd(
	a: Int,
	b: Int,
): Int {
	val s = a + b
	return if (s >= MOD) s - MOD else s
}

/** M[j][k] = 1 if first char j at i, k at i+1 is allowed for edge constraint [aCur, aNext]. */
fun buildEdgeMatrix(
	aCur: Int,
	aNext: Int,
	isLast: Boolean,
): IntArray {
	val m = IntArray(MSZ)
	for (j in 0 until B) {
		for (k in 0 until B) {
			val ok =
				if (aCur == 1) {
					k > j || (j == k && !isLast && aNext == 1)
				} else {
					k < j || (j == k && (isLast || aNext == 0))
				}
			if (ok) m[j * B + k] = 1
		}
	}
	return m
}

private fun mul4(
	a: IntArray,
	b: IntArray,
	out: IntArray,
) {
	for (i in 0 until B) {
		val row = i * B
		for (j in 0 until B) {
			var s = 0L
			for (k in 0 until B) {
				s += a[row + k].toLong() * b[k * B + j]
			}
			out[row + j] = (s % MOD).toInt()
		}
	}
}

class SegTree(
	nn: Int,
	private val n: Int,
	private val a: IntArray,
) {
	private val size: Int
	private val tree: Array<IntArray>
	private val tmp = IntArray(MSZ)

	init {
		var s = 1
		while (s < nn) s *= 2
		size = s
		tree = Array(2 * s) { IntArray(MSZ) }
		for (i in 0 until s) {
			for (j in 0 until B) tree[size + i][j * B + j] = 1
		}
		for (i in 1..nn) setLeaf(i)
		for (i in size - 1 downTo 1) pull(i)
	}

	private fun leafMatrix(pos: Int): IntArray {
		val isLast = pos == n - 1
		val aNext = if (isLast) 0 else a[pos + 1]
		return buildEdgeMatrix(a[pos], aNext, isLast)
	}

	private fun setLeaf(pos: Int) {
		copy(leafMatrix(pos), tree[size + pos - 1])
	}

	private fun copy(
		src: IntArray,
		dst: IntArray,
	) {
		System.arraycopy(src, 0, dst, 0, MSZ)
	}

	private fun pull(v: Int) {
		mul4(tree[v * 2 + 1], tree[v * 2], tmp)
		copy(tmp, tree[v])
	}

	fun flip(pos: Int) {
		a[pos] = 1 - a[pos]
		setLeaf(pos)
		var v = size + pos - 1
		while (v > 1) {
			v /= 2
			pull(v)
		}
		if (pos > 1) {
			setLeaf(pos - 1)
			v = size + pos - 2
			while (v > 1) {
				v /= 2
				pull(v)
			}
		}
	}

	fun answer(): Int {
		val mat = tree[1]
		var ans = 0
		for (i in 0 until B) {
			var s = 0
			val row = i * B
			for (j in 0 until B) {
				s = modAdd(s, mat[row + j])
			}
			ans = modAdd(ans, s)
		}
		return ans
	}
}

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
	val q = fs.nextInt()
	val a = IntArray(n + 1)
	for (i in 1 until n) a[i] = fs.nextInt()

	val st = SegTree(n - 1, n, a)
	val out = StringBuilder()
	for (qi in 0 until q) {
		st.flip(fs.nextInt())
		out.append(st.answer()).append('\n')
	}
	print(out)
}
