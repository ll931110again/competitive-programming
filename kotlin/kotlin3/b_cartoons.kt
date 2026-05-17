// Codeforces 1297 (Kotlin Heroes: Episode 3) — B. Cartoons
// Submission: https://codeforces.com/contest/1297/submission/313149806

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
	var T = nextInt()
	for (it in 0..(T - 1)) {
		var n = nextInt()
		var events = ArrayList<Pair<Int, Int>>()
		for (i in 0..(n - 1)) {
			var item = nextInts()
			events.add(Pair(item[0], 1))
			events.add(Pair(item[1] + 1, -1))
		}

		events.sortWith(compareBy({ it.first }, { it.second }))
		var cnt = 0
		var ans = -1

		var i = 0
		while (i < 2 * n) {
			var j = i
			while ((j < 2 * n) && (events[j].first == events[i].first)) {
				cnt += events[j].second
				j += 1
			}

			if (cnt == 1) {
				ans = events[i].first
				break
			}

			i = j
		}

		println(ans)
	}
}
