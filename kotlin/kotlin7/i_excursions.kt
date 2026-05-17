// Codeforces 1533 (Kotlin Heroes: Episode 7) — I. Excursions
// Submission: https://codeforces.com/contest/1533/submission/121428441

import java.util.ArrayDeque

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

fun main() {
	var (m, n, K) = nextInts()
	var adj = ArrayList<ArrayList<Int>>()
	for (i in 0 until m + n + 1) {
		adj.add(ArrayList<Int>())
	}
	var cost = nextInts()

	for (i in 0 until K) {
		var (x, y) = nextInts()
		y += m
		adj[x].add(y)
		adj[y].add(x)
	}

	var par = IntArray(m + n + 1)
	for (i in 0 until m + n + 1) {
		par[i] = -1
	}

	var res = 0
	for (x in 1 until m + n + 1) {
		if (par[x] == -1) {
			par[x] = -2

			var visited = ArrayList<Int>()
			var hasCycle = false

			var queue = ArrayDeque<Int>()
			queue.push(x)
			visited.add(x)

			while (!queue.isEmpty()) {
				var u = queue.peek()
				queue.pop()

				for (v in adj[u]) {
					if (v != par[u]) {
						if (par[v] == -1) {
							par[v] = u
							visited.add(v)
							queue.push(v)
						} else {
							hasCycle = true
						}
					}
				}
			}

			if (!hasCycle) {
				var cycleCost = -1
				for (u in visited) {
					if (u <= m) {
						if (cycleCost < 0 || cycleCost > cost[u - 1]) {
							cycleCost = cost[u - 1]
						}
					}
				}
				if (cycleCost < 0) {
					cycleCost = 0
				}
				res += cycleCost
			}
		}
	}

	println(res)
}
