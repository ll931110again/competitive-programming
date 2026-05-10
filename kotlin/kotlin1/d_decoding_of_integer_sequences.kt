// Codeforces 1170 (Kotlin Heroes: Episode 1) — D. Decoding of Integer Sequences
// Submission: https://codeforces.com/contest/1170/submission/54758555

fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
	val n = nextInt()
	val a = nextInts()
	var pcount = 0
	for (i in 0..n - 1) {
		if (a[i] == -1) {
			pcount += 1
		}
	}

	var ret = ArrayList<ArrayList<Int>>()
	for (i in 0..pcount - 1) {
		var x = ArrayList<Int>()
		ret.add(x)
	}

	var exists = ArrayList<Int>()
	var marked = ArrayList<Boolean>()
	for (i in 0..pcount - 1) {
		exists.add(i)
		marked.add(false)
	}

	var pos = 0
	for (i in 0..n - 1) {
		val u = exists[pos]
		if (a[i] >= 0) {
			ret[u].add(a[i])
		} else {
			marked[pos] = true
		}
		pos += 1
		if (pos >= exists.size) {
			var newexists = ArrayList<Int>()
			var newmarked = ArrayList<Boolean>()
			for (j in 0..exists.size - 1) {
				if (!marked[j]) {
					newexists.add(exists[j])
					newmarked.add(false)
				}
			}
			exists = newexists
			marked = newmarked
			pos = 0
		}
	}

	println(pcount)
	for (i in 0..pcount - 1) {
		print("${ret[i].size} ")
		println(ret[i].joinToString(prefix = "", postfix = "", separator=" "))
	}
}
