# Topside vs Zaun

**Source:** <https://open.kattis.com/problems/topsidevszaun>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <p>Jinx and Vi are drafting teams to make the most equitable
    teams for the fated battle between Topside and Zaun. Currently,
    there are <span class="tex2jax_process">$N$</span> people who
    are undecided on which side they want to fight for. Each person
    has a skill level <span class="tex2jax_process">$a_i$</span>
    that they bring to the table.</p>
    <p>While Jinx and Vi would normally want to draft the strongest
    fighters for their side, they need to drive up ratings for
    their hit show, Arcane. Instead, they decide to draft teams
    that are equally matched so they have a few restrictions.</p>
    <ol class="enumerate">
      <li>
        <p>Each team must have an equal number of people.</p>
      </li>
      <li>
        <p>Since a person can’t fight for both sides, each person
        may only be a part of at most one team.</p>
      </li>
      <li>
        <p>The sum of the skill levels of each team must be
        equal.</p>
      </li>
      <li>
        <p>The size of the team must be as large as possible. If
        this is not the case, then each fighter can’t get good
        character development.</p>
      </li>
    </ol>
    <p>They want to figure out the size and skills of the teams
    that satisfies these condition, or if none exists. Can you help
    them out?</p>
    <h2>Input</h2>
    <p>The first line of input contains <span class=
    "tex2jax_process">$N$</span> (<span class="tex2jax_process">$1
    \leq N \leq 300$</span>), the number of undecided people. The
    second line of input contains <span class=
    "tex2jax_process">$N$</span> space-separated integers
    <span class="tex2jax_process">$a_1, \ldots , a_N$</span>
    (<span class="tex2jax_process">$1 \le a_i \le 300$</span>)
    representing the skill level of each person.</p>
    <h2>Output</h2>
    <p>Output one line containing the maximal team size
    <span class="tex2jax_process">$S$</span>, or <span class=
    "tex2jax_process">$-1$</span> if a team is impossible to
    construct.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>6
1 1 4 1 2 3
</pre>
        </td>
        <td>
          <pre>3
</pre>
        </td>
      </tr>
    </table>
  </div>
