fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }
 
fun main() {
	var T = nextInt();
	while (T > 0) {
		T -= 1;
		var s = next();
		var n = s.length;
		var dp = LongArray(n + 1);
 
		dp[n] = 0;
		for (i in 1..(n - 1)) {
			dp[i] = -100000000000000000L;
		}
 
		for (i in 0..(n - 1)) {
			if (s[i] == '+') {
				continue;
			}
 
			var left_operand = 0L;
			var right_operand = 0L;
			var found_plus_op = false;
 
			for (j in i..(n - 1)) {
				if (s[j] == '+') {
					if (found_plus_op) {
						break;
					}
					found_plus_op = true;
				} else {
					var digit = s[j] - '0';
					if (found_plus_op) {
						right_operand = right_operand * 10L + 1L * digit;
					} else {
						left_operand = left_operand * 10L + 1L * digit;
					}
				}
 
				if (found_plus_op && left_operand > 0 && right_operand > 0) {
					dp[j + 1] = maxOf(dp[j + 1], dp[i] + left_operand + right_operand);
				}
			}
		}
 
		println(dp[n])
	}
}
