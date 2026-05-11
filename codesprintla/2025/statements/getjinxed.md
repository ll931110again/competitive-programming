# Get Jinxed!

**Source:** <https://open.kattis.com/problems/getjinxed>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <p><i class="itshape">Note: This problem uses an encrypted
    input format to enforce processing queries online. Please pay
    attention to the input specifiction section for more
    details.</i></p>
    <p>It’s just another day in Jinx’s lair. Some of her thousands
    of monkeys (don’t ask) have somehow gotten into her mirror maze
    (also don’t ask). And now—<i class="itshape">now!</i>—she can’t
    find her beloved <i class="itshape">Zapper gun</i> (seriously,
    <b class="bfseries">don’t ask</b>). Could one of the monkeys
    have gotten it? Nah—no chance. Surely. Unless...no, no way.
    Impossible. ....Right? Well, just in case, Jinx wants to know:
    if a monkey really did grab the <i class="itshape">Zapper</i>
    and pulled the trigger inside her maze — who’s getting zapped?
    Can you help her figure it out? (And quickly—before the monkeys
    get any ideas!)</p>
    <p>Jinx’s <i class="itshape">mirror maze</i> is built with the
    following constraints:</p>
    <ul class="itemize">
      <li>
        <p>All monkeys and mirrors are exactly at grid points.</p>
      </li>
      <li>
        <p>All mirrors point exactly 45<span class=
        "tex2jax_process">$^\circ $</span> relative to the grid
        axes; each mirror can be represented as <tt class=
        "ttfamily">/</tt> or <tt class="ttfamily">\</tt> and both
        sides of the mirror are reflective.</p>
      </li>
      <li>
        <p>The <i class="itshape">Zapper</i> can only be fired
        parallel to the grid axes.</p>
      </li>
    </ul>
    <p>You are given an initial state of monkeys and mirrors. You
    are tasked to answer the following types of queries:</p>
    <ul class="itemize">
      <li>
        <p><tt class="ttfamily">1 i x y</tt>: Monkey <span class=
        "tex2jax_process">$i$</span> moves to the coordinates
        <span class="tex2jax_process">$(x,y)$</span>. It is
        guaranteed that no monkey or mirror is currently located at
        <span class="tex2jax_process">$(x,y)$</span> during this
        query.</p>
      </li>
      <li>
        <p><tt class="ttfamily">2 x y c</tt>: The mirror at
        <span class="tex2jax_process">$(x,y)$</span> is toggled,
        based on <span class="tex2jax_process">$c$</span>. If
        <span class="tex2jax_process">$c$</span> equals <tt class=
        "ttfamily">/</tt> or <tt class="ttfamily">\</tt>, a mirror
        corresponding to <span class="tex2jax_process">$c$</span>
        is placed at <span class="tex2jax_process">$(x,y)$</span>,
        replacing any mirror currently at <span class=
        "tex2jax_process">$(x,y)$</span> (if any exists). If
        <span class="tex2jax_process">$c$</span> equals <tt class=
        "ttfamily">.</tt> (a period), the mirror at <span class=
        "tex2jax_process">$(x,y)$</span> is removed without
        replacement. It is guaranteed if <span class=
        "tex2jax_process">$c$</span> equals <tt class=
        "ttfamily">.</tt> that a mirror is present at <span class=
        "tex2jax_process">$(x,y)$</span>. It is guaranteed no
        monkey is present at <span class=
        "tex2jax_process">$(x,y)$</span>, regardless of
        <span class="tex2jax_process">$c$</span>.</p>
      </li>
      <li>
        <p><tt class="ttfamily">3 i d</tt>: Monkey <span class=
        "tex2jax_process">$i$</span> fires the <i class=
        "itshape">Zapper</i>, in a direction determined by
        <span class="tex2jax_process">$d$</span>:</p>
        <ul class="itemize">
          <li>
            <p>If <span class="tex2jax_process">$d$</span> equals
            <tt class="ttfamily">N</tt>, the monkey fires the
            <i class="itshape">Zapper</i> north (towards positive
            <span class="tex2jax_process">$y$</span>).</p>
          </li>
          <li>
            <p>If <span class="tex2jax_process">$d$</span> equals
            <tt class="ttfamily">E</tt>, the monkey fires the
            <i class="itshape">Zapper</i> east (towards positive
            <span class="tex2jax_process">$x$</span>).</p>
          </li>
          <li>
            <p>If <span class="tex2jax_process">$d$</span> equals
            <tt class="ttfamily">S</tt>, the monkey fires the
            <i class="itshape">Zapper</i> south (towards negative
            <span class="tex2jax_process">$y$</span>).</p>
          </li>
          <li>
            <p>If <span class="tex2jax_process">$d$</span> equals
            <tt class="ttfamily">W</tt>, the monkey fires the
            <i class="itshape">Zapper</i> west (towards negative
            <span class="tex2jax_process">$x$</span>).</p>
          </li>
        </ul>
      </li>
    </ul>
    <p>The <i class="itshape">Zapper</i> will reflect off mirrors
    until it hits a monkey or exits the grid. If it would hit a
    monkey, output that monkey’s index for this query. Otherwise,
    output <span class="tex2jax_process">$-1$</span> for this
    query.</p>
    <p>Can you figure out how to save the <i class=
    "itshape">Zapper</i> quickly? <span class=
    "tex2jax_process">$\ldots $</span> Oh, and the monkeys.</p>
    <h2>Input</h2>
    <p>On the first line, three integers, <span class=
    "tex2jax_process">$N$</span>, <span class=
    "tex2jax_process">$M$</span>, <span class=
    "tex2jax_process">$Q$</span> (<span class="tex2jax_process">$1
    \le N, M, Q \le 5 \cdot 10^4$</span>): the number of monkeys,
    the number of initial mirrors and the number of queries.</p>
    <ul class="itemize">
      <li>
        <p><span class="tex2jax_process">$N$</span> lines follow,
        each containing a pair of integers <span class=
        "tex2jax_process">$(x_i,y_i)$</span> (<span class=
        "tex2jax_process">$0 \le x_i,y_i \le 10^6$</span>), the
        location of the <span class="tex2jax_process">$i$</span>-th
        monkey.</p>
      </li>
      <li>
        <p><span class="tex2jax_process">$M$</span> lines follow,
        each of the form <span class="tex2jax_process">$(x_j, y_j,
        c)$</span>. Here, <span class=
        "tex2jax_process">$x_j$</span> and <span class=
        "tex2jax_process">$y_j$</span> are integers (<span class=
        "tex2jax_process">$0 \le x_j, y_j \le 10^6$</span>), the
        position of the mirror, and <span class=
        "tex2jax_process">$c$</span> is either <tt class=
        "ttfamily">/</tt> or <tt class="ttfamily">\</tt>,
        representing the type of the mirror.</p>
      </li>
      <li>
        <p><span class="tex2jax_process">$Q$</span> lines follow,
        in the query format described above, where</p>
        <ul class="itemize">
          <li>
            <p>For all indices <span class=
            "tex2jax_process">$i$</span>, <span class=
            "tex2jax_process">$1 \le i \le N$</span></p>
          </li>
          <li>
            <p>For all coordinates <span class=
            "tex2jax_process">$x$</span> and <span class=
            "tex2jax_process">$y$</span>, <span class=
            "tex2jax_process">$0 \le x,y \le 10^6$</span></p>
          </li>
        </ul>
      </li>
    </ul>
    <p>It is guaranteed that the initial placement of monkeys and
    mirrors contains no duplicate coordinates. It is guaranteed
    that no two monkeys will ever occupy the same square. It is
    guaranteed for all queries of type <tt class="ttfamily">1</tt>,
    the destination is empty. It is guaranteed for all queries of
    type <tt class="ttfamily">2</tt>, the target square does not
    contain a monkey. (One mirror may be replaced by another mirror
    during the second type of query, however).</p>
    <p>To enforce online queries, the <span class=
    "tex2jax_process">$x$</span>-coordinates and <span class=
    "tex2jax_process">$y$</span>-coordinates will be encrypted in
    the following format. Let <span class=
    "tex2jax_process">$s_i$</span> be the sum of the correct
    answers to all queries of type <tt class="ttfamily">3</tt>
    before the <span class="tex2jax_process">$i$</span>-th query,
    modulo the prime <span class="tex2jax_process">$10^6 +
    3$</span>. Then, for any query of type <tt class=
    "ttfamily">1</tt> or <tt class="ttfamily">2</tt>, the
    coordinates <span class="tex2jax_process">$x^\prime _i \equiv
    (x_i - s_i) \mod 10^6 + 3 $</span> and <span class=
    "tex2jax_process">$ y^\prime _i \equiv (y_i - s_i) \mod 10^6 +
    3, 0 \leq x^\prime _i, y^\prime _i &lt; 10^6 + 3 $</span>, will
    be given instead of <span class=
    "tex2jax_process">$x_i,y_i$</span>. If no such queries of type
    <tt class="ttfamily">3</tt> have occured yet, <span class=
    "tex2jax_process">$ s_i $</span> is <span class=
    "tex2jax_process">$ 0 $</span>.</p>
    <h2>Output</h2>
    <p>For each query of type <tt class="ttfamily">3</tt>, output
    the result of that query on its own line. It is guaranteed
    there will be at least one such query.</p>
    <h2>Explanations</h2>
    <p>For your convenience, the unencrypted sample inputs are
    given below. Your code will not be tested on the unencrypted
    inputs.</p>
    <div style="width:288.0pt" class="minipage">
      <p><b class="bfseries">Sample Input 1 (unencrypted):</b></p>
      <pre>
