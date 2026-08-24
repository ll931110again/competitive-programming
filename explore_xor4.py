def s1_values(x, max_k=5000):
    vals = set()
    for k in range(max_k):
        y = x ^ (k * x)
        if y > 0:
            vals.add(y)
    return sorted(vals)

def find_minimal_period(x, max_val=2000):
    vals = s1_values(x, 2000)
    vals = [v for v in vals if v <= max_val]
    vals.sort()

    # The set S1(x) should be a union of arithmetic progressions
    # Find the minimal period P such that S1(x) is a union of residue classes mod P
    # i.e., if y in S1 and y+P <= max_val, then y+P in S1

    for P in range(1, 500):
        # Check if S1 is P-periodic in the sense that it's a union of residue classes mod P
        residues = set(v % P for v in vals)
        ok = True
        for v in vals:
            if v + P <= max_val:
                if (v + P) % P in residues:
                    # Should be in S1
                    if (v + P) not in vals:
                        ok = False
                        break
                else:
                    ok = False
                    break
        # Also check that no extra values are in S1
        # Generate all numbers <= max_val with residues in 'residues'
        expected = set()
        for r in residues:
            for m in range(0, max_val//P + 2):
                val = r + m*P
                if 1 <= val <= max_val:
                    expected.add(val)
        if set(vals) == expected:
            return P, sorted(residues)
    return None, None

for x in range(1, 21):
    period, residues = find_minimal_period(x, 500)
    if period:
        print(f"x={x:2d} ({bin(x):>6}): period={period:3d}, residues={residues}")
    else:
        print(f"x={x:2d} ({bin(x):>6}): no period found")