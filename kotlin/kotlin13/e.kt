fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLong() = next().toLong()
fun nextLongs() = next().split(" ").map { it.toLong() }
 
fun main() {
	var T = nextInt();
	var mod = 998244353;
	for (it in 0 until T) {
		var s = next();
		var total = 1;
		for (i in 0 until s.length) if (s[i] == '?') {
			total = total * 2 % mod;
		}
 
		var bad = 1;
		if (s[0] == '1') {
			bad = 0;
		}
		for (i in 1 until s.length) {
			if (s[i] == '0') {
				bad = 0;
			}
		}
		total -= bad;
		if (total < 0) {
			total += mod;
		}
 
		println(total)
	} 
}