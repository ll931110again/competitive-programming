import random

def count_brute(x, m):
    cnt = 0
    for y in range(1, m + 1):
        xor_val = x ^ y
        if xor_val % x == 0 or xor_val % y == 0:
            cnt += 1
    return cnt

def count_fast(x, m):
    # Power of 2
    if (x & (x - 1)) == 0:
        f = 1
        P = x
        residues = {0}
    elif x in {17, 257, 65537}:
        f = 1
        P = x
        residues = {0}
    else:
        lowest = (x & -x).bit_length() - 1
        highest = x.bit_length() - 1
        span = highest - lowest + 1
        f = 1 << span
        P = x * f
        T = f
        residues = set()
        for k in range(T):
            y = x ^ (k * x)
            residues.add(y % P)

    count = 0
    for r in residues:
        if r == 0:
            count += m // P
        else:
            if r <= m:
                count += (m - r) // P + 1

    # S2
    for y in range(1, x + 1):
        if y > m:
            continue
        if (x ^ y) % y == 0 and (x ^ y) % x != 0:
            count += 1

    return count

# Test random cases
random.seed(42)
for _ in range(1000):
    x = random.randint(1, 50)
    m = random.randint(1, 200)
    b = count_brute(x, m)
    f = count_fast(x, m)
    if b != f:
        print(f"Mismatch: x={x}, m={m}, brute={b}, fast={f}")
        # Debug
        if x == 15:
            print("  x=15 special case")
        break
else:
    print("All 1000 random tests passed!")

# Test edge cases
print("\nEdge cases:")
for x in [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]:
    for m in [1, 2, 3, 4, 5, 10, 20, 50, 100]:
        b = count_brute(x, m)
        f = count_fast(x, m)
        if b != f:
            print(f"Mismatch: x={x}, m={m}, brute={b}, fast={f}")
print("Done")