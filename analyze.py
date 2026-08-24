from functools import lru_cache

def game_value(n, k, s):
    L = 2 * n
    init = tuple(1 if ch == '1' else 0 for ch in s)

    @lru_cache(None)
    def dp(state, rounds_left):
        if rounds_left == 0:
            # red score = potatoes on odd indices (Red team)
            return sum(state[i] for i in range(L) if i % 2 == 1)
        # Determine movable positions: i where state[i]==1 and state[(i+1)%L]==0
        movable = []
        for i in range(L):
            if state[i] == 1 and state[(i+1)%L] == 0:
                movable.append(i)
        # Separate by team based on i parity: even->Blue, odd->Red
        blue_mov = [i for i in movable if i % 2 == 0]   # Blue holds even indices
        red_mov = [i for i in movable if i % 2 == 1]   # Red holds odd indices
        # If no movable, state stays same for all remaining rounds
        if not blue_mov and not red_mov:
            return sum(state[i] for i in range(L) if i % 2 == 1)
        best = -1e9  # Red maximizes
        from itertools import product
        # Red choices: for each red_mov, 0=stay,1=move
        red_choices = list(product([0,1], repeat=len(red_mov)))
        blue_choices = list(product([0,1], repeat=len(blue_mov)))
        for rch in red_choices:
            min_val = 1e9
            for bch in blue_choices:
                # compute next state
                new_state = [0]*L
                # first copy stays for non-movable potatoes and those that stay
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

def deterministic(n,k,s):
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

def red_never_move(n,k,s):
    # Red never moves when possible; Blue always moves when possible? Let's define strategies
    L=2*n
    a=[1 if c=='1' else 0 for c in s]
    for _ in range(k):
        move=[False]*L
        for i in range(L):
            if a[i]==1 and a[(i+1)%L]==0:
                if i%2==1:  # Red held, they choose not to move
                    move[i]=False
                else:  # Blue held, they move if possible (to minimize Red score)
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

def blue_never_move(n,k,s):
    L=2*n
    a=[1 if c=='1' else 0 for c in s]
    for _ in range(k):
        move=[False]*L
        for i in range(L):
            if a[i]==1 and a[(i+1)%L]==0:
                if i%2==0:  # Blue held, they choose not to move
                    move[i]=False
                else:  # Red held, they move if possible (to increase Red score)
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

def both_never_move(n,k,s):
    L=2*n
    a=[1 if c=='1' else 0 for c in s]
    for _ in range(k):
        # no one moves
        pass
    red=sum(a[i] for i in range(L) if i%2==1)
    blue=sum(a)-red
    return red,blue

def both_always_move(n,k,s):
    return deterministic(n,k,s)

# Test cases
tests = [
    (2,1,"1000", (1,0)),
    (2,1,"0011", (0,2)),
    (3,2,"101110", (3,1)),
    (5,100000,"1111111111", (5,5)),
    (5,100000,"0000000000", (0,0)),
    (7,4,"10011110101011", (7,2)),
    (1,1,"10", (1,0)),
    (1,1,"01", (0,1)),
    (1,2,"10", (1,0)),  # from earlier brute? we need compute
    (1,2,"01", (0,1)),
    (2,2,"1000", None),
    (2,2,"0011", None),
    (2,2,"1010", None),
    (2,2,"0101", None),
]

print("Testing optimal vs various strategies:")
for n,k,s,expected in tests:
    if expected is None:
        continue
    r_opt,b_opt = game_value(n,k,s)
    r_det,b_det = deterministic(n,k,s)
    r_redn,b_redn = red_never_move(n,k,s)
    r_bluen,b_bluen = blue_never_move(n,k,s)
    r_none,b_none = both_never_move(n,k,s)
    r_alw,b_alw = both_always_move(n,k,s)
    print(f'n={n} k={k} s={s}')
    print(f'  Optimal:         ({r_opt},{b_opt})')
    print(f'  Deterministic:   ({r_det},{b_det})')
    print(f'  Red never move:  ({r_redn},{b_redn})')
    print(f'  Blue never move: ({r_bluen},{b_bluen})')
    print(f'  Both never:      ({r_none},{b_none})')
    print(f'  Both always:     ({r_alw},{b_alw})')
    if expected:
        print(f'  Expected:        {expected}')
    print()