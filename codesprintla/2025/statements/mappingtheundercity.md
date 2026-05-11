# Mapping the Undercity

**Source:** <https://open.kattis.com/problems/mappingtheundercity>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <p>Vi and Ekko are working together to try to make a map of
    Zaun. Within the city of Zaun, an underground network of
    tunnels connects <span class="tex2jax_process">$N$</span>
    locations, labeled <span class="tex2jax_process">$1$</span>
    through <span class="tex2jax_process">$N$</span>. This network
    forms a connected system where exactly one unique path exists
    between any two locations, and each tunnel segment connects two
    locations. Ekko, starting from the Firelights’ hideout
    (location <span class="tex2jax_process">$1$</span>), and Vi,
    starting from the The Last Drop (location <span class=
    "tex2jax_process">$2$</span>), have each independently measured
    the exact number of tunnel segments required to reach every
    other location in the network. Given these distance
    measurements from both Ekko’s and Vi’s starting points,
    determine how many unique possible layouts of the tunnel
    network could exist while still satisfying both sets of
    recorded distances. Since the answer may be very large, find
    the answer modulo <span class=
    "tex2jax_process">$10^9+7$</span>. It is possible that Vi or
    Ekko made a mistake so that no possible layout could exist
    according to their measurements.</p>
    <h2>Input</h2>
    <p>The first line of input is an integer <span class=
    "tex2jax_process">$N \: (2 \leq N \leq 10^6)$</span>. The
    second line contains <span class="tex2jax_process">$N$</span>
    space-separated integers, <span class="tex2jax_process">$a_1,
    a_2 \ldots , a_N \: (0 \leq a_i \leq 10^6)$</span>, where
    <span class="tex2jax_process">$a_i$</span> is the distance from
    location <span class="tex2jax_process">$i$</span> to the
    Firelights’ hideout. The third line contains <span class=
    "tex2jax_process">$N$</span> space-separated integers,
    <span class="tex2jax_process">$b_1, b_2 \ldots , b_N \: (0 \leq
    b_i \leq 10^6)$</span>, where <span class=
    "tex2jax_process">$b_i$</span> is the distance from location
    <span class="tex2jax_process">$i$</span> to The Last Drop. The
    Firelights’ hideout is at location <span class=
    "tex2jax_process">$1$</span>, and The Last Drop is at location
    <span class="tex2jax_process">$2$</span>.</p>
    <h2>Output</h2>
    <p>Output one line containing a single integer, the number of
    unique tunnel network layouts, modulo <span class=
    "tex2jax_process">$10^9+7$</span>.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>5
0 1 1 1 2
1 0 2 2 3
</pre>
        </td>
        <td>
          <pre>2
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
          <pre>3
0 1 1
1 0 1
</pre>
        </td>
        <td>
          <pre>0
</pre>
        </td>
      </tr>
    </table>
  </div>
