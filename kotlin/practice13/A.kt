import java.util.Scanner

fun main() {
	val scanner = Scanner(System.`in`)
	var t = scanner.nextInt()
	while (t > 0) {
		val n = scanner.nextInt()
		val a = IntArray(n) { scanner.nextInt() }
		val majority = if (a[0] == a[1] || a[0] == a[2]) a[0] else a[1]
		for (i in a.indices) {
			if (a[i] != majority) {
				println(i + 1)
				break
			}
		}
		t--
	}
}
