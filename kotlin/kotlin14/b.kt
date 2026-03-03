import kotlin.math.*;

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var T = nextInt();
	for (it in 0 until T) {
		var (a, b, c, d) = nextInts();
		var numOps = 0;

		// first raise (a, b) to be equal
		var bound = max(a, b);
		if (bound > min(c, d)) {
			bound = min(c, d);
		}

		numOps += max(bound - a, 0);
		numOps += max(bound - b, 0);
		if (a < bound) {
			a = bound;
		}
		if (b < bound) {
			b = bound;
		}

		if (a == b) {
			bound = min(c, d);
			numOps += max(bound - a, 0);
			if (a < bound) {
				a = bound;
				b = bound;
			}
		}

		numOps += max(c - a, 0);
		a = c;

		numOps += max(d - b, 0);
		b = d;

		println(numOps);
	} 
}