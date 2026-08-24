def s1_values(x, max_k=1000):
    vals = []
    for k in range(max_k):
        y = x ^ (k * x)
        if y > 0:
            vals.append(y)
    return vals

def find_period(x, max_val=10000):
    """Try to find periodicity in S1(x)"""
    vals = s1_values(x, 2000)
    vals = [v for v in vals if v <= max_val]
    vals.sort()
    # Look for arithmetic progression structure
    # Group by value mod some candidate period
    for period in range(x, max_val):
        if period > x * 20:
            break
        groups = {}
        for v in vals:
            r = v % period
            if r not in groups:
                groups[r] = []
            groups[r].append(v)
        # Check if each group is an arithmetic progression with difference = period
        ok = True
        for r, lst in groups.items():
            lst.sort()
            for i in range(1, len(lst)):
                if lst[i] - lst[i-1] != period:
                    ok = False
                    break
            if not ok:
                break
        if ok and len(groups) > 1:
            residues = sorted(groups.keys())
            # Check if it covers all values
            return period, residues
    return None, None

for x in [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20]:
    period, residues = find_period(x, 500)
    if period:
        print(f"x={x:2d} ({bin(x):>6}): period={period:3d}, residues={residues}")
    else:
        print(f"x={x:2d} ({bin(x):>6}): no period found")