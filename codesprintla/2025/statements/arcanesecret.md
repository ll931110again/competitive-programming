# Arcane Secret

**Source:** <https://open.kattis.com/problems/arcanesecret>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <p>Viktor has developed a special formula to try and understand
    the secrets of the Arcane. He has <span class=
    "tex2jax_process">$N$</span> runes where the <span class=
    "tex2jax_process">$i$</span>-th rune has a strength
    <span class="tex2jax_process">$s_i$</span>. He wants to
    identify a core rune to study, but only certain runes can
    qualify.</p>
    <p>He can identify a core rune with the following process:</p>
    <ol class="enumerate">
      <li>
        <p>Divide the runes into <span class=
        "tex2jax_process">$K$</span> groups of equal size.</p>
      </li>
      <li>
        <p>Take the maximum strength rune of every group and add it
        into a set <span class="tex2jax_process">$S$</span>. If
        multiple runes within a group tie for the maximum strength,
        one of them is randomly added into <span class=
        "tex2jax_process">$S$</span>. Note that <span class=
        "tex2jax_process">$S$</span> will have <span class=
        "tex2jax_process">$K$</span> elements.</p>
      </li>
      <li>
        <p>The rune whose strength is the median of <span class=
        "tex2jax_process">$S$</span> is a core rune.</p>
      </li>
    </ol>
    <p>How many different runes can possibly be core runes?</p>
    <h2>Input</h2>
    <p>The first line of input contains <span class=
    "tex2jax_process">$N$</span> (<span class="tex2jax_process">$1
    \leq N \leq 10^5$</span>) and <span class=
    "tex2jax_process">$K$</span> (<span class="tex2jax_process">$1
    \leq K \leq N$</span>) representing the number of total runes
    and the number of groups that will be made. It is guaranteed
    that <span class="tex2jax_process">$K$</span> divides
    <span class="tex2jax_process">$N$</span> and <span class=
    "tex2jax_process">$K$</span> is odd. The second line of input
    contains <span class="tex2jax_process">$N$</span> space
    separated integers <span class="tex2jax_process">$s_1, s_2,
    \ldots , s_N (1 \leq s_i \leq 10^9)$</span> representing the
    strength of the runes.</p>
    <h2>Output</h2>
    <p>Output the number of possible core runes that exist.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>9 3
1 1 2 3 5 8 13 21 34
</pre>
        </td>
        <td>
          <pre>3
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
          <pre>6 3
4 4 4 4 4 4
</pre>
        </td>
        <td>
          <pre>6
</pre>
        </td>
      </tr>
    </table>
  </div>
