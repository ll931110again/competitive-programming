# Hextech Ordnance

**Source:** <https://open.kattis.com/problems/hextechordnance>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <p>Heimerdinger is visiting the prestigious Simons Institute,
    the world’s leading research institute on the theory of
    computing — and more recently, hextech. He is helping the
    researchers examine the strange properties of <i class=
    "itshape">hextech ordnance</i>. A sample of <i class=
    "itshape">hextech ordnance</i> gives <span class=
    "tex2jax_process">$n$</span> readings, <span class=
    "tex2jax_process">$a_1, a_2, \ldots , a_n$</span>. Heimerdinger
    realizes that this ordnance is stable if and only if for all
    <span class="tex2jax_process">$1 \leq k \leq n$</span>, there
    exists at least <span class="tex2jax_process">$k$</span>
    subarrays such that the difference between the the maximum and
    minimum elements of the subarray is at most <span class=
    "tex2jax_process">$k$</span>, and the length of the subarray is
    at least 2. Formally, the ordnance is stable if for all
    <span class="tex2jax_process">$1 \leq k \leq n$</span>, there
    exists at least <span class="tex2jax_process">$k$</span>
    subarrays <span class="tex2jax_process">$[i, j]$</span> such
    that:</p>
    <ul class="itemize">
      <li>
        <p><span class="tex2jax_process">$1 \leq i &lt; j \leq
        n$</span>,</p>
      </li>
      <li>
        <p><span class="tex2jax_process">$\max (a_i, a_{i+1},
        \ldots , a_j) - \min (a_i, a_{i+1}, \ldots , a_j) \leq
        k$</span>.</p>
      </li>
    </ul>
    <p>Heimerdinger doesn’t quite know how to check the readings
    himself, and has already assigned all of his students the
    essential task of researching mustache volumizers. As such, he
    asks you—a young rising researcher at Simons Institute—for your
    help.</p>
    <h2>Input</h2>
    <p>The first line of input contains a single integer
    <span class="tex2jax_process">$n$</span> (<span class=
    "tex2jax_process">$1 \leq n \leq 10^6$</span>), the number of
    readings. The second line contains <span class=
    "tex2jax_process">$n$</span> space-seperated integers
    <span class="tex2jax_process">$a_1, a_2, \ldots , a_n$</span>
    (<span class="tex2jax_process">$0 \leq a_i \leq 10^9$</span>),
    the readings from the <i class="itshape">hexcore</i>.</p>
    <h2>Output</h2>
    <p>Output <tt class="ttfamily">stable</tt> if the ordnance is
    stable. Otherwise, output <tt class=
    "ttfamily">unstable</tt>.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>10
10 3 8 2 6 5 3 4 4 10
</pre>
        </td>
        <td>
          <pre>stable
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
          <pre>10
3 15 19 7 15 18 17 4 19 1
</pre>
        </td>
        <td>
          <pre>unstable
</pre>
        </td>
      </tr>
    </table>
  </div>
