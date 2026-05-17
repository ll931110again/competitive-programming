// Codeforces 1297 (Kotlin Heroes: Episode 3) — E. Modernization of Treeland
// Submission: https://codeforces.com/contest/1297/submission/313196484

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun solve() {
	var (n, K) = nextInts()
	var adj = ArrayList<ArrayList<Int>>()
	for (i in 0 until n) {
		adj.add(ArrayList<Int>())
	}

	for (i in 0 until (n - 1)) {
		var (x, y) = nextInts()
		x--
		y--
		adj[x].add(y)
		adj[y].add(x)
	}

	var vis = ArrayList<Boolean>()
	var deg = ArrayList<Int>()
	var xc = 0

	for (i in 0 until n) {
		vis.add(false)
		deg.add(0)
	}

	var answerFound = false

	fun checkAnswer(): Boolean {
		if (answerFound) {
			return true
		}

		var xk = xc
		if (deg[0] <= 1) {
			xk++
		}

		if (xk == K) {
			answerFound = true

			println("Yes")
			var cnt = 0
			for (i in 0 until n) {
				if (vis[i]) {
					cnt++
				}
			}
			println(cnt)
			for (i in 0 until n) {
				if (vis[i]) {
					print(i + 1)
					print(" ")
				}
			}
			println()
			return true
		}

		return false
	}

	if (K == 1) {
		println("Yes")
		println(1)
		println(1)
		return
	}

	for (i in 0 until n) {
		vis.add(false)
		deg.add(0)
	}

	fun bfs() {
		var queue = ArrayDeque<Int>()
		queue.add(0)
		vis[0] = true

		while (queue.isNotEmpty()) {
			var u = queue.removeFirst()
			for (v in adj[u]) {
				if (v != u && !vis[v]) {
					vis[v] = true
					deg[u]++
					deg[v]++

					if (u != 0) {
						if (deg[u] == 1) {
							xc++
						} else if (deg[u] == 2) {
							xc--
						}
					}

					if (v != 0) {
						if (deg[v] == 1) {
							xc++
						} else if (deg[v] == 2) {
							xc--
						}
					}

					if (checkAnswer()) {
						break
					}

					queue.add(v)
				}
			}
		}
	}

	bfs()

	if (!answerFound) {
		println("No")
	}
}

fun main() {
	var T = nextInt()
	for (it in 0 until T) {
		solve()
	}
}
