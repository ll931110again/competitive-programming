# Problem I — Robbie's Research Q-uest

**Source:** <https://codesprintla26.kattis.com/contests/codesprintla26open/problems/robbiesresearchquest>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <p>Robbie is trying to decipher a massive, ancient Zonai
    circuitry map found deep in the Depths. The current schematic
    forms a tree with <span class="tex2jax_process">$N$</span>
    vertices, the vertices representing terminals and the edges
    representing wires. However, Robbie believes he can upgrade the
    schematic. To power his latest Purah Pad upgrade, he’s looking
    for an elusive ‘Q’ structure, which can concentrate the energy
    of the circuit to supply the power he needs.</p>
    <p>Robbie decides to experiment with the schematic. He wants to
    connect two vertices in the tree together with a edge, and then
    strip away unnecessary vertices until the remaining circuit is
    a “Q graph”. Informally, a Q graph is a graph that looks like a
    ‘Q’. More formally, a Q graph consists of:</p>
    <ul class="itemize">
      <li>
        <p>A single cycle of at least 3 vertices, where the degree
        of each vertex in the cycle is exactly 2.</p>
      </li>
      <li>
        <p>A single vertex in the cycle, the “tail vertex”; the
        tail vertex has at least one and at most two additional
        paths connected to it such that each path is a
        non-branching chain (the degree of all vertices in the
        paths is 2, except for the ends of the path, which have
        degree 1).</p>
      </li>
    </ul>
    <p>Examples of Q graphs are shown below:</p>
    <div id="a0000000005" class="figure">
      <center>
        <img src=
        "https://codesprintla26.kattis.com/problems/robbiesresearchquest/file/statement/en/img-0001.jpg"
        alt="\includegraphics[width=0.45\textwidth ]{qgraphs.jpg}"
        style="width:45.00%">
        <div class="caption">
          <b>Figure 1</b>: Various Q graphs
        </div>
      </center>
    </div>
    <div id="a0000000006" class="figure">
      <center>
        <img src=
        "https://codesprintla26.kattis.com/problems/robbiesresearchquest/file/statement/en/img-0002.jpg"
        alt=
        "\includegraphics[width=0.33\textwidth ]{notqgraphs.jpg}"
        style="width:33.00%">
        <div class="caption">
          <b>Figure 2</b>: Various graphs that are <i class=
          "itshape">not</i> Q graphs
        </div>
      </center>
    </div>
    <p>Since Robbie needs to draw as much energy as possible, he
    wants the Q graph to have as many vertices as possible. Can you
    help him optimize his schematic by determining for a given pair
    of vertices to connect, the greatest number of vertices the
    remaining Q graph can have?</p>
    <h2>Input</h2>
    <p>The first line contains two space-separated integers
    <span class="tex2jax_process">$N$</span> and <span class=
    "tex2jax_process">$Q$</span> (<span class="tex2jax_process">$3
    \le N \le 2.5 \cdot 10^5$</span>, <span class=
    "tex2jax_process">$1 \le Q \le 2.5 \cdot 10^5$</span>), the
    number of vertices and the number of queries.</p>
    <p>The next <span class="tex2jax_process">$N - 1$</span> lines
    contain two space-separated integers <span class=
    "tex2jax_process">$u$</span> and <span class=
    "tex2jax_process">$v$</span> (<span class="tex2jax_process">$1
    \le u,v \le N$</span>, <span class="tex2jax_process">$u \neq
    v$</span>), the edges of the tree. It is guaranteed that these
    edges form a tree.</p>
    <p>The next <span class="tex2jax_process">$Q$</span> lines
    contain two space-separated integers <span class=
    "tex2jax_process">$a_i$</span> and <span class=
    "tex2jax_process">$b_i$</span> (<span class=
    "tex2jax_process">$1 \le a_i, b_i \le N$</span>, <span class=
    "tex2jax_process">$a_i \neq b_i$</span>), the vertices to
    connect with an edge in a query. It is guaranteed that
    <span class="tex2jax_process">$(a_i,b_i)$</span> is not already
    an edge in the tree.</p>
    <p>Each query should be processed independently of all
    others.</p>
    <h2>Output</h2>
    <p>Output <span class="tex2jax_process">$Q$</span> integers on
    their own line, where the <span class=
    "tex2jax_process">$i$</span>-th line contains the output for
    the <span class="tex2jax_process">$i$</span>-th query, the
    maximum size of a Q graph that can be created by removing
    vertices from the original tree if the edge connecting
    <span class="tex2jax_process">$(a_i, b_i)$</span> is added. If
    no Q graph can be obtained, output <span class=
    "tex2jax_process">$-1$</span>.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>4 2
1 2
2 3
3 4
1 4
1 3
</pre>
        </td>
        <td>
          <pre>-1
4
</pre>
        </td>
      </tr>
    </table>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 2</th>
        <th>Sample Output 2</th>
      </tr>
      <tr>
        <td>
          <pre>6 4
2 6
1 5
1 3
5 2
6 4
2 3
3 5
5 6
2 3
</pre>
        </td>
        <td>
          <pre>6
6
5
6
</pre>
        </td>
      </tr>
    </table>
  </div>
