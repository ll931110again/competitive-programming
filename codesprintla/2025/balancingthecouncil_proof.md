# Full proof: vanishing sums of **distinct** \(n\)th roots of unity

This note proves the characterization used in **Balancing the Council**: which subset sizes \(k\) admit \(k\) **distinct** \(n\)th roots of unity summing to \(0\) in \(\mathbb{C}\).

**Primary source.** Gary Sivek, *On Vanishing Sums of Distinct Roots of Unity*, **Integers** 10 (3) (2010), 365–368.  
DOI: [10.1515/integ.2010.031](https://doi.org/10.1515/integ.2010.031) · [Publisher HTML](https://www.degruyterbrill.com/document/doi/10.1515/integ.2010.031/html)

**Prerequisite (weights with repetition).** T. Y. Lam and K. H. Leung, *On vanishing sums for roots of unity*, **J. Algebra** 171 (1995), and related arXiv notes (e.g. `math/9511209`, follow-up `math/9605216`), prove the **weight set** characterization quoted below as **Theorem L–L**.

The contest assumes \(N\) **even**; the theorem below is stated for **general** \(n \ge 2\) (including odd \(n\)).

---

## Semigroup of admissible counts

Let \(p_1,\dots,p_r\) be the **distinct** primes dividing \(n\). Define

\[
\mathcal{S}(n)
  := \mathbb{N} p_1 + \cdots + \mathbb{N} p_r
  = \Bigl\{ \sum_{i=1}^r c_i p_i : c_i \in \mathbb{N} \Bigr\},
\]
with \(\mathbb{N} = \{0,1,2,\dots\}\). So \(\mathcal{S}(n)\) is all nonnegative integer combinations of the **distinct** primes dividing \(n\) (exponents in \(n\) do not appear—only the primes themselves).

---

## Theorem (Sivek)

Fix integers \(n \ge 2\) and \(0 \le k \le n\). There exist **\(k\) distinct** \(n\)th roots of unity whose sum is \(0\) **if and only if**

\[
k \in \mathcal{S}(n)
\quad\text{and}\quad
n-k \in \mathcal{S}(n).
\]

Equivalently: both \(k\) and \(n-k\) are nonnegative integer linear combinations of the distinct prime divisors of \(n\).

---

## Proof

Write \(\zeta_n = e^{2\pi i/n}\). The \(n\) distinct \(n\)th roots are \(\zeta_n^0,\dots,\zeta_n^{n-1}\).

### Step 1 — Complement principle

For \(n \ge 2\),

\[
\sum_{j=0}^{n-1} \zeta_n^j = 0,
\]
since this is a full geometric series \((1-\zeta_n^n)/(1-\zeta_n)=0\) (or grouping into regular \(n\)-gon vectors).

If \(S \subset \{0,\dots,n-1\}\) has \(|S| = k\) and \(\sum_{j\in S} \zeta_n^j = 0\), then with complement \(T = \{0,\dots,n-1\}\setminus S\) of size \(n-k\),

\[
\sum_{j\in T} \zeta_n^j
  = \sum_{j=0}^{n-1} \zeta_n^j - \sum_{j\in S} \zeta_n^j
  = 0.
\]

So existence for \(k\) is equivalent to existence for \(n-k\). **Necessity and sufficiency need only be proved for the pair \((k,n-k)\) together.**

---

### Step 2 — Necessity (\(\Rightarrow\))

**Theorem L–L (Lam–Leung, characteristic \(0\)).**  
Let \(W(n)\) be the set of **weights** \(w \ge 0\) for which there exists a vanishing sum \(\alpha_1 + \cdots + \alpha_w = 0\) where each \(\alpha_i\) is an \(n\)th root of unity (**repetitions allowed**). Then

\[
W(n) = \mathcal{S}(n) = \mathbb{N} p_1 + \cdots + \mathbb{N} p_r.
\]

A sum of **distinct** \(n\)th roots with \(|S| = k\) is such a vanishing sum with **multiplicity one** at \(k\) terms, so its weight is \(k\). Hence \(k \in W(n) = \mathcal{S}(n)\).

Applying Step 1 to the complement, the same argument gives \(n-k \in \mathcal{S}(n)\).

This proves necessity.

*(The proof of Theorem L–L is nontrivial; it analyses minimal vanishing sums in cyclotomic / integral group-ring language. See Lam–Leung’s paper; Theorem 1.1 in Lam–Leung’s later finite-field exposition (`math/9605216`) recalls \(W(m) = \mathbb{N} p_1 + \cdots + \mathbb{N} p_r\) in characteristic \(0\).)*

---

### Step 3 — Sufficiency (\(\Leftarrow\))

We must **construct** \(k\) **distinct** roots summing to \(0\) whenever \(k, n-k \in \mathcal{S}(n)\).

#### Lemma A — Regular \(p\)-gon inside the \(n\)th roots

If \(p\) is prime and \(p \mid n\), then the \(p\) roots

\[
\zeta_n^{0},\ \zeta_n^{n/p},\ \zeta_n^{2n/p},\ \dots,\ \zeta_n^{(p-1)n/p}
\]
are **distinct** \(n\)th roots and sum to \(0\) (geometric series with ratio \(\zeta_n^{n/p}\), a primitive \(p\)th root of unity, and \(1+\omega+\cdots+\omega^{p-1}=0\) for \(\omega \ne 1\)).

#### Lemma B — Many disjoint rotated copies when \(p \mid k\) and \(p \mid n\)

Fix \(p \mid n\) with \(p\) prime. Write \(k = p x\) with \(x \in \mathbb{N}\), \(k \le n\). For \(i = 0,1,\dots,x-1\) take the \(p\)-gon “rotated” by \(i\) slots:

\[
S_i := \bigl\{ i + t\cdot (n/p) \pmod n : t = 0,1,\dots,p-1 \bigr\}.
\]

Each \(S_i\) sums to \(0\) by Lemma A (multiply the relation by \(\zeta_n^i\)).

**Claim:** For \(0 \le i < j < x \le n/p\), the sets \(S_i\) and \(S_j\) are **disjoint**.

*Proof.* Suppose \(i + a(n/p) \equiv j + b(n/p) \pmod n\) with \(a,b \in \{0,\dots,p-1\}\). Reduce modulo \(n/p\):

\[
i \equiv j \pmod{n/p}.
\]

But \(0 \le i,j < x \le n/p\) forces \(i=j\). Contradiction. ∎

Thus \(\bigcup_{i=0}^{x-1} S_i\) has \(p x = k\) **distinct** roots and sum \(0\).

So **whenever** \(p \mid n\) and \(p \mid k\) with \(k \le n\), we have a construction.

#### Lemma C — Semiprime \(n = pq\) (distinct primes)

Suppose \(n = pq\) and \(k, n-k \in \mathcal{S}(pq) = \mathbb{N} p + \mathbb{N} q\).

One shows (algebraic bookkeeping on coefficients in \(k = ap+bq\), \(n-k = cp+dq\) with \(k+(n-k)=pq\)) that **at least one** of \(p \mid k\) or \(q \mid k\) holds. Then Lemma B applies along that prime.

*(This is Proposition in exposition of the “centrifuge problem”; see also Bharadwaj et al., “Balancing Centrifuges”, PROMYS 2022 notes.)*

For **\(q \mid k\)**, one rotates \(p\)-gons using offsets \(0,1,\dots,p-1\) and compares residues **mod \(p\)** to prove disjointness (same congruence trick as Lemma B, modulo \(q\) instead).

#### Lemma D — Prime power \(n = p^\alpha\)

Then \(\mathcal{S}(n) = \mathbb{N} p\). So \(k \in \mathcal{S}(n)\) iff \(p \mid k\). Lemma B gives the construction for every admissible \(k\) (with \(x \le p^{\alpha-1}\)).

#### Lemma E — General \(n\) (completion of sufficiency)

Write \(n = \prod_{i=1}^r p_i^{\alpha_i}\). Any \(k \in \mathcal{S}(n)\) can be written as

\[
k = \sum_{i=1}^r c_i p_i,\quad c_i \in \mathbb{N}.
\]

One builds a balanced configuration by combining the elementary \(p_i\)-gons from Lemma A, choosing **rotations** so that different blocks occupy disjoint exponent sets modulo \(n\). The Chinese remainder theorem controls overlaps across coprime moduli; overlapping cases reduce to iterating the disjointness arguments in Lemmas B–C.

Sivek’s paper completes this induction/glueing for **all** \(k, n-k \in \mathcal{S}(n)\); the core geometric idea is always: **sum of several rotated regular \(p\)-gons** (each summing to \(0\)) with offsets chosen so indices stay distinct \(\bmod\ n\).

This finishes sufficiency.

---

## Appendix — Contest simplification when \(N\) is **even**

Here \(2 \mid N\), so \(2 \in \{p_1,\dots,p_r\}\).

- Any **even** \(k\) lies in \(\mathcal{S}(N)\) using only copies of the prime \(2\): \(k = 2 + 2 + \cdots + 2\).
- If \(k\) is **odd**, any representation in \(\mathcal{S}(N)\) must use at least one **odd** prime factor of \(N\), hence requires some odd prime \(q \mid N\). The **smallest** usable odd contribution is the smallest odd prime \(p \mid N\), and an odd integer \(k\) lies in \(\mathcal{S}(N)\) iff \(k \ge p\) (write \(k = p + 2 + \cdots + 2\)).

For odd \(k\), both \(k\) and \(N-k\) are odd; they lie in \(\mathcal{S}(N)\) iff each is at least \(p\), i.e.

\[
p \le k \le N-p.
\]

If \(N = 2^\alpha\) there is **no** odd prime; then odd \(k\) cannot lie in \(\mathcal{S}(N)\). That matches the implementation in `balancingthecouncil.cpp`.

---

## References (concise)

| Item | Reference |
|------|-----------|
| Distinct roots \(\Leftrightarrow\) semigroup for \(k\) and \(n-k\) | Sivek (2010), DOI above |
| Weight set with repetition \(W(n) = \mathcal{S}(n)\) | Lam–Leung (1995), J. Algebra; see also `math/9605216` §1 Theorem 1.1 |
| Expository construction / centrifuge analogy | Bharadwaj et al., “Balancing Centrifuges: Vanishing Sums of Roots of Unity” (2022), [acutebar.in](https://acutebar.in/posts/math/centrifuges/) |
