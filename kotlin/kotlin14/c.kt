import kotlin.math.*;

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var T = nextInt();
	for (it in 0 until T) {
		var k = nextInt();
		if (k == 1) {
			println("YES");
			println(1);
			println("*");
			println(".");
			continue;
		}

		var row1 = "";
		var row2 = "";
		if (k % 5 == 1 || k % 5 == 3) {
			row1 += "*.";
			row2 += "..";
			k -= 3;
		}
		while (k >= 5) {
			row1 += ".*.";
			row2 += "...";
			k -= 5;
		}
		if (k == 3) {
			row1 += ".*";
			row2 += "..";
			k -= 3;
		}
		if (k > 0) {
			println("NO");
		} else {
			println("YES");
			println(row1.length);
			println(row1);
			println(row2);
		}
	}
}