# Popcorn Button

**Source:** <https://open.kattis.com/problems/popcornbutton>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <p>Caitlyn is optimizing her morning routine. First thing each
    morning, she heats up her breakfast in her new hextech
    microwave oven. Caitlyn is a very precise person. <b class=
    "bfseries">When she presses a button, she releases the button
    at exactly the end of the second. The button takes effect
    exactly at the end of this second.</b></p>
    <p>Initially, the microwave is <i class="itshape">idle</i> and
    has no time entered or remaining. When the microwave is
    <i class="itshape">idle</i>, Caitlyn can start the microwave by
    entering a time with the <tt class="ttfamily">Set Time</tt>
    button. To select a time, Caitlyn must press the <tt class=
    "ttfamily">Set Time</tt> button on the microwave, then enter
    the time into the microwave’s keypad, then press the <tt class=
    "ttfamily">Start</tt> button. Caitlyn can enter a one, two or
    three digit time. If Caitlyn enters a one or two digit time,
    the microwave will run for this many seconds. If Caitlyn enters
    a three digit time, the microwave will interpret the first
    digit as minutes and the last two digits as seconds. There are
    no restrictions on which digits Caitlyn can enter. For example,
    if Caitlyn enters <tt class="ttfamily">Set Time-9-3-Start</tt>,
    the microwave will run for 93 seconds. If Caitlyn enters
    <tt class="ttfamily">Set Time-6-9-3-Start</tt>, the microwave
    will run for 6 minutes and 93 seconds, for a total of
    <span class="tex2jax_process">$360 + 93 = 453$</span> seconds.
    If Caitlyn enters <tt class="ttfamily">Set
    Time-0-0-0-Start</tt>, the microwave will run for 0 seconds and
    stop instantly. Caitlyn cannot enter more than 3 digits.</p>
    <p>Additionally, the microwave has several <i class=
    "itshape">preset buttons</i> that Caitlyn can use to
    immediately start the microwave while the microwave is
    <i class="itshape">idle</i>.</p>
    <ul class="itemize">
      <li>
        <p>If Caitlyn presses any digit <tt class="ttfamily">1</tt>
        through <tt class="ttfamily">7</tt> while the microwave is
        <i class="itshape">idle</i> (and <tt class="ttfamily">Set
        Time</tt> has not been pressed), the microwave will run for
        that many minutes, without Caitlyn pressing <tt class=
        "ttfamily">Set Time</tt> or <tt class=
        "ttfamily">Start</tt>. Pressing <tt class=
        "ttfamily">8</tt>, <tt class="ttfamily">9</tt> or
        <tt class="ttfamily">0</tt> before pressing <tt class=
        "ttfamily">Set Time</tt> has no effect.</p>
      </li>
      <li>
        <p>If Caitlyn presses the <tt class="ttfamily">Popcorn</tt>
        button, at the end of the second, the microwave will begin
        running for 2 minutes and 30 seconds.</p>
      </li>
      <li>
        <p>If Caitlyn presses the <tt class="ttfamily">Reheat</tt>
        button, the microwave will begin running for 50
        seconds.</p>
      </li>
      <li>
        <p>If Caitlyn presses the <tt class="ttfamily">Potato</tt>
        button, the microwave will begin running for 5 minutes and
        15 seconds.</p>
      </li>
      <li>
        <p>If Caitlyn presses the <tt class="ttfamily">Add 30
        Seconds</tt> button before pressing any other buttons, the
        microwave will begin running for 30 seconds.</p>
      </li>
    </ul>
    <p>No <i class="itshape">preset button</i> except the
    <tt class="ttfamily">Add 30 Seconds</tt> button has any effect
    when the microwave is <i class="itshape">running</i> or
    <i class="itshape">paused</i>. No <i class="itshape">preset
    button</i> except the digits has any effect when Caitlyn is
    entering a time with the <tt class="ttfamily">Set Time</tt>
    button.</p>
    <p>Caitlyn can also press the <tt class="ttfamily">Stop</tt>
    button to pause the microwave while it is <i class=
    "itshape">running</i>. While <i class="itshape">paused</i>, the
    microwave will not run, and the time will not reset.</p>
    <ul class="itemize">
      <li>
        <p>If Caitlyn presses the <tt class="ttfamily">Stop</tt>
        button while the microwave is <i class=
        "itshape">running</i>, it will stop cooking at the end of
        the second.</p>
      </li>
      <li>
        <p>While the microwave is <i class="itshape">paused</i>,
        only the <tt class="ttfamily">Start</tt>, <tt class=
        "ttfamily">Add 30 Seconds</tt> and <tt class=
        "ttfamily">Stop</tt> buttons have any effect: pressing the
        <tt class="ttfamily">Stop</tt> button resets the time to 0
        seconds and cancels the cooking operation. Pressing the
        <tt class="ttfamily">Start</tt> button resumes cooking with
        the remaining time.</p>
      </li>
      <li>
        <p>Caitlyn can run the hextech microwave without her food
        in it. When the microwave is not <i class=
        "itshape">running</i>, she can instantly insert or remove
        food into the microwave. She cannot insert or remove food
        while the microwave is <i class="itshape">running</i>.</p>
      </li>
    </ul>
    <p>When the microwave is <i class="itshape">running</i>, only
    the <tt class="ttfamily">Stop</tt> and <tt class="ttfamily">Add
    30 Seconds</tt> buttons have any effect. If Caitlyn presses the
    <tt class="ttfamily">Stop</tt> button when the microwave is
    <i class="itshape">running</i> with <span class=
    "tex2jax_process">$t$</span> seconds remaining, at the end of
    the second, the microwave will stop cooking with <span class=
    "tex2jax_process">$t - 1$</span> seconds remaining. If Caitlyn
    presses the <tt class="ttfamily">Stop</tt> button when the
    microwave has only one second left, this second will tick down,
    and the microwave will become <i class="itshape">idle</i>
    instead of remaining <i class="itshape">paused</i> with
    <span class="tex2jax_process">$0$</span> seconds. If
    <span class="tex2jax_process">$t &gt; 1$</span>, the microwave
    will remain <i class="itshape">paused</i> with <span class=
    "tex2jax_process">$t - 1$</span> seconds remaining.</p>
    <p>Caitlyn can use the <tt class="ttfamily">Add 30 Seconds</tt>
    whenever the microwave is <i class="itshape">running</i>,
    <i class="itshape">paused</i> or <i class="itshape">idle</i>,
    but not when a time is being entered with the <tt class=
    "ttfamily">Set Time</tt> button. If Caitlyn presses the
    <tt class="ttfamily">Add 30 Seconds</tt> button when the
    microwave is <i class="itshape">running</i> with <span class=
    "tex2jax_process">$t \leq 610$</span> seconds remaining, the
    microwave will run for this second, and at the end of the
    second, the microwave will have <span class=
    "tex2jax_process">$t + 29$</span> seconds remaining. If Caitlyn
    presses the <tt class="ttfamily">Add 30 Seconds</tt> when the
    microwave is <i class="itshape">running</i> with 1 second
    remaining, the microwave will continue running and read
    <span class="tex2jax_process">$t + 29 = 30$</span> seconds
    remaining at the end of this second. If Caitlyn presses the
    <tt class="ttfamily">Add 30 Seconds</tt> button when the
    microwave is <i class="itshape">paused</i> with <span class=
    "tex2jax_process">$t \leq 609$</span> seconds remaining, the
    microwave will remain <i class="itshape">paused</i> with
    <span class="tex2jax_process">$t + 30$</span> seconds
    remaining. If Caitlyn presses the <tt class="ttfamily">Add 30
    Seconds</tt> button when the microwave is <i class=
    "itshape">idle</i>, the microwave will begin running for 30
    seconds.</p>
    <p>The microwave can only display 3 digits. When Caitlyn
    presses the <tt class="ttfamily">Add 30 Seconds</tt> button,
    the microwave will automatically normalize the time to 3
    digits. If the resulting time would be too large to display
    with only 3 digits, the <tt class="ttfamily">Add 30
    Seconds</tt> button will have no effect for that second. For
    example, if Caitlyn hits <tt class="ttfamily">Add 30
    Seconds</tt> when the microwave is <i class=
    "itshape">running</i> and reads <tt class="ttfamily">0:99</tt>,
    the resulting cook time will be <span class=
    "tex2jax_process">$99 + 29 = 128$</span> seconds. The microwave
    will display this as <tt class="ttfamily">2:08</tt> (2 minutes
    and 8 seconds). If Caitlyn presses the <tt class="ttfamily">Add
    30 Seconds</tt> button when the microwave has 9 minutes and 71
    seconds remaining or longer, the resulting time is too large to
    display, and the button has no effect. If Caitlyn presses the
    <tt class="ttfamily">Add 30 Seconds</tt> button when the
    microwave is <i class="itshape">running</i> and reads
    <tt class="ttfamily">9:70</tt>, the resulting cook time will be
    <span class="tex2jax_process">$9 \times 60 + 70 + 29 =
    639$</span> seconds, and the microwave will display this as
    <tt class="ttfamily">9:99</tt> (9 minutes and 99 seconds).</p>
    <p>For example, to heat her food 90 seconds, Caitlyn use any
    one of the following strategies:</p>
    <ul class="itemize">
      <li>
        <p><tt class="ttfamily">Set Time-9-0-Start</tt> (4 seconds
        spent pressing buttons)</p>
      </li>
      <li>
        <p><tt class="ttfamily">Set Time-1-3-0-Start</tt> (5
        seconds)</p>
      </li>
      <li>
        <p><tt class="ttfamily">1-Add30</tt> (2 seconds)</p>
      </li>
      <li>
        <p><tt class="ttfamily">Set Time-1-0-0-Start-Add30</tt> (6
        seconds)</p>
      </li>
      <li>
        <p><tt class="ttfamily">Add30-Add30-Add30</tt> (3
        seconds)</p>
      </li>
      <li>
        <p><tt class="ttfamily">Reheat (without food)-Add30-(Wait
        19 seconds for microwave to count down)<br>
        -Stop-Add30-(Put food in instantly)-Start</tt> (24
        seconds)</p>
      </li>
    </ul>
    <p>Making Caitlyn’s life more difficult, Vi always presses the
    buttons too hard and breaks them. A lot of the buttons aren’t
    even working right now. Caitlyn can’t use any of the broken
    buttons to enter the time.</p>
    <p>Caitlyn needs to heat her food today for exactly
    <span class="tex2jax_process">$x$</span> seconds. What is the
    minimum number of seconds for Caitlyn to enter this time into
    the microwave? Her time starts when she presses the first
    button, and ends when she presses the last button, regardless
    of how long the food has left.</p>
    <h2>Input</h2>
    <p>The first line contains two space-separated integers:
    <span class="tex2jax_process">$n$</span>, the number of buttons
    that are broken (<span class="tex2jax_process">$0 \leq n \leq
    15$</span>), and <span class="tex2jax_process">$q$</span>, the
    number of times Caitlyn wants to heat her food (<span class=
    "tex2jax_process">$1 \leq q \leq 639$</span>).</p>
    <p>The next line contains <span class=
    "tex2jax_process">$n$</span> space-separated strings,
    representing the button that is broken. The digit buttons are
    represented by a single character, <tt class=
    "ttfamily">0-9</tt>, the digit of the button that is broken.
    This line will be empty if <span class="tex2jax_process">$n =
    0$</span>. The remaining buttons are represented by the
    following strings:</p>
    <ul class="itemize">
      <li>
        <p><tt class="ttfamily">settime</tt></p>
      </li>
      <li>
        <p><tt class="ttfamily">popcorn</tt></p>
      </li>
      <li>
        <p><tt class="ttfamily">reheat</tt></p>
      </li>
      <li>
        <p><tt class="ttfamily">potato</tt></p>
      </li>
      <li>
        <p><tt class="ttfamily">add30</tt></p>
      </li>
    </ul>
    <p>The <tt class="ttfamily">Stop</tt> and <tt class=
    "ttfamily">Start</tt> buttons are never broken.</p>
    <p>The next line contains <span class=
    "tex2jax_process">$q$</span> space-separated integers,
    <span class="tex2jax_process">$x_1, x_2, \ldots , x_q$</span>
    (<span class="tex2jax_process">$1 \leq x_i \leq 639$</span>),
    the times Caitlyn wants to heat her food for over the next
    <span class="tex2jax_process">$q$</span> days.</p>
    <h2>Output</h2>
    <p>For each query, output one line containing a single number,
    the minimum number of seconds Caitlyn needs to enter the time
    into the microwave to cook her food <i class=
    "itshape">exactly</i> <span class=
    "tex2jax_process">$x_i$</span> seconds. This is defined as the
    final time when Caitlyn presses the last button, regardless of
    how long the food has left to cook. If Caitlyn cannot cook her
    food for exactly <span class="tex2jax_process">$x_i$</span>
    seconds, output <tt class="ttfamily">-1</tt>.</p>
    <h2>Explanation</h2>
    <p>Unless otherwise specified, in the below explanations, if
    Caitlyn starts the microwave, her food is assumed to be in the
    microwave.</p>
    <p>To cook her food for <tt class="ttfamily">60</tt> seconds,
    Caitlyn could use any of the three following strategies in the
    sample inputs:</p>
    <ul class="itemize">
      <li>
        <p><tt class="ttfamily">1</tt> (1 second)</p>
      </li>
      <li>
        <p><tt class="ttfamily">Popcorn-(wait 9
        seconds)-Stop-Stop-Reheat</tt> (13 seconds)</p>
      </li>
      <li>
        <p><tt class="ttfamily">2-(wait 59 seconds)-Stop</tt> (61
        seconds)</p>
      </li>
    </ul>
    <p>To cook her food for <tt class="ttfamily">1</tt> second,
    Caitlyn can press any preset button during the first second and
    then hit <tt class="ttfamily">Stop</tt> during the second
    second. The microwave will remain <i class="itshape">paused</i>
    and her food will have cooked for 1 second.</p>
    <p>To cook her food for <tt class="ttfamily">23</tt> seconds,
    Caitlyn could use any of the three following strategies in the
    sample inputs:</p>
    <ul class="itemize">
      <li>
        <p><tt class="ttfamily">Set Time-2-3-Start</tt> (4
        seconds)</p>
      </li>
      <li>
        <p><tt class="ttfamily">Add30 (without food)-(wait 6
        seconds)-Stop-Start (with food)</tt> (9 seconds)</p>
      </li>
      <li>
        <p><tt class="ttfamily">Potato-Wait 22 seconds-Stop</tt>
        (24 seconds)</p>
      </li>
    </ul>
    <p>To cook her food for <tt class="ttfamily">80</tt> seconds,
    Caitlyn could use any of the three following strategies in the
    sample inputs:</p>
    <ul class="itemize">
      <li>
        <p><tt class="ttfamily">Reheat-Add30</tt> (2 seconds)</p>
      </li>
      <li>
        <p><tt class="ttfamily">4-(wait 29
        seconds)-Stop-Stop-Reheat</tt> (33 seconds)</p>
      </li>
      <li>
        <p><tt class="ttfamily">2 (without food)-(wait 39
        seconds)-Stop-Start (with food)</tt> (42 seconds)</p>
      </li>
    </ul>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>0 8

60 1 23 80 30 90 120 180
</pre>
        </td>
        <td>
          <pre>1
2
4
2
1
2
1
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
          <pre>3 8
1 settime add30
60 1 23 80 30 90 120 180
</pre>
        </td>
        <td>
          <pre>13
2
24
33
22
32
1
1
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
          <pre>4 8
1 settime add30 reheat
60 1 23 80 30 90 120 180
</pre>
        </td>
        <td>
          <pre>61
2
24
42
31
32
1
1
</pre>
        </td>
      </tr>
    </table>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 4</th>
        <th>Sample Output 4</th>
      </tr>
      <tr>
        <td>
          <pre>2 8
2 3
60 1 23 80 30 90 120 180
</pre>
        </td>
        <td>
          <pre>1
2
9
2
1
2
3
2
</pre>
        </td>
      </tr>
    </table>
  </div>
