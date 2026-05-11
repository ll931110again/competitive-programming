// Codeforces 1488 (Kotlin Heroes: Episode 6) — I. Demonic Invasion
// Submission: https://codeforces.com/contest/1488/submission/313680866

import java.io.*

import java.util.*

class LinkCutTree(val n: Int) {
    data class Node(var parent: Int = -1, var left: Int = -1, var right: Int = -1, var maxFlow: Int = 0)

    private val tree = Array(n) { Node() }

    private fun isRoot(x: Int) = (tree[x].parent == -1 || (tree[tree[x].parent].left != x && tree[tree[x].parent].right != x))

    private fun rotate(x: Int) {
        val p = tree[x].parent
        val g = tree[p].parent
        val isLeft = (tree[p].left == x)

        if (!isRoot(p)) {
            if (tree[g].left == p) tree[g].left = x else tree[g].right = x
        }

        tree[x].parent = g
        tree[p].parent = x

        if (isLeft) {
            tree[p].left = tree[x].right
            if (tree[x].right != -1) tree[tree[x].right].parent = p
            tree[x].right = p
        } else {
            tree[p].right = tree[x].left
            if (tree[x].left != -1) tree[tree[x].left].parent = p
            tree[x].left = p
        }
    }

    private fun splay(x: Int) {
        while (!isRoot(x)) {
            val p = tree[x].parent
            val g = tree[p].parent
            if (!isRoot(p)) rotate(if ((tree[g].left == p) == (tree[p].left == x)) p else x)
            rotate(x)
        }
    }

    fun access(x: Int) {
        splay(x)
        tree[x].right = -1
        var last = x
        while (tree[x].parent != -1) {
            val p = tree[x].parent
            splay(p)
            tree[p].right = x
            tree[x].parent = -1
            last = p
        }
        splay(last)
    }

    fun link(x: Int, y: Int) {
        access(x)
        access(y)
        tree[x].parent = y
    }

    fun cut(x: Int) {
        access(x)
        if (tree[x].left != -1) {
            tree[tree[x].left].parent = -1
            tree[x].left = -1
        }
    }
}

class DinicMaxFlowLCT(val n: Int) {
    data class Edge(var to: Int, var capacity: Int, var flow: Int, var rev: Int)

    private val adj = Array(n) { mutableListOf<Edge>() }
    private lateinit var level: IntArray
    private lateinit var ptr: IntArray
    private val lct = LinkCutTree(n)

    fun addEdge(u: Int, v: Int, capacity: Int) {
        val forward = Edge(v, capacity, 0, adj[v].size)
        val backward = Edge(u, 0, 0, adj[u].size)
        adj[u].add(forward)
        adj[v].add(backward)
        lct.link(u, v)
    }

    private fun bfs(source: Int, sink: Int): Boolean {
        level = IntArray(n) { -1 }
        val queue: Queue<Int> = LinkedList()
        queue.add(source)
        level[source] = 0

        while (queue.isNotEmpty()) {
            val u = queue.poll()
            for (edge in adj[u]) {
                if (level[edge.to] == -1 && edge.flow < edge.capacity) {
                    level[edge.to] = level[u] + 1
                    queue.add(edge.to)
                }
            }
        }
        return level[sink] != -1
    }

    private fun dfs(u: Int, sink: Int, pushed: Int): Int {
        if (u == sink) return pushed
        while (ptr[u] < adj[u].size) {
            val edge = adj[u][ptr[u]]
            if (level[edge.to] == level[u] + 1 && edge.flow < edge.capacity) {
                val flow = dfs(edge.to, sink, minOf(pushed, edge.capacity - edge.flow))
                if (flow > 0) {
                    edge.flow += flow
                    adj[edge.to][edge.rev].flow -= flow
                    return flow
                }
            }
            ptr[u]++
        }
        return 0
    }

    fun maxFlow(source: Int, sink: Int): Int {
        var flow = 0
        while (bfs(source, sink)) {
            ptr = IntArray(n) { 0 }
            while (true) {
                val pushed = dfs(source, sink, Int.MAX_VALUE)
                if (pushed == 0) break
                flow += pushed
            }
        }
        return flow
    }

    fun removeEdge(u: Int, v: Int) {
        lct.cut(u)
    }
}

fun main() {
    var reader = BufferedReader(InputStreamReader(System.`in`));
    fun next() = reader.readLine()!!
    fun nextInt() = next().toInt()
    fun nextInts() = next().split(" ").map { it.toInt() }

    var (n, m, k) = nextInts();
    var adj = ArrayList<ArrayList<Int>>();

    for (i in 0 until n) {
        adj.add(ArrayList<Int>());
    }

    for (i in 0 until m) {
        var (u, v) = nextInts();
        adj[u - 1].add(v - 1);
        adj[v - 1].add(u - 1);
    }

    var dist = ArrayList<Int>();
    for (i in 0 until n) {
        dist.add(-1);
    }

    fun bfs() {
        var queue = ArrayDeque<Int>();
        queue.add(0);
        dist[0] = 0;

        while (queue.isNotEmpty()) {
            var u = queue.removeFirst();
            for (v in adj[u]) {
                if (dist[v] < 0) {
                    dist[v] = 1 + dist[u];
                    queue.add(v);
                }
            }
        }
    }

    bfs();

    var dinitz = DinicMaxFlowLCT(n + 2 + m);
    var source = n;
    var sink = n + 1;
    var numVertices = n + 2;

    for (u in 0 until n) {
        for (v in adj[u]) {
            if (dist[u] != 1 + dist[v]) {
                continue;
            }
            var star = numVertices;
            numVertices++;

            if (dist[u] % 2 == 0) {
                dinitz.addEdge(source, star, 1);
                dinitz.addEdge(star, u, 1);
                dinitz.addEdge(star, v, 1);
            } else {
                dinitz.addEdge(star, sink, 1);
                dinitz.addEdge(u, star, 1);
                dinitz.addEdge(v, star, 1);
            }
        }
    }

    println(minOf(dinitz.maxFlow(source, sink), k));
}
