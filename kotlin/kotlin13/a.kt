fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var T = nextInt();
	for (it in 0 until T) {
		var n = nextInt();
		var a = nextInts();
		var used = ArrayList<Int>();
		for (i in 0 until n) {
			used.add(0);
		}
 
		for (i in 0 until n) {
			var j = 0;
			while (j < n && a[j] > a[i]) {
				j += 1;
			}
			used[j] = 1;
		}
 
		var cnt = 0;
		for (i in 0 until n) {
			if (used[i] == 0) {
				cnt += 1;
			}
		}
 
		println(cnt);
		for (i in 0 until n) {
			if (used[i] == 0) {
				print(i + 1);
				print(" ");
			}
		}
		println()
	} 
}