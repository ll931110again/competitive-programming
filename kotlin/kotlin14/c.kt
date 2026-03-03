/* Solution for problem C: Minesweeper.
 *
 *
	Note there's a special case of k = 1 with the following pattern that needs to
	be handled separately:
		*
		.
 	The below section handles only the normal cases.

 	The constraint each free cell must be adjacent to at most one mine shows that
 	we can decompose the grid into disjoint blocks of mine and its adjacent free cells.
 	We can quickly see that there are only two patterns possible:
		.*.         and.    *.
		...                 ..
	where the later can only appear at most twice (in the left and right corner).

	The first pattern contributes 5 free cells each, while the second pattern contributes
	3 cells each. Hence the only valid k are of the form 5m, 5m + 3, and 5m + 6 where m >= 0.

 */

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