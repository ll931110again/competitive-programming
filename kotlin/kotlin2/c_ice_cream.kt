// Codeforces 1211 (Kotlin Heroes: Episode 2) — C. Ice Cream
// Submission: https://codeforces.com/contest/1211/submission/60225599

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun solve() {
	var (n, K) = nextInts()
	var a = ArrayList<Int>()
	var b = ArrayList<Int>()
	var c = ArrayList<Int>()

	for (i in 0..n - 1) {
		var (na, nb, nc) = nextInts()
		a.add(na)
		b.add(nb - na)
		c.add(nc)
	}

	var ret = 0L
	for (i in 0..n - 1) {
		K -= a[i]
		ret += 1L * a[i] * c[i]

		if (K < 0) {
			println(-1)
			return
		}
	}

	var sk = 0L
	for (i in 0..n - 1) {
		sk += b[i]
		if (sk >= K) {
			break
		}
	}

	if (sk < K) {
		println(-1)
		return
	}

	var idx = ArrayList<ArrayList<Int>>()
	for (i in 0..n - 1) {
		val x = ArrayList<Int>()
		x.add(c[i])
		x.add(i)
		idx.add(x)
	}

	idx.sortBy { it.get(0) }
	for (i in 0..n - 1) {
		var pos = idx[i][1]
		var rem = K
		if (rem > b[pos]) {
			rem = b[pos]
		}

		ret += 1L * rem * c[pos]
		K -= rem
	}

	println(ret)
}

fun main() {
	solve()
}
