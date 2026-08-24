from functools import lru_cache
import itertools

def game_value(n, k, s):
    L = 2 * n
    init = tuple(1 if ch == '1' else 0 for ch in s)

    @lru_cache(None)
    def dp(state, rounds_left):
        if rounds_left == 0:
            # red score = potatoes on odd indices (Blue team positions)
            return sum(state[i] for i in range(L) if i % 2 == 1)
        # Determine movable positions: i where state[i]==1 and state[(i+1)%L]==0?
        # Actually we need to consider both directions? Let's assume each player can pass left or right.
        # But to simplify, maybe the problem only allows passing to the right?
        # Let's first try with only right moves (as in deterministic simulation) and see if matches.
        # We'll compute movable as those with right neighbor empty.
        movable = []
        for i in range(L):
            if state[i] == 1 and state[(i+1)%L] == 0:
                movable.append(i)
        # Separate by team: even->Blue, odd->Red
        blue_mov = [i for i in movable if i % 2 == 0]   # Blue holds even indices
        red_mov = [i for i in movable if i % 2 == 1]   # Red holds odd indices
        # If no movable, state stays same for all remaining rounds
        if not blue_mov and not red_mov:
            return sum(state[i] for i in range(L) if i % 2 == 1)
        best = -1e9  # Red maximizes
        from itertools import product
        red_choices = list(product([0,1], repeat=len(red_mov)))  # 0=stay,1=move right
        blue_choices = list(product([0,1], repeat=len(blue_mov)))
        for rch in red_choices:
            min_val = 1e9
            for bch in blue_choices:
                new_state = [0]*L
                for i in range(L):
                    if state[i] == 1:
                        if i in movable:
                            if i % 2 == 0:  # blue
                                idx = blue_mov.index(i)
                                move = bch[idx] == 1
                            else:  # red
                                idx = red_mov.index(i)
                                move = rch[idx] == 1
                            if move:
                                nxt = (i+1)%L
                                new_state[nxt] = 1
                            else:
                                new_state[i] = 1
                        else:
                            new_state[i] = 1
                val = dp(tuple(new_state), rounds_left - 1)
                if val < min_val:
                    min_val = val
            if min_val > best:
                best = min_val
        return best

    red = dp(init, k)
    blue = s.count('1') - red
    return red, blue

# Let's test with the given examples
tests = [
    (2,1,"1000", (1,0)),
    (2,1,"0011", (0,2)),
    (3,2,"101110", (3,1)),
    (5,100000,"1111111111", (5,5)),
    (5,100000,"0000000000", (0,0)),
    (7,4,"10011110101011", (7,2)),
]

print("Testing given examples with right-move-only model:")
for n,k,s,expected in tests:
    r,b = game_value(n,k,s)
    print(f"n={n} k={k} s={s}: got ({r},{b}) expected {expected} match? {r==expected[0] and b==expected[1]}")

# Now let's explore small n,k to see pattern
print("\nExploring small n,k:")
for n in range(1, 4):
    L = 2*n
    for k in range(0, 4):
        # generate all possible s of length L with up to L potatoes? but we'll limit to few patterns
        # Instead, we'll iterate over number of potatoes from 0 to L and choose some patterns
        # For simplicity, we'll just do all s with exactly 2 potatoes (or 0,1,2,3) but that's many.
        # Let's do a few representative patterns.
        patterns = []
        # all zeros
        patterns.append('0'*L)
        # all ones
        patterns.append('1'*L)
        # single one at each position
        for i in range(L):
            arr = ['0']*L
            arr[i] = '1'
            patterns.append(''.join(arr))
        # two adjacent ones
        for i in range(L):
            arr = ['0']*L
            arr[i] = '1'
            arr[(i+1)%L] = '1'
            patterns.append(''.join(arr))
        # deduplicate
        patterns = list(set(patterns))
        for s in patterns:
            r,b = game_value(n,k,s)
            # also compute deterministic (everyone moves right if possible)
            det_red, det_blue = deterministic_sim(n,k,s)
            if (r,b) != (det_red, det_blue):
                print(f"n={n} k={k} s={s}: opt ({r},{b}) det ({det_red},{det_blue})")

def deterministic_sim(n,k,s):
    L=2*n
    a=[1 if c=='1' else 0 for c in s]
    for _ in range(k):
        move=[False]*L
        for i in range(L):
            if a[i]==1 and a[(i+1)%L]==0:
                move[i]=True
        na=[0]*L
        for i in range(L):
            if move[i]:
                na[(i+1)%L]=1
            else:
                if a[i]==1:
                    na[i]=1
        a=na
    red=sum(a[i] for i in range(L) if i%2==1)
    blue=sum(a)-red
    return red,blue