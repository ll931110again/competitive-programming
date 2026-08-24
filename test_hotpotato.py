import sys

def simulate(n, k, s):
    L = 2 * n
    arr = [int(ch) for ch in s]
    for _ in range(k):
        # determine moves based on start of round
        move = [False] * L
        for i in range(L):
            if arr[i] == 1:
                nxt = (i + 1) % L
                if arr[nxt] == 0:
                    move[i] = True
        # apply moves simultaneously
        new_arr = [0] * L
        for i in range(L):
            if move[i]:
                # potato moves to nxt
                nxt = (i + 1) % L
                new_arr[nxt] = 1
            else:
                if arr[i] == 1:
                    new_arr[i] = 1
        arr = new_arr
    # count scores
    red = 0  # red team (odd indices 1-based -> 0-index even? Wait: leprechaun numbers 1..2n, odd numbers Red Team.
    # 0-index: index 0 corresponds to leprechaun 1 (odd) -> Red.
    # index 1 -> leprechaun 2 (even) -> Blue.
    # So Red indices are even (0,2,4,...), Blue indices odd (1,3,5,...)
    for idx, val in enumerate(arr):
        if val == 1:
            if idx % 2 == 0:
                red += 1
            else:
                # blue
                pass
    blue = sum(arr) - red
    return red, blue

def brute(n, k, s):
    return simulate(n, k, s)

# test small
for n in range(1, 4):
    L = 2 * n
    for k in range(0, 4):
        # iterate all strings? too many for n=3 L=8 => 256 okay
        for mask in range(1 << L):
            s = ''.join('1' if (mask >> i) & 1 else '0' for i in range(L))
            r,b = simulate(n, k, s)
            # just print some
            # print(n,k,s,r,b)
        pass
print("done")