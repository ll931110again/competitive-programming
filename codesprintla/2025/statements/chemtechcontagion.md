# Chemtech Contagion

**Source:** <https://open.kattis.com/problems/chemtechcontagion>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <p>Singed has a busy day of experiments in his lab ahead of
    him. He designed his lab to be a building of <span class=
    "tex2jax_process">$N$</span> rooms and <span class=
    "tex2jax_process">$N - 1$</span> corridors, such that there
    exists exactly one simple path between any two rooms. Singed’s
    lab also has <span class="tex2jax_process">$M$</span>
    undirected ventilation pipes, where each pipe connects two
    rooms and allows gas to flow between them.</p>
    <p>Singed has a list of <span class=
    "tex2jax_process">$Q$</span> tasks he will perform to set up
    his experiments. In each task, he needs to transport a lot of
    noxious chemicals in between rooms. Each room has a storage
    cabinet. To transport chemicals from room <span class=
    "tex2jax_process">$a$</span> to room <span class=
    "tex2jax_process">$b$</span>, Singed will enter room
    <span class="tex2jax_process">$a$</span>, remove the chemicals
    from the storage cabinet, and then walk through the corridors
    to room <span class="tex2jax_process">$b$</span>, before
    putting the chemicals in room <span class=
    "tex2jax_process">$b$</span>’s storage cabinet. During the
    transport, the chemicals are not securely stored, so they
    release toxic gas, which enters any room Singed carries them
    through. The rooms have doors that prevent gas from spreading
    through the corridors themselves. However, the gas can still
    escape through the ventilation pipes. When toxic gas is in a
    room, the gas will enter any open ventilation pipe connected to
    the room, and flood the opposing room.</p>
    <p>Singed knows exactly which chemicals he needs to transport,
    and the path he needs to take to get there. But after he
    transports these chemicals, he must clean every room that may
    be exposed to the toxic gas. This includes any room he walks
    through, including his starting and destination rooms. It also
    includes any room that is connected to a contaminated room by
    an open ventilation pipe. Singed is trying to figure out how
    can he minimize the amount of cleaning up he has to do later.
    Before performing any of the tasks, Singed can close some of
    the ventilation pipes. What is the minimum number of
    ventilation pipes Singed must close to ensure that the toxic
    gas contaminates the smallest possible number of rooms?</p>
    <h2>Input</h2>
    <p>The first line of input contains three integers <span class=
    "tex2jax_process">$N$</span>, <span class=
    "tex2jax_process">$M$</span>, and <span class=
    "tex2jax_process">$Q$</span> (<span class="tex2jax_process">$1
    \leq N \leq 10^5, 0 \leq M \leq 5 \cdot 10^5, 1 \leq Q \leq 5
    \cdot 10^5$</span>), representing the number of rooms, the
    number of ventilation pipes, and the number of tasks Singed
    will perform, respectively. The next <span class=
    "tex2jax_process">$N - 1$</span> lines describe the corridors
    in Singed’s lab. Each line contains two integers <span class=
    "tex2jax_process">$u$</span> and <span class=
    "tex2jax_process">$v$</span> (<span class="tex2jax_process">$1
    \leq u, v \leq N$</span>), describing a corridor between room
    <span class="tex2jax_process">$u$</span> and room <span class=
    "tex2jax_process">$v$</span>. It is guaranteed that the
    corridors form a tree. The next <span class=
    "tex2jax_process">$M$</span> lines describe the ventilation
    pipes in Singed’s lab. Each line contains two integers
    <span class="tex2jax_process">$u$</span> and <span class=
    "tex2jax_process">$v$</span> (<span class="tex2jax_process">$1
    \leq u, v \leq N, u \neq v$</span>), describing a ventilation
    pipe between room <span class="tex2jax_process">$u$</span> and
    room <span class="tex2jax_process">$v$</span>. No ventilation
    pipe connects a room to itself, but there are no additional
    restrictions on the arrangement of the ventilation pipes. The
    next <span class="tex2jax_process">$Q$</span> lines describe
    the tasks Singed will perform. Each line contains two integers
    <span class="tex2jax_process">$a$</span> and <span class=
    "tex2jax_process">$b$</span> (<span class="tex2jax_process">$1
    \leq a, b \leq N, a \neq b$</span>), describing a task where
    Singed will transport chemicals from room <span class=
    "tex2jax_process">$a$</span> to room <span class=
    "tex2jax_process">$b$</span>. Each query should be processed
    independently of all others.</p>
    <h2>Output</h2>
    <p>For each task, output a single integer, representing the
    number of ventilation pipes Singed must close to minimize the
    amount of cleaning he has to do later.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>4 1 4
2 3
3 1
1 4
3 4
2 1
3 1
1 4
2 4
</pre>
        </td>
        <td>
          <pre>1
1
1
0
</pre>
        </td>
      </tr>
    </table>
  </div>
