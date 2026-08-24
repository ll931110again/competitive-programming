def count_valid_brute(x, m):
    count = 0
    valid = []
    for y in range(1, m + 1):
        xor_val = x ^ y
        if xor_val % x == 0 or xor_val % y == 0:
            count += 1
            valid.append(y)
    return count, valid

def analyze_small_x():
    for x in range(1, 17):
        print(f"\nx = {x} (binary: {bin(x)})")
        for m in [1, 2, 3, 4, 5, 6, 7, 8, 15, 16, 31, 32]:
            cnt, valid = count_valid_brute(x, m)
            print(f"  m={m:2d}: count={cnt:2d}, valid={valid}")

analyze_small_x()