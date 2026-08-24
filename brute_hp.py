import sys
from functools import lru_cache

def solve_brute(n, k, s):
    L = 2 * n
    init = tuple(1 if ch=='1' else 0 for ch in s)

    @lru_cache(None)
    def dp(state, rounds_left, turn):  # turn 0 for Red to move? Actually both teams move simultaneously each round, but decisions are per potato by its holder.
        # Since moves are simultaneous, we need to compute outcome given both teams choose optimally.
        # We'll treat as zero-sum: Red wants maximize R, Blue minimize R.
        if rounds_left == 0:
            # compute R score
            red = 0
            for idx, val in enumerate(state):
                if val:
                    if idx % 2 == 0:  # Red holds potato -> contributes to Blue score, not Red
                        pass
                    else:
                        red += 1  # Blue holds potato -> contributes to Red score
            return red
        # generate all possible moves for each potato according to whose turn? Actually both teams decide simultaneously.
        # We need to consider that each potato's decision is made by the team of its holder at start of round.
        # So we need to assign control: for each i where state[i]==1, controller = team of i (Red if i%2==0 else Blue).
        # Then each controller decides for its potato whether to move (if possible) or stay.
        # If right neighbor empty, controller can choose move or stay; if right occupied, must stay.
        # Since controllers may have opposing objectives, we need to compute the outcome of this simultaneous move game.
        # This is a game where each controller chooses action for their potato; the resulting state determines next rounds_left-1.
        # Since it's zero-sum with perfect information, we can compute the value via minimax over joint actions? That's exponential.
        # But we can note that each potato's action only affects its own movement and the occupancy of neighbor cells.
        # However due to simultaneity and blocking constraints, there are interactions.
        # For small L we can brute force over all combinations of choices for each potato where move is optional.
        pass
    return None

# Instead, let's do explicit game tree: at each round, we consider all possible assignments of moves for each potato where move allowed.
# Since L <= 6 for n<=3, we can brute force.
def simulate_all_moves(state):
    L = len(state)
    # For each i where state[i]==1, determine if can move: right empty?
    can_move = [False]*L
    for i in range(L):
        if state[i]==1:
            nxt = (i+1)%L
            if state[nxt]==0:
                can_move[i]=True
    # Enumerate choices for each movable potato: 0=stay,1=move
    moves = []
    from itertools import product
    choices = product([0,1], repeat=sum(can_move))
    # map choice to potato index
    movable_idx = [i for i in range(L) if can_move[i]]
    for choice in choices:
        new_state = [0]*L
        # apply stays and moves
        for i in range(L):
            if state[i]==1:
                if not can_move[i]:
                    # must stay
                    new_state[i] = 1
                else:
                    # indexed in movable_idx
                    idx = movable_idx.index(i)
                    if choice[idx]==0:
                        new_state[i]=1
                    else:
                        nxt = (i+1)%L
                        new_state[nxt]=1
        # also need to handle case where a potato moves into a cell that also had a potato staying? That can't happen because move only allowed if right empty at start, and staying potato only if right occupied or must stay. So no conflict.
        yield tuple(new_state)

def minimax(state, rounds_left):
    if rounds_left==0:
        red = 0
        for idx,val in enumerate(state):
            if val and idx%2==1:  # Blue holds
                red+=1
        return red
    # Red's turn? Actually both teams move simultaneously; we need to compute the value of the game where Red chooses moves for its potatoes to maximize final R, Blue chooses moves for its potatoes to minimize final R.
    # This is a simultaneous move game; we can compute via solving as a matrix game? But we can treat as Red chooses moves for its potatoes, Blue chooses for its potatoes, and we assume they can coordinate within team.
    # Since the game is zero-sum and perfect information, the value can be computed via minimax over joint strategies: Red picks a strategy (function from its potatoes to actions) to maximize the minimum over Blue's strategies.
    # For small state we can brute force over all combos of Red's choices and Blue's choices.
    L = len(state)
    red_idx = [i for i in range(L) if state[i]==1 and i%2==0]  # Red holds potatoes at even indices
    blue_idx = [i for i in range(L) if state[i]==1 and i%2==1]  # Blue holds
    # For each potato, compute if can move
    can_move = [False]*L
    for i in L:
        if state[i]==1:
            nxt=(i+1)%L
            if state[nxt]==0:
                can_move[i]=True
    # Now enumerate Red choices for its movable potatoes, Blue choices for its movable potatoes.
    red_movable = [i for i in red_idx if can_move[i]]
    blue_movable = [i for i in blue_idx if can_move[i]]
    best = -1e9
    # Red wants to maximize final R; we will compute outcome given Red's choice and Blue's choice, then Red picks max over its choices of (min over Blue's choices).
    from itertools import product
    # Precompute list of choices for each side
    red_choices = list(product([0,1], repeat=len(red_movable)))
    blue_choices = list(product([0,1], repeat=len(blue_movable)))
    for rchoice in red_choices:
        # For this red choice, compute worst-case (min) over blue choices
        min_val = 1e9
        for bchoice in blue_choices:
            # compute new state
            new_state = [0]*L
            # process all potatoes
            for i in range(L):
                if state[i]==1:
                    if not can_move[i]:
                        new_state[i]=1
                    else:
                        if i in red_idx:
                            idx = red_movable.index(i)
                            move = rchoice[idx]==1
                        else: # blue
                            idx = blue_movable.index(i)
                            move = bchoice[idx]==1
                        if move:
                            nxt = (i+1)%L
                            new_state[nxt]=1
                        else:
                            new_state[i]=1
            val = minimax(tuple(new_state), rounds_left-1)
            if val < min_val:
                min_val = val
        if min_val > best:
            best = min_val
    return best

# Test small
for n in range(1,4):
    L=2*n
    for k in range(0,4):
        for mask in range(1<<L):
            s=''.join('1' if (mask>>i)&1 else '0' for i in range(L))
            r = minimax(tuple(1 if ch=='1' else 0 for ch in s), k)
            # just print one example
            if n==2 and k==1 and s=='1000':
                print(f"n={n} k={k} s={s} R={r}")
                # also compute blue score
                total = s.count('1')
                b = total - r
                print(f"Red score {r}, Blue score {b}")