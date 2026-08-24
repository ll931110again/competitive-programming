def simulate_rule184(arr, steps):
    L = len(arr)
    cur = arr[:]
    for _ in range(steps):
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
    return cur

def gaps_from_positions(pos, L):
    m = len(pos)
    gaps = []
    for idx in range(m):
        cur = pos[idx]
        nxt = pos[(idx+1)%m]
        if nxt > cur:
            gap = nxt - cur - 1
        else:
            gap = (L - cur) + nxt - 1
        gaps.append(gap)
    return gaps

def positions_from_gaps(gaps, L):
    # reconstruct positions assuming first particle at 0? Actually we need an offset.
    # We'll compute relative positions.
    m = len(gaps)
    pos = [0]*m
    cur = 0
    for i in range(m):
        pos[i] = cur
        cur = cur + 1 + gaps[i]  # move to next particle: current cell occupied, then gap empties
    # Now pos are positions assuming first particle at 0, but may exceed L-1? Actually sum(gaps)+m = L, so cur after loop = L.
    # So positions are in [0, L-1].
    return pos

def test_random():
    import random
    random.seed(42)
    for L in range(4, 13, 2):
        n = L//2
        for _ in range(100):
            arr = [random.randint(0,1) for _ in range(L)]
            # ensure at least one particle? not necessary.
            # compute after many steps, say 1000 steps
            steps = 1000
            final = simulate_rule184(arr, steps)
            # compute after 2*L steps maybe see periodic?
            # Actually rule 184 on a ring is periodic with period dividing something.
            # Let's compute the configuration after steps and after steps+L to see if same.
            final2 = simulate_rule184(arr, steps+L)
            if final != final2:
                # not periodic with period L
                pass
            # compute gaps
            pos = [i for i, v in enumerate(arr) if v==1]
            m = len(pos)
            if m==0:
                continue
            gaps0 = gaps_from_positions(pos, L)
            pos_final = [i for i, v in enumerate(final) if v==1]
            gaps_final = gaps_from_positions(pos_final, L)
            # maybe after many steps, gaps become equal or differ by at most 1?
            # sort gaps
            sorted_init = sorted(gaps0)
            sorted_final = sorted(gaps_final)
            # compute average gap
            avg = (L - m) // m
            rem = (L - m) % m
            expected = [avg+1]*rem + [avg]*(m-rem)
            expected.sort()
            if sorted_final != expected:
                print(f"L={L} init={arr} gaps0={gaps0} final={final} gaps_final={gaps_final} expected={expected}")
                return
    print("All random tests passed: after many steps gaps become uniform")

if __name__=="__main__":
    test_random()