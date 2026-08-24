from functools import lru_cache
from itertools import product

def red_score_optimal(n, k, s):
    L = 2 * n
    m = s.count('1')
    if m == 0:
        return 0, 0
    # extract positions of potatoes in clockwise order
    pos = [i for i, ch in enumerate(s) if ch == '1']
    # ensure sorted clockwise (they are already in increasing index)
    # compute gaps: number of empty cells between potato i and i+1 (clockwise)
    gaps = []
    for idx in range(m):
        cur = pos[idx]
        nxt = pos[(idx + 1) % m]
        # distance clockwise from cur to nxt
        if nxt > cur:
            gap = nxt - cur - 1
        else:
            gap = (L - cur) + nxt - 1
        gaps.append(gap)
    # initial parity of each potato position (0 for even index -> Red-held initially)
    init_parity = [p % 2 for p in pos]  # 0 if even index (Red holds), 1 if odd index (Blue holds)
    # we will DP over rounds and gap vector and maybe parity of each potato? but parity depends on moves count.
    # Instead we DP over rounds and gap vector and also track each potato's move count mod 2? That's too big.
    # Since m <= 6 for n<=3, we can DP over full state: positions of potatoes (tuple) and rounds left.
    # We'll do that.
    init_state = tuple(pos)
    @lru_cache(None)
    def dp(state, rounds_left):
        if rounds_left == 0:
            # compute red score: potatoes on odd indices (Blue positions)
            return sum(1 for p in state if p % 2 == 1)
        # Determine movable potatoes: i where next cell empty at start
        movable = []
        # we need to know occupancy set
        occ = set(state)
        for idx, p in enumerate(state):
            nxt = (p + 1) % L
            if nxt not in occ:
                movable.append(idx)
        # separate by team based on current holder (state[idx] parity)
        red_mov = [idx for idx in movable if state[idx] % 2 == 0]  # Red holds if even index
        blu_mov = [idx for idx in movable if state[idx] % 2 == 1]
        if not red_mov and not blu_mov:
            return dp(state, rounds_left - 1)
        best = -1e9  # Red maximizes red score
        from itertools import product
        red_choices = list(product([0,1], repeat=len(red_mov)))  # 0=stay,1=move
        blue_choices = list(product([0,1], repeat=len(blu_mov)))
        for rch in red_choices:
            min_val = 1e9
            for bch in blue_choices:
                # compute next state
                new_state_list = list(state)
                # apply moves: for each movable potato, if move then shift forward
                for idx, move_flag in zip(red_mov, rch):
                    if move_flag:
                        # move potato at state[idx] forward
                        p = state[idx]
                        new_p = (p + 1) % L
                        new_state_list[idx] = new_p
                for idx, move_flag in zip(blu_mov, bch):
                    if move_flag:
                        p = state[idx]
                        new_p = (p + 1) % L
                        new_state_list[idx] = new_p
                # after moving, ensure no duplicates? Should be fine because moves only into empty cells.
                new_state = tuple(sorted(new_state_list))  # sort to keep canonical order? Actually order matters for gaps but we can keep as is; but state representation as tuple sorted ensures uniqueness.
                val = dp(new_state, rounds_left - 1)
                if val < min_val:
                    min_val = val
            if min_val > best:
                best = min_val
        return best
    red = dp(init_state, k)
    blue = m - red
    return red, blue

# test examples
tests = [
    (2,1,"1000", (1,0)),
    (2,1,"0011", (0,2)),
    (3,2,"101110", (3,1)),
    (5,100000,"1111111111", (5,5)),
    (5,100000,"0000000000", (0,0)),
    (7,4,"10011110101011", (7,2)),
]
for n,k,s,exp in tests:
    r,b = red_score_optimal(n,k,s)
    print(f"n={n} k={k} s={s} -> ({r},{b}) expected {exp}")
    assert (r,b) == exp
print("All given tests passed")