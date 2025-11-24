fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
fun nextLongs() = next().split(" ").map { it.toLong() }
 
fun main() {
	var T = nextInt()
	while (T > 0) {
		T -= 1;
		var (n, h, b) = nextInts();
		var row1 = next();
		var row2 = next();
 
		var wolf_count = 0;
		var sheep_pos = 0;
		var left_count = 0;
		var right_count = 0;
 
		for (i in 0..(n - 1)) {
			if (row1[i] == 'W') {
				wolf_count += 1;
			}
			if (row1[i] == 'S') {
				sheep_pos = i;
			}
			if (row2[i] == 'W') {
				wolf_count += 1;
			}
			if (row2[i] == 'S') {
				sheep_pos = i;
			}
		}
		
		for (i in 0..(n - 1)) {
			if (row1[i] == 'W') {
				if (i < sheep_pos) {
					left_count += 1;
				} else {
					right_count += 1;
				}
			}
			if (row2[i] == 'W') {
				if (i < sheep_pos) {
					left_count += 1;
				} else {
					right_count += 1;
				}
			}
		}
 
		// kill all the wolves
		var ret = 1L * wolf_count * h;
 
		// build two fences, and kill the wolves on the smaller side of the fence
		ret = minOf(ret, 1L * minOf(left_count, right_count) * h + 2L * b);
 
		// build three fences to block all the wolves
		ret = minOf(ret, 3L * b);
 
		println(ret)
	}
}
