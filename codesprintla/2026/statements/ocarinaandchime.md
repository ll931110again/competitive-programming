# Problem H — Ocarina and Chime

**Source:** <https://codesprintla26.kattis.com/contests/codesprintla26open/problems/ocarinaandchime>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <div style="width:30.00%" class="illustration">
      <img src=
      "https://codesprintla26.kattis.com/problems/ocarinaandchime/file/statement/en/img-0001.jpg"
      alt=
      "https://codesprintla26.kattis.com/problems/ocarinaandchime/file/statement/en/img-0001.jpg"
      class="illustration">
      <div class="description">
        The Ocarina of Time
      </div>
    </div>
    <p>Deep within a forgotten temple, Link encounters a long
    corridor lined with <span class="tex2jax_process">$M$</span>
    magical chimes in a row. To unlock the final door, he must play
    a sacred melody consisting of <span class=
    "tex2jax_process">$N$</span> notes on his ocarina (<span class=
    "tex2jax_process">$M \ge N$</span>). There are <span class=
    "tex2jax_process">$k$</span> pitches, represented by integers
    ranging from <span class="tex2jax_process">$0$</span> to
    <span class="tex2jax_process">$k - 1$</span>.</p>
    <ul class="itemize">
      <li>
        <p>The corridor’s chimes have pitches <span class=
        "tex2jax_process">$a_1, a_2, \dots , a_M$</span> in
        order.</p>
      </li>
      <li>
        <p>The sacred melody has pitches <span class=
        "tex2jax_process">$b_1, b_2, \dots , b_N$</span> which Link
        plays in order.</p>
      </li>
    </ul>
    <p>Link must choose a specific starting chime, <span class=
    "tex2jax_process">$i$</span>, to begin playing the sacred
    melody at, where <span class="tex2jax_process">$1 \le i \le M -
    N + 1$</span>. Standing at the <span class=
    "tex2jax_process">$i$</span>-th chime with pitch <span class=
    "tex2jax_process">$a_i$</span>, he plays the 1st note of his
    melody <span class="tex2jax_process">$b_1$</span>. Moving to
    the <span class="tex2jax_process">$(i+1)$</span>-th chime with
    pitch <span class="tex2jax_process">$a_{i+1}$</span>, he plays
    the 2nd note of his melody <span class=
    "tex2jax_process">$b_2$</span>. He continues this process until
    he reaches the <span class=
    "tex2jax_process">$(i+N-1)$</span>-th chime with pitch
    <span class="tex2jax_process">$a_{i+N-1}$</span>, where he
    plays the <span class="tex2jax_process">$N$</span>-th (final)
    note <span class="tex2jax_process">$b_N$</span>. When Link
    plays the <span class="tex2jax_process">$j$</span>-th note of
    his melody next to the corresponding chime, the two pitches
    combine to create a “magical” pitch, calculated as <span class=
    "tex2jax_process">$a_{i+j-1} + b_j \pmod{k}$</span> for
    <span class="tex2jax_process">$1\le j\le N$</span>. Thus
    starting at position <span class="tex2jax_process">$i$</span>,
    the resulting magical pitches are <span class=
    "tex2jax_process">$a_i+b_1,a_{i+1}+b_2,\dots ,a_{i+N-1}+b_N
    \pmod{k}$</span>. A starting position <span class=
    "tex2jax_process">$i$</span> is considered harmonious if in the
    resulting <span class="tex2jax_process">$N$</span> magical
    pitches, every pitch from <span class=
    "tex2jax_process">$0$</span> to <span class=
    "tex2jax_process">$k - 1$</span> appears an equal number of
    times. Your task is to help Link find all harmonious starting
    positions to unlock the shrine door.</p>
    <h2>Input</h2>
    <p>The first line contains three space-separated integers
    <span class="tex2jax_process">$M$</span>, <span class=
    "tex2jax_process">$N$</span>, and <span class=
    "tex2jax_process">$k$</span> <span class="tex2jax_process">$(1
    \le N \le M \le 5 \cdot 10^5, 2 \le k \le 100)$</span>, the
    number of chimes, the length of the sacred melody, and the
    number of pitches respectively.</p>
    <p>The second line contains <span class=
    "tex2jax_process">$M$</span> space-separated integers
    <span class="tex2jax_process">$a_1, \ldots , a_M$</span>
    <span class="tex2jax_process">$(0 \le a_i &lt; k)$</span>, the
    pitches of the corridor’s chimes in order.</p>
    <p>The third line contains <span class=
    "tex2jax_process">$N$</span> space-separated integers
    <span class="tex2jax_process">$b_1, \ldots , b_N$</span>
    <span class="tex2jax_process">$(0 \le b_i &lt; k)$</span>, the
    pitches of the sacred melody in order.</p>
    <h2>Output</h2>
    <p>The first line of output should be an integer <span class=
    "tex2jax_process">$l$</span>, representing the exact number of
    harmonious starting positions.</p>
    <p>The second line of output should consist of <span class=
    "tex2jax_process">$l$</span> space-separated integers, one for
    each harmonious starting position, sorted in ascending order.
    If there are no harmonious starting positions, this line should
    be left blank.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>6 4 2
1 0 1 0 1 0
1 1 1 1
</pre>
        </td>
        <td>
          <pre>3
1 2 3
</pre>
        </td>
      </tr>
    </table>
  </div>
