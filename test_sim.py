import sys

def simulate_opt(n, k, s):
    L = 2 * n
    arr = [int(ch) for ch in s]
    for _ in range(k):
        # determine which potatoes can move
        can_move = [False] * L
        for i in range(L):
            if arr[i] == 1:
                nxt = (i + 1) % L
                if arr[nxt] == 0:
                    can_move[i] = True
        # decide moves: Red moves potatoes it holds (even indices) if moving increases RedScore
        # Blue moves potatoes it holds (odd indices) if moving decreases RedScore
        move = [False] * L
        for i in range(L):
            if arr[i] == 1 and can_move[i]:
                # Determine effect on RedScore if this potato moves
                # RedScore = potatoes on odd indices (blue-held)
                # Moving flips parity of contribution
                # Currently, if arr[i] on even index (Red-held) -> contributes 0 to RedScore
                # After move to odd index -> contributes 1 to RedScore (increase by 1)
                # If arr[i] on odd index (Blue-held) -> contributes 1 to RedScore
                # After move to even index -> contributes 0 to RedScore (decrease by 1)
                if i % 2 == 0:  # Red-held
                    # moving increases RedScore by 1 -> good for Red
                    move[i] = True
                else:  # Blue-held
                    # moving decreases RedScore by 1 -> good for Blue (since Blue wants minimize RedScore)
                    move[i] = True
                # If moving would hurt their objective, they stay.
                # Actually we assumed they always move if beneficial.
        # apply moves
        new_arr = [0] * L
        for i in range(L):
            if move[i]:
                nxt = (i + 1) % L
                new_arr[nxt] = 1
            else:
                if arr[i] == 1:
                    new_arr[i] = 1
        arr = new_arr
    red = sum(arr[i] for i in range(L) if i % 2 == 1)  # potatoes on blue positions
    blue = sum(arr) - red
    return red, blue

def simulate_det(n, k, s):
    L = 2 * n
    arr = [int(ch) for ch in s]
    for _ in range(k):
        move = [False] * L
        for i in range(L):
            if arr[i] == 1:
                nxt = (i + 1) % L
                if arr[nxt] == 0:
                    move[i] = True
        new_arr = [0] * L
        for i in range(L):
            if move[i]:
                nxt = (i + 1) % L
                new_arr[nxt] = 1
            else:
                if arr[i] == 1:
                    new_arr[i] = 1
        arr = new_arr
    red = sum(arr[i] for i in range(L) if i % 2 == 1)
    blue = sum(arr) - red
    return red, blue

# brute force via recursion for small n,k
from functools import lru_cache
from itertools import product

def brute(n, k, s):
    L = 2 * n
    init = tuple(1 if ch == '1' else 0 for ch in s)
    @lru_cache(None)
    def dp(state, rounds_left):
        if rounds_left == 0:
            return sum(state[i] for i in range(L) if i % 2 == 1)
        movable = []
        for i in range(L):
            if state[i] == 1:
                nxt = (i + 1) % L
                if state[nxt] == 0:
                    movable.append(i)
        red_mov = [i for i in movable if i % 2 == 0]
        blu_mov = [i for i in movable if i % 2 == 1]
        if not red_mov and not blu_mov:
            return dp(state, rounds_left - 1)
        best = -1e9
        from itertools import product
        red_choices = list(product([0,1], repeat=len(red_mov)))
        blue_choices = list(product([0,1], repeat=len(blu_mov)))
        for rch in red_choices:
            min_val = 1e9
            for bch in blue_choices:
                new_state = [0]*L
                for i in range(L):
                    if state[i]==1:
                        if i in movable:
                            if i%2==0:
                                idx = red_mov.index(i)
                                move = rch[idx]==1
                            else:
                                idx = blu_mov.index(i)
                                move = bch[idx]==1
                            if move:
                                nxt = (i+1)%L
                                new_state[nxt]=1
                            else:
                                new_state[i]=1
                        else:
                            new_state[i]=1
                val = dp(tuple(new_state), rounds_left-1)
                if val < min_val:
                    min_val = val
            if min_val > best:
                best = min_val
        return best
    red = dp(init, k)
    blue = s.count('1') - red
    return red, blue

def test():
    for n in range(1,4):
        L=2*n
        for k in range(0,5):
            for mask in range(1<<L):
                s = ''.join('1' if (mask>>i)&1 else '0' for i in range(L))
                r1,b1 = simulate_opt(n,k,s)
                r2,b2 = brute(n,k,s)
                if (r1,b1)!=(r2,b2):
                    print(f"Mismatch n={n} k={k} s={s} opt=({r1},{b1}) brute=({r2},{b2})")
                    return
    print("All tests passed")

if __name__=="__main__":
    test()