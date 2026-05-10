# Olympic Ceremony

**Source:** <https://open.kattis.com/problems/olympicceremony>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <p>There are <span class="tex2jax_process">$N$</span> Olympic
    athletes in line for the opening ceremony of the Piltover
    Olympic Games. The <span class="tex2jax_process">$i$</span>-th
    athlete in line belongs to some district <span class=
    "tex2jax_process">$D_i$</span> in Piltover, but the organizers
    forgot to tell you which district each athlete belongs to. Even
    worse, the ceremony is about to begin, and the athletes will
    have no one to talk to if they aren’t with fellow athletes from
    their district. To make all the athletes feel more comfortable,
    you want all the athletes from a given district to be together
    in line. Additionally, even though you don’t have the list of
    athletes’ districts, you have a device to communicate with the
    organizers and learn information about the athletes’ districts.
    Can you find a permutation of the athletes that makes the
    athletes comfortable in time for the start of the ceremony?</p>
    <h2>Input</h2>
    <p>The only line of input is an integer <span class=
    "tex2jax_process">$N \: (1 \leq N \leq 1\, 000)$</span>, where
    <span class="tex2jax_process">$N$</span> represents the number
    of Olympic athletes.</p>
    <h2>Interaction</h2>
    <p>This is an interactive problem. You can interact with the
    Olympic organizers using standard input and output, but you
    only have time to ask <span class="tex2jax_process">$10\,
    000$</span> questions before the ceremony begins.<br>
    To ask a question, write to standard output in the following
    format:</p>
    <ul class="itemize">
      <li>
        <p>“<tt class="ttfamily">?</tt> <span class=
        "tex2jax_process">$l$</span> <span class=
        "tex2jax_process">$r$</span>” (<span class=
        "tex2jax_process">$1 \le l \le r \le N$</span>) — How many
        unique values are in the list <span class=
        "tex2jax_process">$D_l, \ldots , D_r$</span>? The
        organizers will respond with “<tt class="ttfamily">!</tt>
        <span class="tex2jax_process">$k$</span>” to tell you that
        there are <span class="tex2jax_process">$k$</span> unique
        values. They have a bad habit of adding “<tt class=
        "ttfamily">!</tt>” before each of their answers that they
        won’t fix no matter how much you bug them.</p>
      </li>
    </ul>
    <p>To submit the desired permutation of athletes, write to
    standard output in the following format:</p>
    <ul class="itemize">
      <li>
        <p>“<tt class="ttfamily">!</tt> <span class=
        "tex2jax_process">$P_1, \ldots , P_N$</span>” (<span class=
        "tex2jax_process">$1 \le P_i \le N$</span> and all
        <span class="tex2jax_process">$P_i$</span> distinct) —
        Athlete <span class="tex2jax_process">$P_i$</span> should
        be in the <span class="tex2jax_process">$i$</span>-th
        position, and athletes from the same district should be
        together. More precisely, for every <span class=
        "tex2jax_process">$1 \le i \le j \le k \le N$</span>, if
        <span class="tex2jax_process">$D_{P_i} = D_{P_k}$</span>,
        then <span class="tex2jax_process">$D_{P_i} =
        D_{P_j}$</span>.</p>
      </li>
    </ul>
    <p>There may be multiple permutatations that satisfy the above.
    You may output any one of them.<br></p>
    <p>The judge is not adaptive, meaning that the values
    <span class="tex2jax_process">$D_1,\ldots ,D_N$</span> for a
    given test case are known to the judge before the participant
    starts to query.<br>
    The total number of queries allowed is <span class=
    "tex2jax_process">$10^4$</span>, meaning that your program can
    only output <span class="tex2jax_process">$10^4$</span> lines
    starting with <tt class="ttfamily">?</tt>. In particular, the
    final answer does not count as a query. If the number of
    queries is exceeded, you will receive a verdict of <tt class=
    "ttfamily">Wrong Answer</tt>.<br>
    Any queries not in the specified form will immediately result
    in a verdict of <tt class="ttfamily">Wrong Answer</tt>. After
    printing a query do not forget to output the end of line and
    flush the output. Otherwise, you may receive a verdict of
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
    <table class="sample" summary="sample data">
      <tr>
        <th style="text-align:left; width:33%;">Read</th>
        <th style="text-align:center; width:33%;">Sample
        Interaction 1</th>
        <th style="text-align:right; width:33%;">Write</th>
      </tr>
    </table>
    <div class="sampleinteractionread">
      <pre> 3</pre>
    </div>
    <div class="sampleinteractionwrite">
      <pre> ? 2 3</pre>
    </div>
    <div class="sampleinteractionread">
      <pre> ! 1</pre>
    </div>
    <div class="sampleinteractionwrite">
      <pre> ! 3 2 1</pre>
    </div>
  </div>
