import sys

# Fermat primes > 5 where S1(x) = all multiples of x
FERMAT_PRIMES_SPECIAL = {17, 257, 65537}

def solve():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    t = int(next(it))
    out = []
    for _ in range(t):
        x = int(next(it))
        m = int(next(it))

        # Compute S2: y in [1, x] such that (x^y) % y == 0
        s2_list = []
        for y in range(1, x + 1):
            if (x ^ y) % y == 0:
                s2_list.append(y)

        # Compute S1 periodic structure
        if (x & (x - 1)) == 0:  # power of 2
            f = 1
            P = x
            residues = {0}
        elif x in FERMAT_PRIMES_SPECIAL:
            f = 1
            P = x
            residues = {0}
        else:
            # Find lowest and highest set bit
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

        # Count S1 up to m
        count = 0
        for r in residues:
            if r == 0:
                count += m // P
            else:
                if r <= m:
                    count += (m - r) // P + 1

        # Add S2 elements not in S1 and <= m
        for y in s2_list:
            if y > m:
                continue
            # Check if y is in S1: (x^y) % x == 0
            if (x ^ y) % x != 0:
                count += 1

        out.append(str(count))

    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    solve()