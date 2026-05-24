// Codeforces 2011 (Kotlin Heroes: Episode 11) — E. Rock-Paper-Scissors Bot

const val INF = 1e9.toInt()
const val VARS = "RPS"

fun next() = readLine()!!
fun nextInt() = next().toInt()

fun calc(a: List<Int>): Int {
	if (a[0] != 0) {
		return INF
	}
	val balance =
		a.zipWithNext().fold(0) { s, (x, y) ->
			s + (if (x == y) 1 else 0) - (if ((x + 2) % 3 == y) 1 else 0)
		}
	return a.size + maxOf(balance, 0)
}

fun main() {
	var t = nextInt()
	while (t > 0) {
		t -= 1
		val a = next().map { VARS.indexOf(it) }
		println(
			minOf(
				calc(a),
				calc(listOf(0) + a),
				calc(listOf(0, 2) + a),
			),
		)
	}
}
