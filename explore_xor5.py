def compute_s1_residues(x, max_k=10000):
    """Find the periodic structure of S1(x)"""
    vals = []
    seen = {}
    for k in range(max_k):
        y = x ^ (k * x)
        if y > 0:
            vals.append(y)
        # Check for period in k: a_{k+T} = a_k + P
        # We need to find T and P

    # Try to find T by looking at differences
    for T in range(1, min(len(vals)//2, 100)):
        # Check if a_{i+T} - a_i is constant for i in some range
        diffs = []
        for i in range(len(vals) - T):
            if i + T < len(vals):
                diffs.append(vals[i+T] - vals[i])
        # If all diffs are equal from some point
        if len(diffs) >= 10:
            # Check last 10 diffs
            last_diff = diffs[-1]
            if all(d == last_diff for d in diffs[-10:]):
                P = last_diff
                # Residues are vals[0..T-1] mod P
                residues = sorted(set(v % P for v in vals[:T] if v > 0))
                return T, P, residues
    return None, None, None

for x in range(1, 33):
    T, P, residues = compute_s1_residues(x, 2000)
    if T:
        print(f"x={x:2d} ({bin(x):>7}): T={T:2d}, P={P:4d}, |R|={len(residues):2d}, P/x={P/x:.1f}, residues={residues}")
    else:
        print(f"x={x:2d} ({bin(x):>7}): not found")