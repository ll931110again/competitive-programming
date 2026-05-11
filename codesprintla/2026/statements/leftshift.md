# Problem D — Left Shift

**Source:** <https://codesprintla26.kattis.com/contests/codesprintla26open/problems/leftshift>

## Statement (HTML fragment from `problembody`)

<div class="problembody">
    <div style="width:20.00%" class="illustration">
      <img src="https://codesprintla26.kattis.com/problems/leftshift/file/statement/en/img-0001.jpg"
      alt="https://codesprintla26.kattis.com/problems/leftshift/file/statement/en/img-0001.jpg"
      class="illustration">
      <div class="description">
        A Sheikah slate; attributed to Tartine and to Zelda Wiki
      </div>
    </div>
    <p>Link accidentally scanned the wrong QR Code with his Sheikah
    Slate and got ransomware on his Sheikah Slate that prevents him
    from using it. The Sheikah Slate displays a string <span class=
    "tex2jax_process">$S$</span> of length <span class=
    "tex2jax_process">$N$</span> and there’s a text field where you
    can input some numbers. You can apply the following operation
    to the string:</p>
    <ul class="itemize">
      <li>
        <p>You can select exactly one substring of <span class=
        "tex2jax_process">$S$</span> ranging from positions
        <span class="tex2jax_process">$L$</span> to <span class=
        "tex2jax_process">$R$</span> (<span class=
        "tex2jax_process">$1 \le L \le R \le N$</span>)</p>
      </li>
      <li>
        <p>Take a cyclic shift of that substring to the left by
        one</p>
      </li>
      <li>
        <p>Replace the substring from positions <span class=
        "tex2jax_process">$L$</span> to <span class=
        "tex2jax_process">$R$</span> with the cyclic shift of that
        substring</p>
      </li>
      <li>
        <p>Alternatively, you can select no such substring and
        leave the string as it is.</p>
      </li>
    </ul>
    <p>Let <span class="tex2jax_process">$Q$</span> be the length
    of the longest palindrome of even length in the string
    <span class="tex2jax_process">$S$</span> after applying the
    operation zero or one times. To unlock the Sheikah Slate, Link
    needs to find <span class="tex2jax_process">$Q$</span>,
    <span class="tex2jax_process">$L$</span>, and <span class=
    "tex2jax_process">$R$</span>. Please output <span class=
    "tex2jax_process">$Q$</span>, <span class=
    "tex2jax_process">$L$</span>, and <span class=
    "tex2jax_process">$R$</span> so that Link can use his Sheikah
    Slate again.</p>
    <h2>Input</h2>
    <p>The first line contains a single integer <span class=
    "tex2jax_process">$N \: (1 \leq N \leq 2000)$</span>, the
    length of the string <span class=
    "tex2jax_process">$S$</span>.</p>
    <p>The second line contains a string <span class=
    "tex2jax_process">$S$</span> of length <span class=
    "tex2jax_process">$N$</span>, consisting of lowercase English
    letters.</p>
    <h2>Output</h2>
    <p>Output one line containing a single integer <span class=
    "tex2jax_process">$Q$</span>, the length of the longest
    palindrome of even length that Link can attain using zero or
    one applications of the above operation. If no palindrome of
    even length can be attained, output a single integer
    <span class="tex2jax_process">$-1$</span>.</p>
    <p>If the above line is not <span class=
    "tex2jax_process">$-1$</span>, on the next line, output a
    single integer <span class="tex2jax_process">$-1$</span> if you
    do not perform any left cyclic shift to achieve <span class=
    "tex2jax_process">$Q$</span>. Otherwise, output two
    space-separated integers <span class=
    "tex2jax_process">$L$</span> and <span class=
    "tex2jax_process">$R \: (1 \leq L \leq R \leq N)$</span> such
    that a single left cyclic shift on the substring from positions
    <span class="tex2jax_process">$L$</span> to <span class=
    "tex2jax_process">$R$</span> (inclusive) will yield
    <span class="tex2jax_process">$Q$</span>.</p>
    <p>If multiple possible ways exist to achieve <span class=
    "tex2jax_process">$Q$</span>, you may output any one of
    them.</p>
    <table class="sample" summary="sample data">
      <tr>
        <th>Sample Input 1</th>
        <th>Sample Output 1</th>
      </tr>
      <tr>
        <td>
          <pre>21
xcampacademyisthebest
</pre>
        </td>
        <td>
          <pre>4
2 6
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
          <pre>10
bcdbdcdddd
</pre>
        </td>
        <td>
          <pre>6
1 3

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
          <pre>9
aaaaaaabc
</pre>
        </td>
        <td>
          <pre>6
-1
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
          <pre>3
abc
</pre>
        </td>
        <td>
          <pre>-1
</pre>
        </td>
      </tr>
    </table>
  </div>
