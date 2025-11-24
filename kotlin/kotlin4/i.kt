import kotlin.math.*
 
fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }
 
fun main() {
	var (n, m, q, s) = nextInts()
	s -= 1
 
	var initial = nextInts()
	var adj = Array(n){ArrayList<Int>()}
	var d = Array(n){Array(n){n + 3}}
 
	for (i in 0..m - 1) {
		var (u, v) = nextInts()
		u -= 1
		v -= 1
		adj[u].add(v)
		d[u][v] = 1
	}
 
	for (k in 0..n - 1) {
		for (u in 0..n - 1) {
			for (v in 0..n - 1) {
				d[u][v] = minOf(d[u][v], d[u][k] + d[k][v])
			}
		}
	}
 
	var INF = 1e18.toLong()
	var dp = Array(n) {Array(n) {Array(60) {INF}}}
	var cost = Array(n) {Array(n * n){-1L}}
 
	for (u in 0..n - 1) {
		var minPath = Array(n) {Array(1 shl n){INF}}
		minPath[u][1 shl u] = 0L
 
		for (mask in 0..(1 shl n) - 1) {
			for (a in 0..n - 1) if (minPath[a][mask] < INF) {
				for (b in 0..n - 1) {
					var tb = mask or (1 shl b)
					minPath[b][tb] = minOf(minPath[b][tb], minPath[a][mask] + d[a][b])
				}
			}
		}
 
		for (v in 0..n - 1) {
			dp[u][v][0] = minPath[v][(1 shl n) - 1]
		}
 
		for (mask in 0..(1 shl n) - 1) {
			var tval = 0L
			for (i in 0..n - 1) if ((mask and (1 shl i)) > 0) {
				tval += 1L * initial[i]
			}
			for (v in 0..n - 1) if (minPath[v][mask] < INF) {
				var path = minPath[v][mask].toInt()
				cost[u][path] = maxOf(cost[u][path], tval)
			}
		}
	}
 
	for (k in 1 until 60) {
		for (u in 0..n - 1) {
			for (v in 0..n - 1) {
				for (t in 0..n - 1) {
					dp[u][v][k] = minOf(dp[u][v][k], dp[u][t][k - 1] + dp[t][v][k - 1])
				}
			}
		}
	}
 
	var queries = nextLongs()
	var P = 0L
	for (i in 0..n - 1) {
		P += 1L * initial[i]
	}
 
	for (it in 0..q - 1) {
		var query = queries[it]
		var cyc = query / P
		var rem = query % P
 
		var prev = Array(n){INF}
		prev[s] = 0L
		for (i in 0..60 - 1) if ((cyc and (1L shl i)) > 0) {
			var nxt = Array(n){INF}
			for (u in 0..n - 1) {
				for (v in 0..n - 1) {
					nxt[v] = minOf(nxt[v], prev[u] + dp[u][v][i])
				}
			}
			for (u in 0..n - 1) {
				prev[u] = nxt[u]
			}
		}
 
		var ret = INF
		for (v in 0..n - 1) if (prev[v] < INF) {
			for (i in 0..n * n - 1) if (cost[v][i] >= rem) {
				ret = minOf(ret, prev[v] + i)
			}
		}
		println(ret)
	}
}
