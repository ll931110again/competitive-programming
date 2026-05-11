# Problem J — Seize the Means of Construct Production!

**Source:** <https://codesprintla26.kattis.com/contests/codesprintla26open/problems/seizethemeans>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <p>Link is trying to build constructs in the Construct Factory
    to help Mineru. Within this factory, there is an assembly line
    of length <span class="tex2jax_process">$N$</span>. This
    assembly line builds constructs. A construct is represented as
    a string consisting of digits from <span class=
    "tex2jax_process">$0$</span> to <span class=
    "tex2jax_process">$4$</span>. At each position from 1 to
    <span class="tex2jax_process">$N$</span>, the assembly line
    will add a single digit <span class=
    "tex2jax_process">$0$</span>, <span class=
    "tex2jax_process">$1$</span>, <span class=
    "tex2jax_process">$2$</span>, <span class=
    "tex2jax_process">$3$</span>, or <span class=
    "tex2jax_process">$4$</span> to a construct.</p>
    <p>On this assembly line, we have <span class=
    "tex2jax_process">$M$</span> restrictions on construct
    production. The <span class="tex2jax_process">$i$</span>-th
    restriction means that assembly line positions between
    <span class="tex2jax_process">$C_i$</span> and <span class=
    "tex2jax_process">$D_i$</span> cannot add a certain digit
    <span class="tex2jax_process">$B_i$</span> directly to the
    right of another digit <span class=
    "tex2jax_process">$A_i$</span>. More explicitly, we prohibit
    for all <span class="tex2jax_process">$C_i \leq k &lt;
    D_i$</span> that position <span class=
    "tex2jax_process">$k$</span> on the line produces <span class=
    "tex2jax_process">$A_i$</span> and position <span class=
    "tex2jax_process">$k+1$</span> produces <span class=
    "tex2jax_process">$B_i$</span>.</p>
    <p>Additionally, we have <span class=
    "tex2jax_process">$P$</span> checkpoints that his constructs
    may need to satisfy. The <span class=
    "tex2jax_process">$j$</span>-th checkpoint is at assembly line
    position <span class="tex2jax_process">$E_j$</span> and is
    satisfied if the assembly line produces a certain character
    <span class="tex2jax_process">$F_j$</span> at that
    position.</p>
    <p>We then have <span class="tex2jax_process">$Q$</span>
    queries.</p>
    <ul class="itemize">
      <li>
        <p>Queries of type <span class="tex2jax_process">$0$</span>
        are specified by <span class="tex2jax_process">$G$</span>,
        <span class="tex2jax_process">$H$</span>, and <span class=
        "tex2jax_process">$K$</span>. Count the number of distinct
        constructs that satisfy at least <span class=
        "tex2jax_process">$K \: (0 \leq K \leq 4)$</span>
        checkpoints that can be constructed when only considering
        the part of the assembly line between the <span class=
        "tex2jax_process">$G$</span>th and <span class=
        "tex2jax_process">$H$</span>th checkpoints (inclusive on
        both ends). Since this number may be large, output this
        modulo <span class="tex2jax_process">$10^9 + 7$</span>.</p>
      </li>
      <li>
        <p>Queries of type <span class="tex2jax_process">$1$</span>
        are specified by <span class="tex2jax_process">$L$</span>.
        This toggles the <span class="tex2jax_process">$L$</span>th
        checkpoint (if it’s on, it becomes off and vice versa). If
        a checkpoint is off, it can never be satisfied. All
        checkpoints start in the on position.</p>
      </li>
    </ul>
    <h2>Input</h2>
    <p>The first line contains four space-separated integers
    <span class="tex2jax_process">$N$</span>, <span class=
    "tex2jax_process">$M$</span>, <span class=
    "tex2jax_process">$P$</span>, and <span class=
    "tex2jax_process">$Q$</span>, <span class="tex2jax_process">$(3
    \leq N \leq 10^6, 0 \leq M \leq 1\, 000, 2 \leq P \leq \min (N,
    10^4), 1 \leq Q \leq 1\, 000)$</span>, the length of the
    assembly line, the number of restrictions, the number of
    checkpoints, and the number of queries respectively.</p>
    <p>The next <span class="tex2jax_process">$M$</span> lines
    represent the <span class="tex2jax_process">$M$</span>
    restrictions. Each line contains four space-separated integers
    <span class="tex2jax_process">$C$</span>, <span class=
    "tex2jax_process">$D$</span>, <span class=
    "tex2jax_process">$A$</span>, and <span class=
    "tex2jax_process">$B$</span> <span class="tex2jax_process">$(1
    \leq C &lt; D \leq N, 0 \leq A, B \leq 4)$</span>. This
    represents that assembly line positions between <span class=
    "tex2jax_process">$C$</span> and <span class=
    "tex2jax_process">$D$</span> cannot add a certain digit
    <span class="tex2jax_process">$B$</span> directly to the right
    of another digit <span class="tex2jax_process">$A$</span>. The
    indices <span class="tex2jax_process">$C$</span> and
    <span class="tex2jax_process">$D$</span> represent absolute
    positions on the assembly line, not checkpoint indices.</p>
    <p>The next <span class="tex2jax_process">$P$</span> lines
    represent the <span class="tex2jax_process">$P$</span>
    checkpoints. Each line contains two space-separated integers
    <span class="tex2jax_process">$E$</span> and <span class=
    "tex2jax_process">$F$</span> <span class="tex2jax_process">$(1
    \leq E \leq N, 0 \leq F \leq 4)$</span>. This represents that
    there is a checkpoint at assembly line position <span class=
    "tex2jax_process">$E$</span> and is satisfied if the assembly
    line produces a certain character <span class=
    "tex2jax_process">$F$</span> at that position. It is guaranteed
    that these checkpoints will be in sorted order by increasing
    <span class="tex2jax_process">$E$</span> and all <span class=
    "tex2jax_process">$E$</span>’s will be distinct.</p>
    <p>The next <span class="tex2jax_process">$Q$</span> lines
    represent the <span class="tex2jax_process">$Q$</span> queries.
    Each line will start with a <span class=
    "tex2jax_process">$0$</span> or a <span class=
    "tex2jax_process">$1$</span>. All queries use checkpoint
    indices, and checkpoints are indexed by the order in which they
    appear in input.</p>
    <ul class="itemize">
      <li>
        <p>If a line starts with a <span class=
        "tex2jax_process">$0$</span>, it will contain three
        additional space-separated integers <span class=
        "tex2jax_process">$G$</span>, <span class=
        "tex2jax_process">$H$</span>, and <span class=
        "tex2jax_process">$K$</span> <span class=
        "tex2jax_process">$(1 \leq G &lt; H \leq P, 0 \leq K \leq
        4)$</span>. This represents that the <span class=
        "tex2jax_process">$G$</span>th and <span class=
        "tex2jax_process">$H$</span>th checkpoints are the start
        and end points (inclusive) of this query, and that we need
        to satisfy at least <span class=
        "tex2jax_process">$K$</span> checkpoints in this query.</p>
      </li>
      <li>
        <p>If a line starts with a <span class=
        "tex2jax_process">$1$</span>, it will contain a single
        additional integer <span class="tex2jax_process">$L \: (1
        \leq L \leq P)$</span>. This represents that we toggle the
        <span class="tex2jax_process">$L$</span>th checkpoint
        (turning it on if it is off and vice versa).</p>
      </li>
    </ul>
    <p>It is guaranteed that there will be at least one query of
    type <span class="tex2jax_process">$0$</span>.</p>
    <h2>Output</h2>
    <p>Output a single line per each query of type <span class=
    "tex2jax_process">$0$</span>. For each such query, output a
    single integer, the number of distinct constructs that can be
    produced by the assembly line following all applicable
    constraints, modulo <span class="tex2jax_process">$10^9 +
    7$</span>.</p>
    <div id="a0000000004" class="figure">
      <center>
        <img src=
        "https://codesprintla26.kattis.com/problems/seizethemeans/file/statement/en/img-0001.jpg"
        alt="\includegraphics[width=0.6\textwidth ]{drawing.jpg}"
        style="width:60.00%">
        <div class="caption">
          <b>Figure 1</b>: Sample testcase 1 before any updates.
          233 is a valid construct and passes one checkpoint. 201
          is invalid as 1 is placed after 0.
        </div>
      </center>
    </div>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>3 1 2 7
1 3 0 1
1 2
3 4
0 1 2 2
0 1 2 1
0 1 2 0
1 1
0 1 2 2
0 1 2 1
0 1 2 0
</pre>
        </td>
        <td>
          <pre>5
43
115
0
24
115
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
          <pre>6 3 6 8
1 6 0 1
2 5 3 4
3 4 2 3
1 0
2 1
3 2
4 3
5 4
6 0
0 1 2 1
0 1 2 2
0 3 4 0
0 3 4 2
1 4
0 3 4 1
1 4
0 3 4 1
</pre>
        </td>
        <td>
          <pre>8
0
22
0
4
8
</pre>
        </td>
      </tr>
    </table>
  </div>
