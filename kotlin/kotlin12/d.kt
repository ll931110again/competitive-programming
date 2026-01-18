fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun query(pos: Int): String {
	print("? ");
	println(pos);
	System.out.flush();
 
	var s = next();
	return s;
}
 
fun output(pos: Int) {
	print("! ");
	println(pos);
}
 
fun main() {
	var (_n, h) = next().split(" ");
	var n = _n.toInt();
 
	var initial = query(1);
	var isLowerBefore = if ('a' <= initial[0] && initial[0] <= 'z') true else false;
 
	var low = 1;
	var high = n;
	var ans = -1;
	while (low <= high) {
		var mid = (low + high) / 2;
		var s = query(mid);
 
		if (s == h) {
			ans = mid;
			break;
		} else {
			var isLess = true;
			if (('a' <= s[0] && s[0] <= 'z') == ('a' <= h[0] && h[0] <= 'z')) {
				isLess = s < h;
			} else {
				if (isLowerBefore) {
					isLess = ('a' <= s[0] && s[0] <= 'a');
				} else {
					isLess = ('A' <= s[0] && s[0] <= 'Z');
				}
			}
 
			if (isLess) {
				low = mid + 1;
			} else {
				high = mid - 1;
			}
		}
	}
 
	output(ans);
}