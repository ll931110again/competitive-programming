# Problem G — Multiverse Madness

**Source:** <https://codesprintla26.kattis.com/contests/codesprintla26open/problems/multiversemadness>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <div style="width:40.00%" class="illustration">
      <img src=
      "https://codesprintla26.kattis.com/problems/multiversemadness/file/statement/en/img-0001.jpg"
      alt=
      "https://codesprintla26.kattis.com/problems/multiversemadness/file/statement/en/img-0001.jpg"
      class="illustration">
      <div class="description">
        The Gate of Time
      </div>
    </div>
    <p>After venturing to Termina, Link is trying to figure out how
    parallel worlds and timelines work. He knows that an event
    happening/not happening in one world could maybe lead to that
    same event happening/not happening in another world, but he
    doesn’t exactly know what leads to what.</p>
    <p>He’s currently examining a single event as a case study and
    has currently mapped out <span class=
    "tex2jax_process">$N$</span> worlds. Each world is assigned a
    binary value <span class="tex2jax_process">$0$</span> (for not
    happening) or <span class="tex2jax_process">$1$</span> (for
    happening). He also knows that the worlds are connected by
    <span class="tex2jax_process">$N-1$</span> edges. These edges
    are directed. Thus, if edge <span class=
    "tex2jax_process">$i$</span> leads from <span class=
    "tex2jax_process">$A_i$</span> to <span class=
    "tex2jax_process">$B_i$</span>, that means that the event in
    world <span class="tex2jax_process">$A_i$</span> could possibly
    affect events in <span class="tex2jax_process">$B_i$</span>.
    Note, however, if these edges were undirected, the graph
    structure would form a tree.</p>
    <p>Let’s define the parent worlds of a world <span class=
    "tex2jax_process">$j$</span> to be the set of all worlds with
    an edge pointing into world <span class=
    "tex2jax_process">$j$</span>. If the set of parents is not
    empty, the value of world <span class=
    "tex2jax_process">$j$</span> can be written as some formula in
    Boolean algebra dependent on some subset (potentially empty) of
    the values of node <span class="tex2jax_process">$j$</span>’s
    parent worlds. For example, if <span class=
    "tex2jax_process">$B$</span> and <span class=
    "tex2jax_process">$C$</span> were the parent world of
    <span class="tex2jax_process">$A$</span>, two possible example
    formulas are <span class="tex2jax_process">$A$</span> = NOT
    <span class="tex2jax_process">$B$</span> OR (<span class=
    "tex2jax_process">$B$</span> AND <span class=
    "tex2jax_process">$C$</span>) or <span class=
    "tex2jax_process">$A$</span> = TRUE. If the set of parents is
    empty for a world <span class="tex2jax_process">$j$</span>
    (i.e. <span class="tex2jax_process">$j$</span> is a root),
    however, Link will simply know that <span class=
    "tex2jax_process">$j = 0$</span> with some probability
    <span class="tex2jax_process">$p$</span> and <span class=
    "tex2jax_process">$j = 1$</span> with some probability
    <span class="tex2jax_process">$1 - p$</span> for some unknown
    value of <span class="tex2jax_process">$p$</span>.</p>
    <p>In summary, Hyrule’s multiverse can be defined as consisting
    of a polytree structure, along with probability distributions
    defining the values of root worlds, and logical formulas
    defining how non-root worlds will be determined. However, Link
    only knows the structure and not any distributions or logical
    formulas. To make further progress on the problem, he is
    creating a machine that will exactly determine the binary
    values of <span class="tex2jax_process">$K$</span> different
    world <span class="tex2jax_process">$F_1, F_2, \ldots ,
    F_K$</span>. He hasn’t run this machine yet so he doesn’t
    exactly know what values he would receive though.</p>
    <p>Help Link answer <span class="tex2jax_process">$Q$</span>
    queries. Each query consists of two worlds <span class=
    "tex2jax_process">$C_i$</span> and <span class=
    "tex2jax_process">$D_i$</span> (which are not in the set
    <span class="tex2jax_process">$F$</span>). Answer whether there
    will exist some multiverse that follows our tree structure
    where after knowing the exact values <span class=
    "tex2jax_process">$V_1, V_2, \ldots , V_K$</span> of the worlds
    in <span class="tex2jax_process">$F_1, F_2, \ldots ,
    F_K$</span>, knowing the value of <span class=
    "tex2jax_process">$C_i$</span> would change the probability
    <span class="tex2jax_process">$D_i$</span> occurs.</p>
    <p>Formally, for a multiverse <span class=
    "tex2jax_process">$M$</span> and a set of known values
    <span class="tex2jax_process">$V=\{ V_1,V_2,\ldots , V_K\}
    $</span> of the set of worlds <span class=
    "tex2jax_process">$F=\{ F_1, F_2, \ldots , F_K\} $</span> that
    may actually happen in <span class=
    "tex2jax_process">$M$</span>, define</p>
    <ol class="enumerate">
      <li>
        <p>Given the values <span class=
        "tex2jax_process">$V$</span> for worlds <span class=
        "tex2jax_process">$F$</span> and without knowing the value
        of <span class="tex2jax_process">$C_i$</span>, let the
        probability of <span class="tex2jax_process">$D_i$</span>
        occurring equals <span class=
        "tex2jax_process">$p_1$</span></p>
      </li>
      <li>
        <p>Given the values <span class=
        "tex2jax_process">$V$</span> for worlds <span class=
        "tex2jax_process">$F$</span> and with the value of
        <span class="tex2jax_process">$C_i$</span> known to be
        <span class="tex2jax_process">$0$</span>, let the
        probability of <span class="tex2jax_process">$D_i$</span>
        occurring equals <span class=
        "tex2jax_process">$p_2$</span></p>
      </li>
      <li>
        <p>Given the values <span class=
        "tex2jax_process">$V$</span> for worlds <span class=
        "tex2jax_process">$F$</span> and with the value of
        <span class="tex2jax_process">$C_i$</span> known to be
        <span class="tex2jax_process">$1$</span>, let the
        probability of <span class="tex2jax_process">$D_i$</span>
        occurring equal <span class=
        "tex2jax_process">$p_3$</span></p>
      </li>
    </ol>
    <p>Disregard any multiverses where <span class=
    "tex2jax_process">$C_i$</span> has zero probability to be
    <span class="tex2jax_process">$0$</span> or zero probability to
    be <span class="tex2jax_process">$1$</span>.</p>
    <p>Answer <span class="tex2jax_process">$0$</span> to the query
    if for all possible viable multiverses <span class=
    "tex2jax_process">$M$</span> that follow our given polytree
    structure and for all possible valid <span class=
    "tex2jax_process">$F$</span> that may actually exist in
    <span class="tex2jax_process">$M$</span>, <span class=
    "tex2jax_process">$p_1 = p_2 = p_3$</span>. Answer <span class=
    "tex2jax_process">$1$</span> otherwise.</p>
    <h2>Input</h2>
    <p>The first line of input contains three space-separated
    integers <span class="tex2jax_process">$N$</span>, <span class=
    "tex2jax_process">$K$</span>, and <span class=
    "tex2jax_process">$Q$</span> <span class="tex2jax_process">$(2
    \leq N \leq 2\cdot 10^5, 0 \leq K \leq N-2, 1 \leq Q \leq
    2\cdot 10^5)$</span>, the number of worlds, the number of
    worlds whose value we would know, and the number of queries,
    respectively.</p>
    <p>The next <span class="tex2jax_process">$N-1$</span> lines
    contains two space-separated integer <span class=
    "tex2jax_process">$A_i$</span> and <span class=
    "tex2jax_process">$B_i \: (1 \leq A_i, B_i \leq N, A_i \neq
    B_i)$</span>, which implies that <span class=
    "tex2jax_process">$A_i$</span> may possibly affect <span class=
    "tex2jax_process">$B_i$</span> as part of a logical
    formula.</p>
    <p>The next <span class="tex2jax_process">$C$</span> lines
    contains a single integer <span class="tex2jax_process">$F_i \:
    (1 \leq F_i \leq N)$</span>, where <span class=
    "tex2jax_process">$F_i$</span> is the <span class=
    "tex2jax_process">$i$</span>-th world whose value we would
    know.</p>
    <p>The next <span class="tex2jax_process">$Q$</span> lines
    contain two space-separated integers <span class=
    "tex2jax_process">$C_i$</span> and <span class=
    "tex2jax_process">$D_i \: (1 \leq C_i, D_i \leq N, C_i \neq
    D_i)$</span>, where the <span class=
    "tex2jax_process">$i$</span>-th query asks whether knowing the
    value of <span class="tex2jax_process">$C_i$</span> could
    change the probability distribution of <span class=
    "tex2jax_process">$D_i$</span>.</p>
    <h2>Output</h2>
    <p>Output <span class="tex2jax_process">$Q$</span> lines, where
    the <span class="tex2jax_process">$i$</span>-th line contains
    the output for the <span class="tex2jax_process">$i$</span>-th
    query. Output <span class="tex2jax_process">$0$</span> if the
    probability distribution of <span class=
    "tex2jax_process">$D_i$</span> is unchanged upon knowledge of
    <span class="tex2jax_process">$C_i$</span> for any possible
    multiverse that follows our structure and for any possible set
    of values <span class="tex2jax_process">$V$</span> for the set
    of worlds <span class="tex2jax_process">$F$</span> that can
    actually occur within that multiverse. Otherwise, output
    <span class="tex2jax_process">$1$</span>.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>7 1 2
1 2
1 3
4 3
3 5
5 6
6 7
1
2 4
5 7
</pre>
        </td>
        <td>
          <pre>0
1
</pre>
        </td>
      </tr>
    </table>
  </div>
