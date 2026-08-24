from functools import lru_cache
from itertools import product

def game_value(n, k, s):
    L = 2 * n
    init = tuple(1 if ch == '1' else 0 for ch in s)

    @lru_cache(None)
    def dp(state, rounds_left):
        if rounds_left == 0:
            # red score = potatoes on odd indices (Blue positions)
            return sum(state[i] for i in range(L) if i % 2 == 1)
        movable = []
        for i in range(L):
            if state[i] == 1 and state[(i+1)%L] == 0:
                movable.append(i)
        red_mov = [i for i in movable if i % 2 == 0]   # Red holds even indices
        blu_mov = [i for i in movable if i % 2 == 1]   # Blue holds odd indices
        if not red_mov and not blu_mov:
            return dp(state, rounds_left - 1)
        best = -1e9  # Red maximizes red score
        from itertools import product
        red_choices = list(product([0,1], repeat=len(red_mov)))  # 0=stay,1=move
        blue_choices = list(product([0,1], repeat=len(blu_mov)))
        for rch in red_choices:
            min_val = 1e9
            for bch in blue_choices:
                new_state = [0]*L
                for i in range(L):
                    if state[i] == 1:
                        if i in movable:
                            if i % 2 == 0:  # red
                                idx = red_mov.index(i)
                                move = rch[idx] == 1
                            else:  # blue
                                idx = blu_mov.index(i)
                                move = bch[idx] == 1
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

    red_score = dp(init, k)
    blue_score = s.count('1') - red_score
    return red_score, blue_score

# test examples
tests = [
    (2,1,"1000", (1,0)),
    (2,1,"0011", (0,2)),
    (3,2,"101110", (3,1)),
    (5,100000,"1111111111", (5,5)),
    (5,100000,"0000000000", (0,0)),
    (7,4,"10011110101011", (7,2)),
]
print("Testing given examples:")
for n,k,s,exp in tests:
    r,b = game_value(n,k,s)
    print(f"n={n} k={k} s={s} -> ({r},{b}) expected {exp}")
    assert (r,b) == exp
print("All given tests passed")

# Let's also compute some random small cases to see pattern
import random
random.seed(42)
print("\nRandom small cases (n up to 3, k up to 4):")
for n in range(1,4):
    L=2*n
    for k in range(0,5):
        # generate a few random strings
        for _ in range(3):
            mask = random.getrandbits(L)
            s = ''.join('1' if (mask>>i)&1 else '0' for i in range(L))
            r,b = game_value(n,k,s)
            # also compute deterministic simulation
            det_r, det_b = simulate_det(n,k,s)
            print(f"n={n} k={k} s={s} game=({r},{b}) det=({det_r},{det_b})")
            if (r,b) != (det_r,det_b):
                print("   ^ differs from deterministic")
def simulate_det(n,k,s):
    L=2*n
    arr=[int(ch) for ch in s]
    for _ in range(k):
        move=[False]*L
        for i in range(L):
            if arr[i]==1:
                nxt=(i+1)%L
                if arr[nxt]==0:
                    move[i]=True
        new_arr=[0]*L
        for i in range(L):
            if move[i]:
                nxt=(i+1)%L
                new_arr[nxt]=1
            else:
                if arr[i]==1:
                    new_arr[i]=1
        arr=new_arr
    red=sum(arr[i] for i in range(L) if i%2==1)
    blue=sum(arr)-red
    return red,blue