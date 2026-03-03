fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var T = nextInt();
	for (it in 0 until T) {
		var k = nextInt();
		var (a1, b1) = nextInts();
		var (a2, b2) = nextInts();
		var a3 = 0;
		var b3 = k;

		var sumA = a1 + a2 + a3;
		var sumB = b1 + b2 + b3;
		if (sumA < sumB) {
			println("YES");
		} else if (sumA > sumB) {
			println("NO");
		} else {
			var delta = 0;
			if (a1 > b1) {
				delta++;
			} else {
				delta--;
			}
			if (a2 > b2) {
				delta++;
			} else {
				delta--;
			}
			if (a3 > b3) {
				delta++;
			} else {
				delta--;
			}
			if (delta > 0) {
				println("NO");
			} else {
				println("YES");
			}
		}
	} 
}