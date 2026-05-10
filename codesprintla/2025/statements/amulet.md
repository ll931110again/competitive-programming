# The Amulet

**Source:** <https://open.kattis.com/problems/amulet>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <p>Caitlyn is on another case. Her biggest clue this time is a
    small amulet containing an <span class="tex2jax_process">$R
    \times C$</span> grid of characters. From an informant, she
    learned that the amulet has a secret code. She’s not sure what
    the code is yet, but knows it satisfies the following
    properties:</p>
    <ul class="itemize">
      <li>
        <p>The code appears as a substring in exactly <span class=
        "tex2jax_process">$x$</span> rows, and does not appear in
        any other rows.</p>
      </li>
      <li>
        <p>The code appears as a substring in exactly <span class=
        "tex2jax_process">$y$</span> columns, and does not appear
        in any other columns.</p>
      </li>
      <li>
        <p>The code is maximal length of all strings that satisfy
        the first two conditions.</p>
      </li>
      <li>
        <p>The code is lexicographically smallest of all strings
        that satisfy the first three conditions.</p>
      </li>
      <li>
        <p>The code has length greater than zero.</p>
      </li>
    </ul>
    <p>A string appears as a substring in a row if it appears as a
    contiguous sequence of characters in that row when read from
    left to right. A string appears as a substring in a column if
    it appears as a contiguous sequence of characters in that
    column when read from top to bottom. For two strings of equal
    length, <span class="tex2jax_process">$S$</span> and
    <span class="tex2jax_process">$T$</span>, <span class=
    "tex2jax_process">$S$</span> is lexicographically smaller than
    <span class="tex2jax_process">$T$</span> if there exists an
    index <span class="tex2jax_process">$k$</span> such that for
    all <span class="tex2jax_process">$i &lt; k$</span>,
    <span class="tex2jax_process">$S_i = T_i$</span>, and
    <span class="tex2jax_process">$S_k$</span> precedes
    <span class="tex2jax_process">$T_k$</span> in alphabetical
    order.</p>
    <p>Caitlyn knows <span class="tex2jax_process">$x$</span> and
    <span class="tex2jax_process">$y$</span>, but she still can’t
    spot the code, and she’s starting to suspect that the informant
    was just wasting her time. Can you create an algorithm to help
    Caitlyn crack the code?</p>
    <h2>Input</h2>
    <p>The first line contains four integers <span class=
    "tex2jax_process">$R$</span>, <span class=
    "tex2jax_process">$C$</span>, <span class=
    "tex2jax_process">$x$</span>, and <span class=
    "tex2jax_process">$y$</span> (<span class="tex2jax_process">$1
    \leq R \times C \leq 2 \cdot 10^5$</span>, <span class=
    "tex2jax_process">$0 \leq x \leq R$</span>, <span class=
    "tex2jax_process">$0 \leq y \leq C$</span>, <span class=
    "tex2jax_process">$x + y &gt; 0$</span>). The next <span class=
    "tex2jax_process">$R$</span> lines each contain a string of
    length <span class="tex2jax_process">$C$</span> consisting of
    lowercase letters <tt class="ttfamily">a-z</tt>, representing
    the amulet.</p>
    <h2>Output</h2>
    <p>Output a single line containing the secret code as described
    above. If no such code exists, output <tt class=
    "ttfamily">-1</tt> instead.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>3 3 2 2
xba
bab
acc
</pre>
        </td>
        <td>
          <pre>ba
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
          <pre>3 3 2 2
aba
bab
acc
</pre>
        </td>
        <td>
          <pre>ab
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
          <pre>3 3 2 2
aba
bab
abc
</pre>
        </td>
        <td>
          <pre>ba
</pre>
        </td>
      </tr>
    </table>
  </div>
