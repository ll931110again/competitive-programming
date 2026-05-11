# Problem C — Goron City

**Source:** <https://codesprintla26.kattis.com/contests/codesprintla26open/problems/goroncity>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <div style="width:40.00%" class="illustration">
      <img src="https://codesprintla26.kattis.com/problems/goroncity/file/statement/en/img-0001.jpg"
      alt="https://codesprintla26.kattis.com/problems/goroncity/file/statement/en/img-0001.jpg"
      class="illustration">
      <div class="description">
        A Goron; attributed to Htwretched and to Zelda Wiki
      </div>
    </div>
    <p>While exploring the Eldin region of Hyrule, Link meets the
    Gorons, the rock-eating, mountain species in Hyrule, who had
    decided generations ago to build Goron City next to a volcano
    called Death Mountain. Although Link, rather reasonably, is
    worried about an eruption in the near future, the Gorons don’t
    worry in the slightest, as they are highly resistant to fire
    and heat, and could easily survive in an eruption.</p>
    <p>When Death Mountain finally erupts, the Gorons realize that
    while they are resistant to the lava, Goron City is not. In an
    effort to rebuild the city after the eruption, the Gorons call
    Link for advice on the quickest way to rebuild the city.</p>
    <p>Rebuilding Goron City consists of two types of tasks, eating
    rocks, and building houses. There are <span class=
    "tex2jax_process">$N$</span> rocks to eat, where rock
    <span class="tex2jax_process">$i$</span> always takes one day
    to eat and requires strength <span class=
    "tex2jax_process">$a_i$</span>. There are also <span class=
    "tex2jax_process">$M$</span> houses to build, with house
    <span class="tex2jax_process">$j$</span> requiring strength
    <span class="tex2jax_process">$b_j$</span>. However, while
    eating rocks comes naturally to Gorons, building houses is much
    more unnatural and always takes <span class=
    "tex2jax_process">$t$</span> days to complete. All tasks start
    at the beginning of a day and finish at the end of a day. Goron
    City’s reconstruction will be complete after all <span class=
    "tex2jax_process">$N$</span> rocks are eaten and all
    <span class="tex2jax_process">$M$</span> houses are built.</p>
    <p>Though there is much work to do, Link has the help of the
    <span class="tex2jax_process">$G$</span> Gorons in the city.
    Goron <span class="tex2jax_process">$g$</span> has strength
    <span class="tex2jax_process">$c_g$</span>, and therefore can
    eat any rock or build any house that requires strength less
    than or equal to <span class="tex2jax_process">$c_g$</span>.
    The Gorons can independently work on these tasks, and each task
    can only have one Goron working on it. Additionally, each Goron
    must complete the task they are working on before moving on to
    another one. Please help Link assign the Gorons to tasks so
    that Goron City’s reconstruction is complete in the smallest
    number of days.</p>
    <h2>Input</h2>
    <p>The first line contains four space-separated integers
    <span class="tex2jax_process">$N$</span>, <span class=
    "tex2jax_process">$M$</span>, <span class=
    "tex2jax_process">$G$</span>, and <span class=
    "tex2jax_process">$t$</span>, <span class="tex2jax_process">$(0
    \le N, M \le 10^6, 1 \le G \le 10^4, 1 \le t \le 100)$</span>,
    the number of rocks, number of houses, number of Gorons, and
    number of days required to build a house respectively.</p>
    <p>The second line contains <span class=
    "tex2jax_process">$N$</span> space-separated integers
    <span class="tex2jax_process">$(0 \le a_1, \ldots , a_N \le
    10^9)$</span>, where rock <span class=
    "tex2jax_process">$i$</span> requires strength <span class=
    "tex2jax_process">$a_i$</span>.</p>
    <p>The third line contains <span class=
    "tex2jax_process">$M$</span> space-separated integers
    <span class="tex2jax_process">$(0 \le b_1, \ldots , b_M \le
    10^9)$</span>, where house <span class=
    "tex2jax_process">$j$</span> requires strength <span class=
    "tex2jax_process">$b_j$</span>.</p>
    <p>The fourth line contains <span class=
    "tex2jax_process">$G$</span> space-separated integers
    <span class="tex2jax_process">$(0 \le c_1, \ldots , c_G \le
    10^9)$</span>, where Goron <span class=
    "tex2jax_process">$g$</span> has strength <span class=
    "tex2jax_process">$c_g$</span>.</p>
    <h2>Output</h2>
    <p>Output one line containing a single number <span class=
    "tex2jax_process">$D$</span>, which is the minimum number of
    days required for Link and the Gorons to complete Goron City’s
    reconstruction, or <span class="tex2jax_process">$-1$</span> if
    the Gorons cannot reconstruct Goron City.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>2 3 5 4
5 8
2 7 9
3 5 8 10 11
</pre>
        </td>
        <td>
          <pre>4
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
2 7 9
3 5 10
7 11
</pre>
        </td>
        <td>
          <pre>5
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
          <pre>3 3 2 3
2 7 7
3 5 10
7 11
</pre>
        </td>
        <td>
          <pre>6
</pre>
        </td>
      </tr>
    </table>
  </div>
