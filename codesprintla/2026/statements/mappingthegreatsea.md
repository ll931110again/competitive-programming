# Problem F — Mapping the Great Sea

**Source:** <https://codesprintla26.kattis.com/contests/codesprintla26open/problems/mappingthegreatsea>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <div style="width:45.00%" class="illustration">
      <img src=
      "https://codesprintla26.kattis.com/problems/mappingthegreatsea/file/statement/en/img-0001.jpg"
      alt=
      "https://codesprintla26.kattis.com/problems/mappingthegreatsea/file/statement/en/img-0001.jpg"
      class="illustration">
      <div class="description">
        A map of the great sea
      </div>
    </div>
    <p>Link is sailing the Great Sea! The Great Sea can be
    represented by a square grid with <span class=
    "tex2jax_process">$N$</span> rows and <span class=
    "tex2jax_process">$N$</span> columns, where each cell has a
    value of <span class="tex2jax_process">$1$</span> if it
    contains an island, or a value of <span class=
    "tex2jax_process">$0$</span> if it does not.</p>
    <p>Link is working on mapping out the Great Sea. As such, he is
    interested in figuring out certain properties of these islands.
    In particular, he noticed that each island can be covered with
    a rectangle, where each rectangle can fall into two
    categories:</p>
    <ol class="enumerate">
      <li>
        <p>The first type is a rectangle within the grid that only
        covers <span class="tex2jax_process">$1$</span>’s that can
        no longer be expanded. That is, if the rectangle was
        expanded upward, downwards, leftwards, or rightwards, the
        rectangle would either go out of bounds of the grid or no
        longer contain only <span class=
        "tex2jax_process">$1$</span>’s.</p>
      </li>
      <li>
        <p>The second type is a rectangle of the first type that
        additionally has the restriction that it covers at least
        one <span class="tex2jax_process">$1$</span> that cannot be
        covered by any other rectangle of the first type.</p>
      </li>
    </ol>
    <p>After sailing around for a while, Link now has a complete
    map, but is unable to figure out how to efficiently find these
    aforementioned rectangles on his map. Please help him count the
    number of rectangles of each type!</p>
    <h2>Input</h2>
    <p>The first line contains a single integer <span class=
    "tex2jax_process">$N \: (3 \leq N \leq 2000)$</span>, the side
    length of the square grid.</p>
    <p>The next <span class="tex2jax_process">$N$</span> lines each
    contain <span class="tex2jax_process">$N$</span> integers,
    where each integer is either <span class=
    "tex2jax_process">$0$</span> or <span class=
    "tex2jax_process">$1$</span>. These integers represent the
    square grid of islands.</p>
    <h2>Output</h2>
    <p>Output a single line, containing two space-separated
    integers representing the number of rectangles of the first
    type and rectangles of the second type respectively.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>4
0 1 0 0
1 1 0 0
1 0 0 0
1 0 0 0
</pre>
        </td>
        <td>
          <pre>3 2
</pre>
        </td>
      </tr>
    </table>
  </div>
