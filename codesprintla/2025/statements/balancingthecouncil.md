# Balancing the Council

**Source:** <https://open.kattis.com/problems/balancingthecouncil>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <p>The Piltover city council is having an emergency meeting.
    Despite the gravity of the situation, the council’s bureaucracy
    compels them to prioritize balancing the seating over actually
    discussing the situation. In the city town hall, there are
    <span class="tex2jax_process">$N$</span> seats and <span class=
    "tex2jax_process">$M$</span> senators. <span class=
    "tex2jax_process">$N$</span> is guaranteed to be an even
    integer. The <span class="tex2jax_process">$N$</span> seats are
    spaced evenly in a circle, and each senator will occupy exactly
    one unique seat. A seating arrangement is called “balanced” if
    the <i class="itshape">average</i> of the arrangement is
    located at the center of the circle.</p>
    <p>Suppose the <span class="tex2jax_process">$M$</span>
    senators are seated at points <span class=
    "tex2jax_process">$(x_1, y_1), (x_2, y_2), \ldots , (x_M,
    y_M)$</span>. The <i class="itshape">average</i> of the
    arrangement is the point <span class="tex2jax_process">$(A_x,
    A_y)$</span>, where:</p><span class="tex2jax_process">\[ A_x =
    \frac{x_1 + x_2 + \ldots + x_M}{M} \]</span><span class=
    "tex2jax_process">\[ A_y = \frac{y_1 + y_2 + \ldots + y_M}{M}
    \]</span>
    <p>The center of the circle is the unique point equidistant
    from all seats. Given the number of seats and senators, is
    there a balanced arrangement?</p>
    <h2>Input</h2>
    <p>The only line of input contains two space-separated
    integers, <span class="tex2jax_process">$N (4 \le N \le 2 \cdot
    10^9), M (1 \le M \le N)$</span>, where <span class=
    "tex2jax_process">$N$</span> is the number of seats, and
    <span class="tex2jax_process">$M$</span> is the number of
    senators. <span class="tex2jax_process">$N$</span> is
    guaranteed to be an even integer.</p>
    <h2>Output</h2>
    <p>Output <span class="tex2jax_process">$\texttt{Yes}$</span>
    if there exists a balanced arrangement and <span class=
    "tex2jax_process">$\texttt{No}$</span> otherwise.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>6 4
</pre>
        </td>
        <td>
          <pre>Yes
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
          <pre>8 7
</pre>
        </td>
        <td>
          <pre>No
</pre>
        </td>
      </tr>
    </table>
  </div>
