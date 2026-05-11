# Problem B — Guardian Laser

**Source:** <https://codesprintla26.kattis.com/contests/codesprintla26open/problems/guardianlaser>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <div style="width:40.00%" class="illustration">
      <img src=
      "https://codesprintla26.kattis.com/problems/guardianlaser/file/statement/en/img-0001.jpg" alt=
      "https://codesprintla26.kattis.com/problems/guardianlaser/file/statement/en/img-0001.jpg"
      class="illustration">
      <div class="description">
        Example Reflecting Laser
      </div>
    </div>
    <p>Link and a guardian are standing in a triangle on the
    coordinate plane whose vertices are <span class=
    "tex2jax_process">$O=(0, 0), A=(1, 0), B=(0, 1)$</span>. The
    guardian is located at point <span class=
    "tex2jax_process">$O$</span>.</p>
    <p>The guardian faces in the direction of the line with
    equation <span class="tex2jax_process">$ax-by=0$</span>, where
    <span class="tex2jax_process">$a$</span> and <span class=
    "tex2jax_process">$b$</span> are positive integers. Then, the
    guardian fires a laser which reflects off the triangle’s walls.
    If the laser hits a corner of the triangle, it stops.</p>
    <p>Since the path of the laser can be very unpredictable, Link
    has decided to position himself at only point <span class=
    "tex2jax_process">$A$</span> or point <span class=
    "tex2jax_process">$B$</span>. Can you help Link choose one of
    these points so that he does not get hit by the laser? It can
    be shown that at least one of these points will be safe.</p>
    <h2>Input</h2>
    <p>Each test contains multiple test cases. The first line
    contains a single integer <span class=
    "tex2jax_process">$t$</span> (<span class="tex2jax_process">$1
    \leq t \leq 10^4$</span>), the number of test cases. The
    description of the test cases follows.</p>
    <p>The first and only line of each test case contains two
    space-separated integers <span class=
    "tex2jax_process">$a$</span> and <span class=
    "tex2jax_process">$b$</span> (<span class="tex2jax_process">$1
    \leq a, b \leq 10^9$</span>), the coefficients of the line that
    the guardian shoots in the direction of.</p>
    <h2>Output</h2>
    <p>For each testcase, print a single character on its own line:
    <tt class="ttfamily">A</tt> or <tt class="ttfamily">B</tt>. If
    multiple possible answers exist, output any.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>3
1 1
1 2
2 2
</pre>
        </td>
        <td>
          <pre>A
A
B
</pre>
        </td>
      </tr>
    </table>
  </div>
