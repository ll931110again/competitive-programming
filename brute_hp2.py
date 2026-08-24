import sys
from functools import lru_cache

def solve_brute(n, k, s):
    L = 2 * n
    init = tuple(1 if ch == '1' else 0 for ch in s)

    @lru_cache(None)
    def dp(state, rounds_left):
        if rounds_left == 0:
            # red score = potatoes on blue positions (odd indices 0-based)
            return sum(state[i] for i in range(L) if i % 2 == 1)
        # Determine movable potatoes
        movable = []  # list of indices where can move
        for i in range(L):
            if state[i] == 1:
                nxt = (i + 1) % L
                if state[nxt] == 0:
                    movable.append(i)
        # Separate by team
        red_mov = [i for i in movable if i % 2 == 0]  # Red holds even indices
        blu_mov = [i for i in movable if i % 2 == 1]  # Blue holds odd indices
        # If no movable potatoes, state stays same
        if not red_mov and not blu_mov:
            return dp(state, rounds_left - 1)
        best = -1e9  # Red maximizes
        # Enumerate Red's choices: for each red movable, decide move (1) or stay (0)
        from itertools import product
        red_choices = list(product([0, 1], repeat=len(red_mov)))
        blue_choices = list(product([0, 1], repeat=len(blu_mov)))
        for rch in red_choices:
            # For this red choice, compute worst-case (min) over blue choices
            min_val = 1e9
            for bch in blue_choices:
                # compute next state
                new_state = [0] * L
                # first copy stays for non-movable potatoes and those that stay
                for i in range(L):
                    if state[i] == 1:
                        # check if i is movable and decision to move
                        if i in movable:
                            if i % 2 == 0:  # red
                                idx = red_mov.index(i)
                                move = rch[idx] == 1
                            else:  # blue
                                idx = blu_mov.index(i)
                                move = bch[idx] == 1
                            if move:
                                nxt = (i + 1) % L
                                new_state[nxt] = 1
                            else:
                                new_state[i] = 1
                        else:
                            # not movable, must stay
                            new_state[i] = 1
                # potatoes that were 0 stay 0 (already zero)
                val = dp(tuple(new_state), rounds_left - 1)
                if val < min_val:
                    min_val = val
            if min_val > best:
                best = min_val
        return best

    return dp(init, k)

# test examples
tests = [
    (2, 1, "1000", (1,0)),
    (2, 1, "0011", (0,2)),
    (3, 2, "101110", (3,1)),
    (5, 100000, "1111111111", (5,5)),
    (5, 100000, "0000000000", (0,0)),
    (7, 4, "10011110101011", (7,2)),
]
for n,k,s,expected in tests:
    r = solve_brute(n,k,s)
    b = s.count('1') - r
    print(f"n={n} k={k} s={s} -> Red={r} Blue={b} expected {expected}")
    assert (r,b) == expected
print("All tests passed")