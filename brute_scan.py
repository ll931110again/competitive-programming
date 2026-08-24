import sys
from functools import lru_cache
from itertools import product

def red_score_optimal(n, k, s):
    L = 2 * n
    init = tuple(1 if ch == '1' else 0 for ch in s)
    @lru_cache(None)
    def dp(state, rounds_left):
        if rounds_left == 0:
            # red score = potatoes on odd indices
            return sum(state[i] for i in range(L) if i % 2 == 1)
        movable = []
        for i in range(L):
            if state[i] == 1 and state[(i+1)%L] == 0:
                movable.append(i)
        red_mov = [i for i in movable if i % 2 == 0]
        blu_mov = [i for i in movable if i % 2 == 1]
        if not red_mov and not blu_mov:
            return dp(state, rounds_left - 1)
        best = -1e9  # Red maximizes
        from itertools import product
        red_choices = list(product([0,1], repeat=len(red_mov)))
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
    red = dp(init, k)
    blue = s.count('1') - red
    return red, blue

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

def test():
    for n in range(1,5):
        L=2*n
        for k in range(0,7):
            print(f"Testing n={n} k={k}")
            for mask in range(1<<L):
                s=''.join('1' if (mask>>i)&1 else '0' for i in range(L))
                r_opt,b_opt = red_score_optimal(n,k,s)
                r_det,b_det = simulate_det(n,k,s)
                if (r_opt,b_opt)!=(r_det,b_det):
                    print(f"  Diff s={s}: opt=({r_opt},{b_opt}) det=({r_det},{b_det})")
                    # Also compute some stats
                    R0 = sum(1 for i in range(L) if i%2==0 and s[i]=='1')
                    B0 = sum(1 for i in range(L) if i%2==1 and s[i]=='1')
                    print(f"    R0={R0} B0={B0}")
                    ten = sum(1 for i in range(L) if s[i]=='1' and s[(i+1)%L]=='0')
                    print(f"    #10={ten}")
                    # Let's also compute maybe the value after many rounds?
                    return
    print("All tests passed up to n=4 k=6")

if __name__=="__main__":
    test()