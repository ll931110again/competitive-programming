fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var n = nextInt();
	var numOps = 0;
	for (i in 0 until n) {
		if (i % 2 == 0) {
			numOps += (n - i) * 2 + 1;
		} else {
			numOps += (n - i) * 2;
		}
	}
 
	println(numOps);
	for (i in 0 until n) {
		if (i % 2 == 0) {
			for (j in i until n) {
				println("pushback a[$j]");
				println("min");
			}
			println("popfront");
		} else {
			for (j in (n - 1) downTo i) {
				println("min");
				println("popback");
			}
		}
	}
}