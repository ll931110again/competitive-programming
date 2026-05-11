# Problem L — Spirit Tracks

**Source:** <https://codesprintla26.kattis.com/contests/codesprintla26open/problems/spirittracks>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <div style="width:30.00%" class="illustration">
      <img src=
      "https://codesprintla26.kattis.com/problems/spirittracks/file/statement/en/img-0001.jpg" alt=
      "https://codesprintla26.kattis.com/problems/spirittracks/file/statement/en/img-0001.jpg"
      class="illustration">
      <div class="description">
        The Legend of Zelda Spirit Tracks box art
      </div>
    </div>
    <p>Anjean has tasked Link with delivering Force Gems to the
    various realms of New Hyrule to prevent the Demon King Malladus
    from resurrecting. However, Malladus has scrambled the
    junctions of the Spirit Tracks. Help Link navigate to deliver
    the Force Gems before it’s too late!<br>
    The Spirit Tracks can be represented by a tree with
    <span class="tex2jax_process">$N$</span> nodes, where each node
    is assigned a number from 1 to <span class=
    "tex2jax_process">$N$</span>.</p>
    <ul class="itemize">
      <li>
        <p>Nodes of degree 1 represent stations.</p>
      </li>
      <li>
        <p>Nodes of degree 2 represent bridges.</p>
      </li>
      <li>
        <p>Nodes of degree 3 represent junctions.</p>
      </li>
      <li>
        <p>Edges represent tracks.</p>
      </li>
      <li>
        <p>It is guaranteed that no nodes with degree greater than
        3 exist.</p>
      </li>
    </ul>
    <p>Each junction <span class="tex2jax_process">$i$</span> has a
    corresponding spirit threshold, <span class=
    "tex2jax_process">$s_i$</span>, and each edge <span class=
    "tex2jax_process">$j$</span> has a corresponding priority
    <span class="tex2jax_process">$p_j$</span>.<br>
    When Link departs from station <span class=
    "tex2jax_process">$d$</span> carrying a force gem <span class=
    "tex2jax_process">$f$</span>, his train exits the station
    through its only edge and is directed as follows:</p>
    <ul class="itemize">
      <li>
        <p>If the train encounters a station, it stops.</p>
      </li>
      <li>
        <p>If the train encounters a bridge, it continues across
        the bridge and onto the other edge.</p>
      </li>
      <li>
        <p>If the train encounters a junction with spirit threshold
        <span class="tex2jax_process">$s_i$</span>,</p>
        <ul class="itemize">
          <li>
            <p>If <span class="tex2jax_process">$f\le s_i$</span>,
            then the train continues onto the edge with lesser
            priority among the two other edges at that
            junction.</p>
          </li>
          <li>
            <p>If <span class="tex2jax_process">$f&gt;s_i$</span>,
            then the train continues onto the edge with greater
            priority among the two other edges at that
            junction.</p>
          </li>
        </ul>
      </li>
    </ul>
    <p>You are tasked to answer the following types of queries.
    Queries are processed in order, and updates from queries of
    type <tt class="ttfamily">1</tt> persist and affect all future
    queries.</p>
    <ul class="itemize">
      <li>
        <p><tt class="ttfamily">1 a b</tt> Change the spirit
        threshold of junction <span class=
        "tex2jax_process">$a$</span> to value <span class=
        "tex2jax_process">$b$</span>.</p>
      </li>
      <li>
        <p><tt class="ttfamily">2 d f</tt> Determine Link’s
        destination if he departs from station <span class=
        "tex2jax_process">$d$</span> carrying force gem
        <span class="tex2jax_process">$f$</span>.</p>
      </li>
    </ul>
    <h2>Input</h2>
    <p>The first line contains two space-separated integers
    <span class="tex2jax_process">$N$</span>, <span class=
    "tex2jax_process">$M$</span>, and <span class=
    "tex2jax_process">$Q$</span> (<span class=
    "tex2jax_process">$2\le N\le 10^5$</span>, <span class=
    "tex2jax_process">$0\le M\le \frac{N-1}{2}$</span>,
    <span class="tex2jax_process">$1\le Q\le 10^5$</span>)
    representing the number of nodes in the tree, the number of
    junctions, and the number of queries.</p>
    <p>The next <span class="tex2jax_process">$N-1$</span> lines
    contain three space-separated integers <span class=
    "tex2jax_process">$u$</span>, <span class=
    "tex2jax_process">$v$</span>, and <span class=
    "tex2jax_process">$p$</span> (<span class=
    "tex2jax_process">$1\le u,v\le N$</span>, <span class=
    "tex2jax_process">$1\le p\le 10^9$</span>) representing a edge
    from node <span class="tex2jax_process">$u$</span> to node
    <span class="tex2jax_process">$v$</span> with priority
    <span class="tex2jax_process">$p$</span>.</p>
    <p>The next <span class="tex2jax_process">$M$</span> lines
    contain two space-separated integers <span class=
    "tex2jax_process">$j$</span> and <span class=
    "tex2jax_process">$s$</span> (<span class=
    "tex2jax_process">$1\le j\le N$</span>, <span class=
    "tex2jax_process">$0\le s\le 10^9$</span>) representing a
    junction at node <span class="tex2jax_process">$j$</span> with
    spirit threshold <span class="tex2jax_process">$s$</span>.</p>
    <p>The next <span class="tex2jax_process">$Q$</span> lines
    follow in the query format described above, where</p>
    <ul class="itemize">
      <li>
        <p>For all <span class="tex2jax_process">$a$</span> and
        <span class="tex2jax_process">$b$</span>, <span class=
        "tex2jax_process">$(1\le a\le N, 0\le b\le 10^9)$</span>
        and <span class="tex2jax_process">$a$</span> is a
        junction.</p>
      </li>
      <li>
        <p>For all <span class="tex2jax_process">$d$</span> and
        <span class="tex2jax_process">$f$</span>, <span class=
        "tex2jax_process">$(1\le d\le N, 1\le f\le 10^9)$</span>
        and <span class="tex2jax_process">$d$</span> is a
        station.</p>
      </li>
    </ul>
    <p>It is guaranteed that the given nodes and edges form a tree
    with maximum degree 3. It is guaranteed that nodes described as
    stations, bridges, and junctions have degrees 1, 2, and 3,
    respectively. It is guaranteed that no two edges adjacent to
    the same node have the same priority.</p>
    <h2>Output</h2>
    <p>For each query of type <tt class="ttfamily">2</tt>, output
    the result of that query on its own line. It is guaranteed
    there will be at least one such query.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>2 0 2
1 2 1
2 1 100
2 2 1
</pre>
        </td>
        <td>
          <pre>2
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
          <pre>5 1 6
1 2 1
1 3 2
2 4 2
2 5 3
2 1
2 4 1
2 4 2
2 5 1
1 2 4
2 3 2
2 4 2
</pre>
        </td>
        <td>
          <pre>3
5
3
4
3
</pre>
        </td>
      </tr>
    </table>
  </div>
