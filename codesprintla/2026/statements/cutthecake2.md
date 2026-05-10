# Problem A — Cut the Cake

**Source:** <https://codesprintla26.kattis.com/contests/codesprintla26open/problems/cutthecake2>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <div style="width:40.00%" class="illustration">
      <img src=
      "https://codesprintla26.kattis.com/problems/cutthecake2/file/statement/en/img-0001.png" alt=
      "https://codesprintla26.kattis.com/problems/cutthecake2/file/statement/en/img-0001.png" class=
      "illustration">
      <div class="description">
        Example Interaction
      </div>
    </div>
    <p>Link is buying a cake from Pita and Wheaton’s bakery.
    However, his cake has been cut to certain sizes for his friends
    and then put back together. However, he can’t find the slices
    anymore! Help him find them.</p>
    <p>His cake can be described as a rectangle whose corners lie
    at <span class="tex2jax_process">$(0, 0)$</span>, <span class=
    "tex2jax_process">$(N, 0)$</span>, <span class=
    "tex2jax_process">$(N, M)$</span>, and <span class=
    "tex2jax_process">$(0, M)$</span>. This cake is cut into
    <span class="tex2jax_process">$K$</span> different rectangles,
    through a series of axis-aligned cuts. Each of these cuts
    chooses one of the current rectangle slices, and completely
    cuts it into two slices. Each of these resulting rectangles
    have side lengths that are integers. All the slices have then
    been put together again into their original shape in the exact
    same way as they were constructed.</p>
    <p>Link can feel around a specific part of the cake and ask a
    query <span class="tex2jax_process">$X_1$</span> <span class=
    "tex2jax_process">$X_2$</span> <span class=
    "tex2jax_process">$Y_1$</span> <span class=
    "tex2jax_process">$Y_2 \: (0 \leq X_1 &lt; X_2 \leq N, 0 \leq
    Y_1 &lt; Y_2 \leq M)$</span>, which asks the length of the
    longest cut within the axis-aligned subrectangle stretching
    from <span class="tex2jax_process">$(X_1, Y_1)$</span> to
    <span class="tex2jax_process">$(X_2, Y_2)$</span>. Cuts not
    entirely contained within the subrectangle still contribute to
    our query, but we only consider the part of that cut that is
    contained within the subrectangle. Cuts on the edges of the
    subrectangle are excluded from the query. Because cuts are
    restricted to individual slices, two colinear cuts made on
    opposite sides of a perpendicular cut remain distinct. They do
    not combine to form one continuous cut for queries. If no such
    cut exists inside the subrectangle, he gets a value of
    <span class="tex2jax_process">$0$</span>.</p>
    <p>Using at most <span class="tex2jax_process">$1600$</span>
    queries, help him find all internal cuts of the cake.</p>
    <h2>Input</h2>
    <p>The only line of input contains three space-separated
    integers <span class="tex2jax_process">$N$</span>, <span class=
    "tex2jax_process">$M$</span>, and <span class=
    "tex2jax_process">$K$</span> <span class="tex2jax_process">$(2
    \leq N \leq 2\, 000, 2 \leq M \leq 2\, 000, 2 \leq K \leq \min
    (NM, 100))$</span>, the width of the cake, the height of the
    cake, and the number of constituent rectangles
    respectively.</p>
    <h2>Interaction</h2>
    <p>This is an interactive problem. You can interact with the
    cake using standard input and output, but you only have time to
    make <span class="tex2jax_process">$1600$</span> queries before
    his friends arrive.</p>
    <p>To ask a query, write to standard output in the following
    format:</p>
    <ul class="itemize">
      <li>
        <p>“<tt class="ttfamily">?</tt> <span class=
        "tex2jax_process">$X_1$</span> <span class=
        "tex2jax_process">$X_2$</span> <span class=
        "tex2jax_process">$Y_1$</span> <span class=
        "tex2jax_process">$Y_2$</span>” <span class=
        "tex2jax_process">$\: (0 \leq X_1 &lt; X_2 \leq N, 0 \leq
        Y_1 &lt; Y_2 \leq M)$</span></p>
      </li>
    </ul>
    <p>As a response, you will receive the longest cut within the
    subrectangle from <span class="tex2jax_process">$(X_1,
    Y_1)$</span> to <span class="tex2jax_process">$(X_2,
    Y_2)$</span>. If there is no such cut within the subrectangle,
    you will receive <span class="tex2jax_process">$0$</span>.</p>
    <p>When you determine the answer for the current test case,
    print:</p>
    <ul class="itemize">
      <li>
        <p>“<tt class="ttfamily">!</tt> <span class=
        "tex2jax_process">$P$</span>” <span class=
        "tex2jax_process">$\: (1 \leq P \leq K)$</span></p>
      </li>
    </ul>
    <p>This means that you will print <span class=
    "tex2jax_process">$P$</span> more lines of output. Please
    restrict <span class="tex2jax_process">$P$</span> to the range
    required; otherwise you will receive a verdict of <tt class=
    "ttfamily">Wrong Answer</tt>. Each line of output should
    consist of</p>
    <ul class="itemize">
      <li>
        <p>“<span class="tex2jax_process">$t$</span> <span class=
        "tex2jax_process">$a$</span> <span class=
        "tex2jax_process">$b$</span> <span class=
        "tex2jax_process">$c$</span>”</p>
      </li>
    </ul>
    <p>where <span class="tex2jax_process">$a &lt; b$</span>.
    <span class="tex2jax_process">$t$</span> is either “<tt class=
    "ttfamily">h</tt>” or “<tt class="ttfamily">v</tt>”, which
    represents a horizontal or vertical cut respectively. If
    <span class="tex2jax_process">$t = \texttt{h}$</span>, this
    represents a horizontal cake cut stretching from <span class=
    "tex2jax_process">$(a, c)$</span> to <span class=
    "tex2jax_process">$(b, c) \: (0 \leq a &lt; b \leq N, 0 &lt; c
    &lt; M)$</span>. If <span class="tex2jax_process">$t =
    \texttt{v}$</span>, this represents a vertical cake cut
    stretching from <span class="tex2jax_process">$(c, a)$</span>
    to <span class="tex2jax_process">$(c, b) \: (0 \leq a &lt; b
    \leq M, 0 &lt; c &lt; N)$</span>. Any outputting of cuts is
    acceptable as long as they exactly cover all cuts in the
    rectangle (i.e. they do not cover any non-cuts). The cuts you
    output may even overlap.</p>
    <p>The judge is not adaptive, meaning that the cuts for a given
    test case are known to the judge before the participant starts
    to query. The total number of queries allowed is <span class=
    "tex2jax_process">$1600$</span>, meaning that your program can
    only output <span class="tex2jax_process">$1600$</span> lines
    starting with <tt class="ttfamily">?</tt>. In particular, the
    final answer does not count as a query. If the number of
    queries is exceeded, you will receive a verdict of <tt class=
    "ttfamily">Wrong Answer</tt>. If, at any interaction step, you
    read <span class="tex2jax_process">$-1$</span> instead of valid
    data, your solution must exit immediately. This means that your
    solution will receive <tt class="ttfamily">Wrong answer</tt>
    because of an invalid query or any other mistake. Failing to
    exit can result in an arbitrary verdict because your solution
    will continue to read from a closed stream. Any queries not in
    the specified form will immediately result in a verdict of
    <tt class="ttfamily">Wrong Answer</tt>. After printing a query
    do not forget to output the end of line and flush the output.
    Otherwise, you may receive a verdict of <tt class=
    "ttfamily">Idleness limit exceeded</tt>. To do this, use</p>
    <ul class="itemize">
      <li>
        <p>fflush(stdout) or cout.flush() in C++;</p>
      </li>
      <li>
        <p>System.out.flush() in Java;</p>
      </li>
      <li>
        <p>stdout.flush() in Python;</p>
      </li>
    </ul>
    <p>See documentation for other languages.</p>
    <p>A testing tool is provided as an attachment to help you
    develop your solution.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th style="text-align:left; width:33%;">Read</th>
        <th style="text-align:center; width:33%;">Sample
        Interaction 1</th>
        <th style="text-align:right; width:33%;">Write</th>
      </tr>
    </table>
    <div class="sampleinteractionread">
      <pre> 3 3 4</pre>
    </div>
    <div class="sampleinteractionwrite">
      <pre> ? 1 3 2 3</pre>
    </div>
    <div class="sampleinteractionread">
      <pre> 0</pre>
    </div>
    <div class="sampleinteractionwrite">
      <pre> ? 0 2 0 2</pre>
    </div>
    <div class="sampleinteractionread">
      <pre> 2</pre>
    </div>
    <div class="sampleinteractionwrite">
      <pre> ? 1 3 1 3</pre>
    </div>
    <div class="sampleinteractionread">
      <pre> 2</pre>
    </div>
    <div class="sampleinteractionwrite">
      <pre> ? 1 3 0 2</pre>
    </div>
    <div class="sampleinteractionread">
      <pre> 2</pre>
    </div>
    <div class="sampleinteractionwrite">
      <pre> ? 0 2 2 3</pre>
    </div>
    <div class="sampleinteractionread">
      <pre> 1</pre>
    </div>
    <div class="sampleinteractionwrite">
      <pre> ! 3
 v 0 3 1
 h 1 3 2
 v 0 2 2</pre>
    </div>
  </div>
