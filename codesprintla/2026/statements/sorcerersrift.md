# Problem K — Sorcerer’s Rift

**Source:** <https://codesprintla26.kattis.com/contests/codesprintla26open/problems/sorcerersrift>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <div style="width:33.00%" class="illustration">
      <img src=
      "https://codesprintla26.kattis.com/problems/sorcerersrift/file/statement/en/img-0001.jpg" alt=
      "https://codesprintla26.kattis.com/problems/sorcerersrift/file/statement/en/img-0001.jpg"
      class="illustration">
      <div class="description">
        Artwork of Yuga
      </div>
    </div>
    <p>The evil sorcerer Yuga has kidnapped Princess Zelda and
    escaped through a rift between Hyrule and Lorule. Now Link must
    locate the sorcerer’s rift to travel to Lorule and save Zelda.
    Luckily, he has the help of 2 spirits, which can aid him to
    find it.<br>
    There exist <span class="tex2jax_process">$N$</span> and
    <span class="tex2jax_process">$M$</span> locations in Hyrule
    and Lorule respectively. Roads exist between locations such
    that there exists exactly one path between any pair of
    locations within the same world. Additionally, the roads have
    been built such that <b class="bfseries">no more than 4 roads
    meet at any given location in either world</b>. It is known
    that the sorcerer’s rift exists between an unknown location in
    Hyrule and an unknown location in Lorule, allowing there to
    exist a path between any pair of locations.<br>
    In order to find the rift, Link can instruct each spirit to
    travel between any pair of locations. The 2 spirits can
    magically navigate between their pairs of locations, but can
    only communicate to Link the number of locations that both
    traveled through (including their start and end locations).
    Additionally, the spirits can only do this 36 times before
    their magic gets depleted.<br>
    Can you determine the locations on either side of the rift?</p>
    <h2>Input</h2>
    <p>The first line of input contains two space-separated
    integers <span class="tex2jax_process">$N$</span> and
    <span class="tex2jax_process">$M$</span> (<span class=
    "tex2jax_process">$1 \le N,M &lt; 2\cdot 10^5)$</span>
    representing the number of locations in Hyrule and Lorule,
    respectively.</p>
    <p>The following <span class="tex2jax_process">$N-1$</span>
    lines contain two space-separated integers <span class=
    "tex2jax_process">$a$</span> and <span class=
    "tex2jax_process">$b$</span> (<span class=
    "tex2jax_process">$1\le a,b\le N$</span>) representing a road
    between location <span class="tex2jax_process">$a$</span> and
    location <span class="tex2jax_process">$b$</span> in
    Hyrule.</p>
    <p>The following <span class="tex2jax_process">$M-1$</span>
    lines contain two space-separated integers <span class=
    "tex2jax_process">$c$</span> and <span class=
    "tex2jax_process">$d$</span> (<span class=
    "tex2jax_process">$N+1\le c,d\le N+M$</span>) representing a
    road between location <span class="tex2jax_process">$c$</span>
    and location <span class="tex2jax_process">$d$</span> in
    Lorule.</p>
    <h2>Interaction</h2>
    <p>This is an interactive problem. You can interact with the
    spirits using standard input and output, but they only have
    enough magic to do <span class="tex2jax_process">$36$</span>
    trips.</p>
    <p>To instruct the spirits, write to standard output in the
    following format:</p>
    <ul class="itemize">
      <li>
        <p>“<tt class="ttfamily">?</tt> <span class=
        "tex2jax_process">$w$</span> <span class=
        "tex2jax_process">$x$</span> <span class=
        "tex2jax_process">$y$</span> <span class=
        "tex2jax_process">$z$</span>” (<span class=
        "tex2jax_process">$1\le w,x,y,z\le N+M$</span>) — With
        spirit 1 traveling between locations <span class=
        "tex2jax_process">$w$</span> and <span class=
        "tex2jax_process">$x$</span> and spirit 2 traveling between
        locations <span class="tex2jax_process">$y$</span> and
        <span class="tex2jax_process">$z$</span>, how many
        locations did they both travel through? The spirits will
        respond with a single integer <span class=
        "tex2jax_process">$k$</span>, the number of locations.</p>
      </li>
    </ul>
    <p>To submit the location of the rift, write to standard output
    in the following format:</p>
    <ul class="itemize">
      <li>
        <p>“<tt class="ttfamily">!</tt> <span class=
        "tex2jax_process">$u$</span> <span class=
        "tex2jax_process">$v$</span>” (<span class=
        "tex2jax_process">$1\le u\le N$</span>, <span class=
        "tex2jax_process">$N+1\le v\le N+M$</span>) — The rift is
        located between location <span class=
        "tex2jax_process">$u$</span> in Hyrule and location
        <span class="tex2jax_process">$v$</span> in Lorule.</p>
      </li>
    </ul>
    <p>The judge is not adaptive, meaning that the values
    <span class="tex2jax_process">$k$</span> for a given test case
    are known to the judge before the participant starts to
    query.</p>
    <p>The total number of queries allowed is <span class=
    "tex2jax_process">$36$</span>, meaning that your program can
    only output <span class="tex2jax_process">$36$</span> lines
    starting with <tt class="ttfamily">?</tt>. In particular, the
    final answer does not count as a query. If the number of
    queries is exceeded, you will receive a verdict of <tt class=
    "ttfamily">Wrong Answer</tt>.</p>
    <p>Any queries not in the specified form will immediately
    result in a verdict of <tt class="ttfamily">Wrong Answer</tt>.
    After printing a query do not forget to output the end of line
    and flush the output. Otherwise, you may receive a verdict of
    <tt class="ttfamily">Idleness limit exceeded</tt>. To do this,
    use</p>
    <ul class="itemize">
      <li>
        <p><tt class="ttfamily">fflush(stdout)</tt> or <tt class=
        "ttfamily">cout.flush()</tt> in C++;</p>
      </li>
      <li>
        <p><tt class="ttfamily">System.out.flush()</tt> in
        Java;</p>
      </li>
      <li>
        <p><tt class="ttfamily">stdout.flush()</tt> in Python;</p>
      </li>
      <li>
        <p>see documentation for other languages.</p>
      </li>
    </ul>
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
      <pre> 1 2
 2 3</pre>
    </div>
    <div class="sampleinteractionwrite">
      <pre> ? 1 2 3 3</pre>
    </div>
    <div class="sampleinteractionread">
      <pre> 1</pre>
    </div>
    <div class="sampleinteractionwrite">
      <pre> ! 1 3</pre>
    </div>
  </div>
