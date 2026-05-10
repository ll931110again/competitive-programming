# Problem E — Lynel Melee

**Source:** <https://codesprintla26.kattis.com/contests/codesprintla26open/problems/lynelmelee>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <div style="width:40.00%" class="illustration">
      <img src=
      "https://codesprintla26.kattis.com/problems/lynelmelee/file/statement/en/img-0001.jpg" alt=
      "https://codesprintla26.kattis.com/problems/lynelmelee/file/statement/en/img-0001.jpg" class=
      "illustration">
      <div class="description">
        Lynel battle
      </div>
    </div>
    <p>Link is exploring a dungeon and encounters an Armored Lynel.
    Armored Lynels are armored enemies that are able to block the
    first <span class="tex2jax_process">$B$</span> damage that they
    receive before they start to take health damage. Specifically,
    block decreases when hit by attacks, and any leftover damage
    from the attack is dealt as health damage. To fight the Armored
    Lynel, Link has arrows that can deal up to <span class=
    "tex2jax_process">$N$</span> damage. Specifically, Link is able
    to do <span class="tex2jax_process">$a_i$</span> attacks of
    <span class="tex2jax_process">$i$</span> damage. However,
    arrows that deal at least <span class=
    "tex2jax_process">$5$</span> damage are called “heavy” arrows,
    and because of their weight, Link can only carry up to
    <span class="tex2jax_process">$10^4$</span> heavy arrows.</p>
    <p>Although things look challenging, thankfully, Link has
    Barbarian Armor equipped, which makes any attack that deals at
    least 1 health damage to the Armored Lynel deal at least 5
    health damage. In other words, if health damage was some
    positive integer <span class="tex2jax_process">$h$</span>,
    health damage modified by the Barbarian armor <span class=
    "tex2jax_process">$h'=\max (h,5)$</span>. Additionally, Link
    has come prepared. He’s made sure to have enough heavy arrows
    to break an Armored Lynel’s block. Link can slay an Armored
    Lynel if he can break its block and reduce its health to 0.
    What is the maximum health Armored Lynel Link can slay?</p>
    <h2>Input</h2>
    <p>The first line contains two space-separated integers,
    <span class="tex2jax_process">$N$</span> and <span class=
    "tex2jax_process">$B$</span>, <span class="tex2jax_process">$(1
    \leq N \leq 1\, 000, 1 \leq B \leq 10^{7})$</span>, the maximum
    attack damage and the block of the Armored Lynel.</p>
    <p>The next line contains <span class=
    "tex2jax_process">$N$</span> space-separated integers
    <span class="tex2jax_process">$a_{1}, a_{2}, \ldots ,
    a_{N}$</span> <span class="tex2jax_process">$(0 \leq a_{1},
    a_{2}, \ldots , a_{N} \leq 10^{9})$</span>, with <span class=
    "tex2jax_process">$a_{i}$</span> giving the number of attacks
    of damage <span class="tex2jax_process">$i$</span>.</p>
    <p>It is guaranteed that the <span class=
    "tex2jax_process">$\sum a_{i}$</span> for <span class=
    "tex2jax_process">$i \geq 5$</span> does not exceed
    <span class="tex2jax_process">$10^{4}$</span> and that
    <span class="tex2jax_process">$\sum a_{i} \cdot i$</span> for
    <span class="tex2jax_process">$i \geq 5$</span> exceeds
    <span class="tex2jax_process">$B$</span>.</p>
    <h2>Output</h2>
    <p>Output one line containing a single integer, the maximum
    health Armored Lynel Link can slay.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>6 5
1 0 0 0 0 1
</pre>
        </td>
        <td>
          <pre>10
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
          <pre>6 1
0 1 0 0 0 1
</pre>
        </td>
        <td>
          <pre>11
</pre>
        </td>
      </tr>
    </table>
  </div>
