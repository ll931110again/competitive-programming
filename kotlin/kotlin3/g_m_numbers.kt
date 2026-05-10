// Codeforces 1297 (Kotlin Heroes: Episode 3) — G. M-numbers
// Submission: https://codeforces.com/contest/1297/submission/313181654

import java.math.BigInteger

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun solve(n: Int, K: Int): BigInteger {
	// If n contains a prime factor greater than 10, return -1
	var tmp = n;
	for (i in 2..9) {
		while (tmp % i == 0) {
			tmp /= i;
		}
	}
	if (tmp > 1) {
		return BigInteger("-1");
	}

	var divisors = ArrayList<Int>();
	for (i in 1..40000) {
		if (n % i == 0) {
			if (i <= n/i) {
				divisors.add(i);
			}
			if (i < n/i) {
				divisors.add(n/i);
			}
		}
	}

	var divisorsIdx = mutableMapOf<Int, Int>();
	for (i in 0..(divisors.size - 1)) {
		divisorsIdx[divisors[i]] = i;
	}

	var divisorSize = divisors.size;
	var dp = ArrayList< ArrayList<Int> >();

	var initial = ArrayList<Int>();
	for (j in (0..divisorSize)) {
		initial.add(0);
	}
	dp.add(initial);

	dp[0][0] = 1;
	var ansLen = 0;
	var total = 0;
	var nIdx = divisorsIdx.getOrElse(n) { -1 };

	for (i in 0..50000) {
		initial = ArrayList<Int>();
		for (j in (0..divisorSize)) {
			initial.add(0);
		}
		dp.add(initial);
		for (j in 0..(divisorSize - 1)) {
			var currentDivisor = divisors[j];
			for (d in 1..9) {
				if (n / d < currentDivisor) {
					break;
				}
				var nextDivisor = currentDivisor * d;
				var nIdx = divisorsIdx.getOrElse(nextDivisor) { -1 };
				
				if (nIdx >= 0) {
					dp[i + 1][nIdx] += dp[i][j];
					if (dp[i + 1][nIdx] > K) {
						dp[i + 1][nIdx] = K + 1;
					}
				}
			}
		}
		total += dp[i + 1][nIdx];
		if (total >= K) {
			ansLen = i + 1;
			break;
		}
	}

	var digits = ArrayList<Int>();
	for (i in 0..(ansLen - 1)) {
		digits.add(0);
	}

	total = 0;
	for (i in 1..(ansLen - 1)) {
		total += dp[i][nIdx];
	}

	var prod = 1;
	for (i in (ansLen - 1) downTo 0) {
		for (d in 1..9) {
			if (n / d < prod || n % (prod * d) > 0) {
				continue;
			}
			nIdx = divisorsIdx.getOrElse(n / (prod * d)) { -1 };
			var amount = dp[i][nIdx];
			if (total + amount >= K) {
				digits[i] = d;
				prod *= d;
				break;
			}

			total += amount;
		}
	}

	var output = BigInteger("0");
	for (i in (ansLen - 1) downTo 0) {
		var ds = BigInteger("0");
		for (x in 0..(digits[i] - 1)) {
			ds += BigInteger("1");
		}
		output = output * BigInteger("10") + ds;
	}

	return output;
}

fun main() {
	var (n, K) = nextInts();
	println(solve(n, K));
}
