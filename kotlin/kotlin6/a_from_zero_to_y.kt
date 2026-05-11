// Codeforces 1488 (Kotlin Heroes: Episode 6) — A. From Zero To Y
// Submission: https://codeforces.com/contest/1488/submission/313465657

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var T = nextInt();
	for (it in 0 until T) {
		var (x, y) = nextInts();
		var ops = 0;

		var tx = x;
		while (y / 10 >= tx) {
			tx *= 10;
		}

		var k = 0;
		while (tx >= x) {
			var newOps = (y - k) / tx;
			ops += newOps;
			k += newOps * tx;
			tx /= 10;			
		}

		ops += (y - k);

		println(ops);
	}
}
