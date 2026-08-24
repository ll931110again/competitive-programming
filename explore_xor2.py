def s1_values(x, limit=1000):
    """Return sorted list of S1(x) values up to limit"""
    vals = set()
    for k in range(0, limit//x + 100):
        y = x ^ (k * x)
        if 1 <= y <= limit:
            vals.add(y)
    return sorted(vals)

def s2_values(x, limit=1000):
    """Return sorted list of S2(x) values up to limit"""
    # y such that x = y ^ (k*y) for some k>=0
    # This is equivalent to: y divides x^y, i.e., (x^y) % y == 0
    vals = set()
    for y in range(1, limit + 1):
        if (x ^ y) % y == 0:
            vals.add(y)
    return sorted(vals)

def analyze_x(x, limit=200):
    s1 = s1_values(x, limit)
    s2 = s2_values(x, limit)
    union = sorted(set(s1) | set(s2))
    print(f"x={x} ({bin(x)})")
    print(f"  S1: {s1}")
    print(f"  S2: {s2}")
    print(f"  Union: {union}")
    print()

for x in [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]:
    analyze_x(x, 100)