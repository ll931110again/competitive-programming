def find_period_s1(x, max_k=2000):
    """Find T, P such that a_{k+T} = a_k + P for all k in a range"""
    a = []
    for k in range(max_k):
        y = x ^ (k * x)
        a.append(y)

    # We want to find T, P such that a[k+T] - a[k] == P for k = 0..L-1
    # Try T from 1 upwards
    for T in range(1, min(len(a)//2, 200)):
        # Check if a[k+T] - a[k] is constant for k = 0..T-1 (one full cycle)
        if T * 2 <= len(a):
            diffs = [a[i+T] - a[i] for i in range(T)]
            if len(set(diffs)) == 1:
                P = diffs[0]
                if P > 0:
                    # Verify for a few more
                    ok = True
                    for i in range(T, min(len(a)-T, T+10)):
                        if a[i+T] - a[i] != P:
                            ok = False
                            break
                    if ok:
                        # Get residues from first T values
                        residues = sorted(set(v % P for v in a[:T] if v > 0))
                        return T, P, residues
    return None, None, None

for x in range(1, 33):
    T, P, residues = find_period_s1(x, 2000)
    if T:
        print(f"x={x:2d} ({bin(x):>7}): T={T:2d}, P={P:4d}, |R|={len(residues):2d}, P/x={P/x:.1f}, residues={residues}")
    else:
        print(f"x={x:2d} ({bin(x):>7}): not found")