3 1 9
2 1
2 0
0 2
2 2 \
3 1 N
3 2 N
3 3 E
2 2 2 /
3 1 N
3 2 N
2 2 2 \
3 1 N
3 2 N
</pre>
    </div>
    <div style="width:288.0pt" class="minipage">
      <p><b class="bfseries">Sample Input 2 (unencrypted):</b></p>
      <pre>
2 4 17
2 2
3 2
1 1 \
1 3 /
5 1 /
5 3 \
3 1 E
3 1 W
1 1 2 1
3 1 E
3 1 W
1 2 3 3
3 1 E
3 2 W
1 1 1 2
3 2 E
3 2 W
1 1 1 4
3 2 W
2 1 3 \
3 2 W
3 2 E
3 1 S
</pre>
    </div>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>3 1 9
2 1
2 0
0 2
2 2 \
3 1 N
3 2 N
3 3 E
2 1000000 1000000 /
3 1 N
3 2 N
2 1000000 1000000 \
3 1 N
3 2 N
</pre>
        </td>
        <td>
          <pre>3
1
1
-1
1
3
1
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
          <pre>2 4 17
2 2
3 2
1 1 \
1 3 /
5 1 /
5 3 \
3 1 E
3 1 W
1 1 1 0
3 1 E
3 1 W
1 2 0 0
3 1 E
3 2 W
1 1 999998 999999
3 2 E
3 2 W
1 1 999996 999999
3 2 W
2 999994 999996 \
3 2 W
3 2 E
3 1 S
</pre>
        </td>
        <td>
          <pre>2
-1
1
1
2
1
1
1
2
1
-1
2
</pre>
        </td>
      </tr>
    </table>
  </div>
