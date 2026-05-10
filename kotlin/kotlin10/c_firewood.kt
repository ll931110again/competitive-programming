// Codeforces 1958 (Kotlin Heroes: Episode 10) — C. Firewood
// Submission: https://codeforces.com/contest/1958/submission/260818984

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }

 
fun main() {
	val T = nextInt()
	for (i in 1..T) {
		var (n, k) = nextLongs()
		var nl = 1L;
		for (id in 1..n) {
			nl *= 2L;
		}

		var ret = 0
		while (k > 0) {
			ret += 1;
			nl /= 2;
			if (nl <= k) {
				k -= nl;
			}
		}

		println(ret)
	}
}
