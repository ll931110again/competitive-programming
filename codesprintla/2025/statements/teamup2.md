# Team Up!

**Source:** <https://open.kattis.com/problems/teamup2>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <p>Life of the undercity can be boring sometimes. There are
    <span class="tex2jax_process">$N$</span> children in the
    undercity, and they have decided to play a new game — Team Up!
    Each player is on a team, and each turn, a team is selected
    uniformly at random. Two players, <span class=
    "tex2jax_process">$i$</span> and <span class=
    "tex2jax_process">$j$</span>, can use their team’s turn to
    “team up” on player <span class="tex2jax_process">$k$</span>,
    if and only if <span class="tex2jax_process">$i &lt; k &lt;
    j$</span>. After this, player <span class=
    "tex2jax_process">$k$</span> joins <span class=
    "tex2jax_process">$i$</span> and <span class=
    "tex2jax_process">$j$</span>’s team. A team may choose to pass
    (do nothing) on any given turn.</p>
    <div id="a0000000007" class="figure">
      <center>
        <img src="https://open.kattis.com/problems/teamup2/file/statement/en/img-0001.png"
        alt=
        "\includegraphics[width=0.4\textwidth ]{teamupexample1.png}"
        style="width:40.00%">
        <div class="caption">
          <b>Figure 1</b>: Players 2 and 6 team up on player 5.
        </div>
      </center>
    </div>
    <p>A team with one player remaining is considered eliminated,
    as that team can never make a turn again. When only one team is
    not eliminated, that team is the winner. A team is the
    <em>effective winner</em> if the probability that they will
    win, with perfect play, is <span class=
    "tex2jax_process">$1$</span>.</p>
    <div id="a0000000008" class="figure">
      <center>
        <img src="https://open.kattis.com/problems/teamup2/file/statement/en/img-0002.png"
        alt=
        "\includegraphics[width=0.2\textwidth ]{teamupexample2.png}"
        style="width:20.00%">
        <div class="caption">
          <b>Figure 2</b>: Team 3 has won.
        </div>
      </center>
    </div>
    <div id="a0000000009" class="figure">
      <center>
        <img src="https://open.kattis.com/problems/teamup2/file/statement/en/img-0003.png"
        alt=
        "\includegraphics[width=0.2\textwidth ]{teamupexample3.png}"
        style="width:20.00%">
        <div class="caption">
          <b>Figure 3</b>: Team 3 is the effective winner; no
          amount of random luck or perfect play from opposing teams
          will prevent team 3 from eventually winning.
        </div>
      </center>
    </div>
    <p>Note that some games may not have a winner. In the following
    example, neither team can ever win, and the game ends in a
    draw:</p>
    <div id="a0000000010" class="figure">
      <center>
        <img src="https://open.kattis.com/problems/teamup2/file/statement/en/img-0004.png"
        alt=
        "\includegraphics[width=0.2\textwidth ]{teamupexample4.png}"
        style="width:20.00%">
        <div class="caption">
          <b>Figure 4</b>: Neither team can make any progress.
        </div>
      </center>
    </div>
    <p>The children want to keep track of who is winning. You are
    given <span class="tex2jax_process">$Q$</span> queries, of the
    form:</p>
    <ul class="itemize">
      <li>
        <p><tt class="ttfamily">1 t i</tt>: Player <tt class=
        "ttfamily">i</tt> joins team <tt class="ttfamily">t</tt>.
        This turn may not necessarily be valid by the rules of the
        game (children of the undercity don’t always play fair);
        there are no restrictions on when a player might change
        teams.</p>
      </li>
      <li>
        <p><tt class="ttfamily">2 t</tt>: Output the number of
        <i class="itshape">consecutive turns</i> a team needs to
        have <em>effectively won</em>, assuming the children play
        perfectly and follow all rules, or <span class=
        "tex2jax_process">$-1$</span> if they cannot win, even with
        unlimited consecutive turns.</p>
      </li>
    </ul>
    <p>Here, a team having <span class="tex2jax_process">$k$</span>
    <i class="itshape">consecutive turns</i> means that team is
    selected and gets to make a move for the next <span class=
    "tex2jax_process">$k$</span> consecutive turns. Can you help
    the children keep track of who is winning?</p>
    <h2>Input</h2>
    <p>The first line contains two integers, <span class=
    "tex2jax_process">$N$</span> (<span class="tex2jax_process">$2
    \leq N \leq 3 \cdot 10^5$</span>) and <span class=
    "tex2jax_process">$Q$</span> (<span class="tex2jax_process">$1
    \leq Q \leq 3 \cdot 10^5$</span>).<br>
    The second line contains <span class=
    "tex2jax_process">$N$</span> integers, <span class=
    "tex2jax_process">$a_1, a_2, \dots , a_N$</span>, where
    <span class="tex2jax_process">$1 \leq a_i \leq N$</span>
    denotes the initial team of the <span class=
    "tex2jax_process">$i$</span>-th player.<br>
    Each of the following <span class="tex2jax_process">$Q$</span>
    lines contains one query in one of the following formats:</p>
    <ul class="itemize">
      <li>
        <p><tt class="ttfamily">1 t i:</tt> Here, <span class=
        "tex2jax_process">$t$</span> is a team identifier
        (<span class="tex2jax_process">$1 \le t \le N$</span>) and
        <span class="tex2jax_process">$i$</span> is a player index
        (<span class="tex2jax_process">$1 \leq i \leq
        N$</span>).</p>
      </li>
      <li>
        <p><tt class="ttfamily">2 t:</tt> Here, <span class=
        "tex2jax_process">$t$</span> is a team identifier
        (<span class="tex2jax_process">$1 \le t \le N$</span>).</p>
      </li>
    </ul>
    <p>It is guaranteed that there is at least one query of the
    second type.</p>
    <h2>Output</h2>
    <p>For each query of the form <tt class="ttfamily">2 t</tt>,
    output a single line containing one integer — the number of
    consecutive turns a team needs to have effectively won, or
    <span class="tex2jax_process">$-1$</span> if the team cannot
    win even with unlimited consecutive turns.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>4 5
1 2 1 2
2 1
2 2
1 1 2
2 1
2 2
</pre>
        </td>
        <td>
          <pre>1
1
0
-1
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
          <pre>5 2
1 2 1 2 1
2 1
2 2
</pre>
        </td>
        <td>
          <pre>0
-1
</pre>
        </td>
      </tr>
    </table>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 3</th>
        <th>Sample Output 3</th>
      </tr>
      <tr>
        <td>
          <pre>4 2
1 1 2 2
2 1
2 2
</pre>
        </td>
        <td>
          <pre>-1 
-1
</pre>
        </td>
      </tr>
    </table>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 4</th>
        <th>Sample Output 4</th>
      </tr>
      <tr>
        <td>
          <pre>6 8
2 3 2 1 3 1
2 3
2 1
1 3 3
2 3
2 1
1 1 5
2 3
2 1
</pre>
        </td>
        <td>
          <pre>2
-1
1
-1
-1 
-1
</pre>
        </td>
      </tr>
    </table>
  </div>
