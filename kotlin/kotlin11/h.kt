fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
var par = IntArray(105);
 
fun root(u: Int): Int {
	if (u != par[u]) {
		par[u] = root(par[u]);
	}
	return par[u];
}
 
fun join(u: Int, v: Int) {
	var xu = root(u);
	var xv = root(v);
	if (xu != xv) {
		par[xv] = xu;
	}
}
 
fun solve(a: ArrayList<ArrayList<Int>>, m: Int, n: Int, k: Int): Int {
	for (u in 0..(2 * m - 1)) {
		par[u] = u;
	}
 
	for (i in 0..(m - 1)) {
		for (j in (i + 1)..(m - 1)) {
			for (xj in 0..1) {
				var zero_count = 0;
				var one_count = 0;
 
				for (t in 0..(n - 1)) {
					var i_val = a[i][t];
					var j_val = a[j][t].xor(xj);
					if (i_val == 0 && j_val == 0) {
						zero_count += 1;
					} else if (i_val == 1 && j_val == 1) {
						one_count += 1;
					}
				}
 
				if (zero_count >= k || one_count >= k) {
					join(2 * i, 2 * j + xj.xor(1));
					join(2 * i + 1, 2 * j + xj);
				}
			}
		}
	}
 
	for (u in 0..(2 * m - 1)) {
		if (root(u) == root(u.xor(1))) {
			return -1;
		}
	}
 
	var visited = BooleanArray(2 * m);
	var cmpIndex = IntArray(2 * m);
 
	var components = ArrayList<ArrayList<Int>>();
	var cmpCount = 0;
	for (u in 0..(2 * m - 1)) {
		if (visited[u]) {
			continue;
		}
		visited[u] = true;
 
		var row = ArrayList<Int>();
		row.add(u);
 
		for (v in 0..(2 * m - 1)) {
			if (!visited[v] && root(v) == root(u)) {
				row.add(v);
			}
		}
 
		components.add(row);
		for (x in row) {
			cmpIndex[x] = cmpCount;
		}
 
		cmpCount += 1;
	}
 
	for (u in 0..(2 * m - 1)) {
		visited[u] = false;
	}
 
	var ans = 0;
	for (u in 0..(2 * m - 1)) {
		if (!visited[u]) {
			var cmp = cmpIndex[u];
			var cmp_dual = cmpIndex[u.xor(1)];
			if (cmp == cmp_dual) {
				return -1;
			}
 
			var cmp_count = 0;
			var cmp_dual_count = 0;
			for (x in components[cmp]) {
				visited[x] = true;
				if (x % 2 > 0) {
					cmp_count += 1;
				}
			}
			for (x in components[cmp_dual]) {
				visited[x] = true;
				if (x % 2 > 0) {
					cmp_dual_count += 1;
				}
			}
 
			ans += minOf(cmp_count, cmp_dual_count);
		}
	}
 
	return ans;
}
 
fun main() {
	var T = nextInt()
	while (T > 0) {
		T -= 1;
		var (m, n, k) = nextInts();
 
		var a = ArrayList< ArrayList<Int> >();
		for (i in 0..(m - 1)) {
			var items = nextInts();
			var row = ArrayList<Int>();
			for (x in items) {
				row.add(x);
			}
			a.add(row);
		}
 
		var mult = 16;
		var ret = 0;
		while (mult > 0) {
			// Compose the matrix based on the corresponding mask
			var masked = ArrayList< ArrayList<Int> >();
			for (i in 0..(m - 1)) {
				var row = ArrayList<Int>();
				for (j in 0..(n - 1)) {
					if (a[i][j].and(mult) > 0) {
						row.add(1);
					} else {
						row.add(0);
					}
				}
				masked.add(row);
			}
 
			var ans = solve(masked, m, n, k);
			if (ans < 0) {
				ret = -1;
				break;
			} else {
				ret += ans * mult;
				mult /= 2;
			}
		}
 
		println(ret);
	}
}
