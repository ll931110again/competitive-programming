def simulate_state(arr, steps):
    L = len(arr)
    cur = arr[:]
    seen = {tuple(cur): 0}
    for t in range(1, steps+1):
        move = [False]*L
        for i in range(L):
            if cur[i]==1:
                nxt = (i+1)%L
                if cur[nxt]==0:
                    move[i]=True
        nxt = [0]*L
        for i in range(L):
            if move[i]:
                nxt[(i+1)%L]=1
            else:
                if cur[i]==1:
                    nxt[i]=1
        cur = nxt
        if tuple(cur) in seen:
            return seen[tuple(cur)], t-seen[tuple(cur)]
        seen[tuple(cur)]=t
    return None, None

import random
for n in range(1,6):
    L=2*n
    for _ in range(10):
        arr = [random.randint(0,1) for _ in range(L)]
        start, period = simulate_state(arr, 1000)
        if period is not None:
            print(f"n={n} period={period}")
        else:
            print(f"n={n} no period found in 1000 steps")