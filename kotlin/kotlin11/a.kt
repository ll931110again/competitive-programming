fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var T = nextInt()
	while (T > 0) {
		T -= 1;
		var n = nextInt();
		var a = nextInts();
 
		var max_below = 0;
		var value = a[n - 1];
		for (i in 0..(n - 2)) {
			if (max_below < a[i]) {
				max_below = a[i];
			}
		}
 
		if (max_below == value - 1) {
			println(max_below)
		} else {
			println("Ambiguous")
		}
	}
}
