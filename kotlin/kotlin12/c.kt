fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var coins = next();
	var n = coins.length;
 
	var types = listOf('G', 'S', 'B');
	var ss = ArrayList<ArrayList<Int>>();
	for (ch in types) {
		var row = ArrayList<Int>();
		var accum = 0;
		row.add(0);
		for (i in 0 until n) {
			if (coins[i] == ch) {
				accum++;
			}
			row.add(accum);
		}
		ss.add(row);
	}
 
	var Q = nextInt();
	for (iq in 0 until Q) {
		var (low, high) = nextInts();
		var amounts = ArrayList<Int>();
		for (i in 0 until 3) {
			amounts.add(ss[i][high] - ss[i][low - 1]);
		}
		amounts.sort();
		println(amounts[2] + amounts[0]);
	}
}