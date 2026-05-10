# Evasion

**Source:** <https://open.kattis.com/problems/evasion>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <p>Powder was hanging out in the Last Drop, when she just heard
    from Vander that The Last Drop was getting <i class=
    "itshape">another</i> "random" search for illicit <i class=
    "itshape">Hex Crystals</i>. She needs to hide all of her
    <i class="itshape">Hex Crystals</i>, and fast. However, Powder
    also left a lot of <i class="itshape">Hex Crystals</i> in her
    lab. When the Piltover Enforcers get here and find her missing,
    they’re surely going to search her lab too. To avoid detection,
    Powder needs to arrive at her lab from the Last Drop and arrive
    strictly before the Enforcers, while also making sure to avoid
    encountering them on the way. Once there, she can grab her
    crystals and leave instantly. The tunnels are so dark that
    Powder can’t see the Enforcers coming from ahead. The tunnels
    are also so narrow that she will be detected if their paths
    cross through each other at the same time, even if they only
    meet a single point.</p>
    <p>We can represent the tunnel system as an undirected graph,
    where the <span class="tex2jax_process">$n$</span> vertices
    represent junctions and <span class=
    "tex2jax_process">$m$</span> edges represent tunnels. Powder is
    currently at The Last Drop, which is located at vertex
    <span class="tex2jax_process">$s$</span>. Powder’s lab is at
    vertex <span class="tex2jax_process">$t$</span>, and the
    Enforcers are currently at vertex <span class=
    "tex2jax_process">$p$</span>. The Enforcers will begin heading
    directly towards The Last Drop at the exact moment that Powder
    starts moving. Powder assumes the Enforcers will use some
    shortest path to get there, but she doesn’t know which one. As
    soon as the Enforcers arrive at The Last Drop and find Powder
    missing, they will immediately head for Powder’s lab, again
    using some shortest path. Powder will be caught if the
    Enforcers make it to her lab before she does (they will wait
    there for her to arrive) or if she is in the same position in
    the graph at the same time as the Enforcers. Note that Powder
    can be any arbitrarily small, positive distance <span class=
    "tex2jax_process">$\epsilon $</span> away from the Enforcers
    without being caught. Powder assumes she will travel at
    <span class="tex2jax_process">$0.5$</span> <span class=
    "tex2jax_process">$\mathrm{m}/\mathrm{s}$</span> and the
    Enforcers will travel at <span class=
    "tex2jax_process">$1$</span> <span class=
    "tex2jax_process">$\mathrm{m}/\mathrm{s}$</span>. While the
    Enforcers will always follow a shortest path to their goal,
    Powder’s route can be arbitrary, including entering a tunnel
    without traversing the whole tunnel, or stopping and waiting
    for an arbitrary amount of time. Powder only considers a route
    viable if it will succeed regardless of which route the
    Enforcers choose. Does she have time to make it to her lab, nab
    the crystals, and evade the Enforcers?</p>
    <p>If Powder can get to her lab and grab the crystals before
    the Enforcers arrive, output the shortest time in which she can
    do so. It can be shown that for some integer <span class=
    "tex2jax_process">$k$</span>, either the shortest time is
    exactly <span class="tex2jax_process">$k$</span> seconds, or
    Powder cannot finish in <span class=
    "tex2jax_process">$k$</span> seconds but she can finish in
    <span class="tex2jax_process">$k + \epsilon $</span> seconds
    for any arbitrarily small constant <span class=
    "tex2jax_process">$\epsilon &gt; 0$</span>. In either of these
    cases, output <span class="tex2jax_process">$k$</span>.</p>
    <p>If Powder cannot get to her lab before the Enforcers without
    being caught, output <span class=
    "tex2jax_process">$-1$</span>.</p>
    <h2>Input</h2>
    <p>The first line contains five integers, <span class=
    "tex2jax_process">$n$</span>, <span class=
    "tex2jax_process">$m$</span>, <span class=
    "tex2jax_process">$s$</span>, <span class=
    "tex2jax_process">$t$</span>, and <span class=
    "tex2jax_process">$p$</span> (<span class="tex2jax_process">$1
    \leq n \leq 10^5$</span>, <span class="tex2jax_process">$1 \leq
    m \leq 2 \cdot 10^5$</span>, <span class="tex2jax_process">$1
    \leq s, t, p \leq n$</span>), where <span class=
    "tex2jax_process">$n$</span> is the number of junctions,
    <span class="tex2jax_process">$m$</span> is the number of
    tunnels, <span class="tex2jax_process">$s$</span> is the
    location of The Last Drop, <span class=
    "tex2jax_process">$t$</span> is the location of Powder’s lab,
    and <span class="tex2jax_process">$p$</span> is the location
    where the Enforcers start from. All of <span class=
    "tex2jax_process">$s$</span>, <span class=
    "tex2jax_process">$t$</span> and <span class=
    "tex2jax_process">$p$</span> are distinct.<br>
    The next <span class="tex2jax_process">$m$</span> lines each
    contain three integers, <span class=
    "tex2jax_process">$u$</span>, <span class=
    "tex2jax_process">$v$</span>, and <span class=
    "tex2jax_process">$w$</span> (<span class="tex2jax_process">$1
    \leq u, v \leq n$</span>, <span class="tex2jax_process">$1 \leq
    w \leq 10^9$</span>). The triple <span class=
    "tex2jax_process">$(u,v,w)$</span> describes a tunnel from
    vertex <span class="tex2jax_process">$u$</span> to vertex
    <span class="tex2jax_process">$v$</span> with length
    <span class="tex2jax_process">$w$</span> meters. The system of
    tunnels may not be simple (there may be self-loops or multiple
    edges).</p>
    <h2>Output</h2>
    <p>If Powder can get to her lab and grab the crystals before
    the Enforcers, output the shortest time that she can do so, in
    seconds. If Powder cannot get to her lab before the Enforcers
    without being caught, output <span class=
    "tex2jax_process">$-1$</span>.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>3 2 1 2 3
1 2 9
3 1 20
</pre>
        </td>
        <td>
          <pre>18
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
          <pre>3 2 1 2 3
1 2 9
3 1 9
</pre>
        </td>
        <td>
          <pre>-1
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
          <pre>5 4 1 4 5
1 2 200
2 3 1
2 4 100
4 5 400
</pre>
        </td>
        <td>
          <pre>700
</pre>
        </td>
      </tr>
    </table>
  </div>
