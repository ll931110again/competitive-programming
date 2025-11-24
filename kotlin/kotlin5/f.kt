import java.util.*
import kotlin.math.abs
 
fun next() = readLine()!!
fun nextInt() = next().toInt()
fun nextInts() = next().split(" ").map { it.toInt() }

fun main() {
    var (n, k, x) = nextInts()
    var a = nextInts()

    var low = 0L
    var high = 1e10.toLong()
    var ans = high

    fun can(mid : Long): Boolean {
        var cursum = 0L
        var need = 0

        var cur = PriorityQueue<Int>(compareBy<Int> { -it })
        for (i in 0 until n) {
            cursum += a[i]
            cur.add(a[i])
            while (cur.size > x || cursum > mid) {
                cursum -= cur.first()
                cur.remove()
                need += 1
            }
            if (cur.size == x) {
                cur.clear()
                cursum = 0L
            }
        }

        return need <= k
    }

    while (low <= high) {
        var mid = (low + high) / 2L
        if (can(mid)) {
            ans = mid
            high = mid - 1
        } else {
            low = mid + 1
        }
    }

    println(ans)
}
