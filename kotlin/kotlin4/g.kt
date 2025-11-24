import kotlin.math.*
 
fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun gcd(x: Int, y: Int): Int {
	if (x == 0) {
		return y
	}
	if (x > y) {
		return gcd(y, x)
	}
	return gcd(y % x, x)
}
 
fun solve(k: Int, n: Int, periods: List<Int>, arr: List<Int>, s1: Int, s2: Int): Array<Int> {
	var collections = mutableSetOf<Int>()
	for (i in 0..k - 1) {
		collections.add(periods[i])
	}
 
	var ans = Array(4){-1}
	var marked = Array(n){0}
	for (d in 1..(arr[s2] - arr[s1])) {
		if ((arr[s2] - arr[s1]) % d != 0) {
			continue
		}
		if (!collections.contains(d)) {
			continue
		}
 
		for (i in 0..n - 1) {
			if (i >= s1 && (arr[i] - arr[s1]) % d == 0) {
				marked[i] = 1
			}
		}
 
		var idx2 = 0
		while (idx2 < n && marked[idx2] == 1) {
			idx2 += 1
		}
 
		if (idx2 >= n) {
			ans[0] = arr[s1]
			ans[1] = d
			ans[2] = 1
			ans[3] = periods[0]
			return ans
		}
 
		var d2 = 0
		for (i in idx2 + 1..n - 1) if (marked[i] == 0) {
			d2 = gcd(d2, arr[i] - arr[idx2])
		}
 
		var ok = -1
		if (d2 == 0) {
			d2 = periods[0]
		}
 
		var i = 1
		while (i * i <= d2) {
			if (d2 % i == 0) {
				if (collections.contains(i)) {
					ok = i
				}
				if (collections.contains(d2/i)) {
					ok = d2/i
				}
			}
			i += 1
		}
 
		if (ok >= 0) {
			ans[0] = arr[s1]
			ans[1] = d
			ans[2] = arr[idx2]
			ans[3] = ok
			return ans
		}
	}
	return ans
}
 
fun main() {
	var (k, n) = nextInts()
	var periods = nextInts()
	var arr = nextInts()
 
	var ans = solve(k, n, periods, arr, 0, 1)
	if (ans[0] < 0 && n > 2) {
		ans = solve(k, n, periods, arr, 0, 2)
	}
	if (ans[0] < 0 && n > 2) {
		ans = solve(k, n, periods, arr, 1, 2)
	}
	// There's always a solution for n = 2
	if (n == 2) {
		ans[0] = arr[0]
		ans[1] = periods[0]
		ans[2] = arr[1]
		ans[3] = periods[0]
	}
 
	if (ans[0] < 0) {
		println("NO")
	} else {
		println("YES")
		var x0 = ans[0]
		var x1 = ans[1]
		var x2 = ans[2]
		var x3 = ans[3]
		println("$x0 $x1")
		println("$x2 $x3")
	}
}
