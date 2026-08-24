import itertools

def simulate_round(state, n, red_obj, blue_obj):
    """
    state: tuple of 0/1 length 2n
    red_obj: function that takes a move decision and returns value for Red (to maximize)
    blue_obj: function for Blue (to minimize?)
    Actually we need to model the game as: each Red player chooses action to maximize final Red score,
    each Blue player chooses action to minimize final Red score (zero-sum).
    We'll compute the value of the state as the final Red score after remaining rounds.
    """
    L = 2 * n
    # For each position i, if state[i]==1, the player at i can choose:
    # - stay: potato stays at i
    # - pass left: potato goes to (i-1) mod L
    # - pass right: potato goes to (i+1) mod L
    # But only if the target is empty? Actually if multiple potatoes go to same spot, what happens?
    # The problem likely assumes that passing is simultaneous and if two potatoes go to same spot,
    # they stack? Or maybe only one can pass? Usually in such problems, each leprechaun can pass
    # to neighbor regardless of occupancy; multiple potatoes can be at same position.
    # We'll assume that.
    # So each player with a potato chooses an action in {-1,0,1} where -1=left, 0=stay, 1=right.
    # Then we compute next state: for each i, next_state[j] = sum over i of state[i] * indicator(action_i leads to j)
    # Since state[i] is 0 or 1, next_state[j] is count of potatoes moving to j.
    # But the problem likely expects that each position can have at most one potato? Not sure.
    # Looking at the examples, initial state has at most one per position? Actually s="101110" has consecutive ones.
    # So multiple potatoes can be at same position? In s="101110", positions: 0:1,1:0,2:1,3:1,4:1,5:0 -> positions 2,3,4 all have potatoes, so adjacent positions can both have potatoes.
    # So occupancy limit is not one.
    # We'll assume unlimited stacking.

    # We'll implement a minimax over all possible action combinations.
    # However, the action space is 3^(number of potatoes) which is too big even for small n.
    # But for n<=3 and k small we can try.

    # Instead, we note that the game is separable? Actually each potato moves independently?
    # Because the occupancy limit doesn't matter, and the effect of one potato's move doesn't affect another's options (since they can pass regardless of occupancy).
    # However, the objective depends on the final distribution, which is not separable because the score depends on counts per parity.
    # But since the moves are independent, the final distribution is the sum of independent moves of each potato.
    # Therefore, we can treat each potato independently: each potato starts at some position, and over k rounds, each round it moves left, right, or stays (if allowed).
    # The final position of each potato is its starting position plus the sum of moves (each move -1,0,+1).
    # The Red score is then sum over potatoes of indicator(final position is on Red team?).
    # Since each potato's moves are independent, the expected Red score (if we were computing expectation) would be sum of probabilities.
    # But here we have adversarial players: Red controls the moves of potatoes initially on Red team? Wait, no.
    # Actually, each leprechaun controls the potato they currently hold. So the control of a potato can change hands when it is passed.
    # Therefore, the control is not fixed to a potato; it depends on who holds it.
    # This makes it not independent.

    # Given time, let's brute force over all possible move choices for each player (with potato) for small n and k.
    # We'll assume each player with a potato can choose left, right, or stay (3 choices).
    # We'll compute the next state deterministically given choices.
    # Then we do minimax over rounds.

    from functools import lru_cache

    @lru_cache(None)
    def dp(state_tuple, rounds_left):
        if rounds_left == 0:
            # Red score = number of potatoes on Red team positions.
            # We need to know which team is which.
            # Let's try both possibilities and see which matches expected.
            # We'll return the Red score as defined by team_red: list of indices that are Red.
            red_indices = team_red  # to be defined outside
            return sum(state_tuple[i] for i in red_indices)
        # Determine which players have potatoes: indices i where state_tuple[i]==1
        active = [i for i in range(L) if state_tuple[i] == 1]
        # For each active player, possible moves: -1,0,1
        # We'll generate all combinations? Too many.
        # Instead, we do minimax: Red players choose to maximize, Blue players choose to minimize.
        # We need to know which indices are Red players, which are Blue.
        # Let's assume team_red = set of indices that are Red team.
        red_players = [i for i in active if i in team_red]
        blue_players = [i for i in active if i not in team_red]
        # If no active players, state stays same.
        if not active:
            return dp(state_tuple, rounds_left - 1)
        # We'll iterate over Red players' choices and Blue players' choices.
        # Since the number of active players is small for small n, we can do product.
        red_choices = list(itertools.product([-1,0,1], repeat=len(red_players)))
        blue_choices = list(itertools.product([-1,0,1], repeat=len(blue_players)))
        best_red = -1e9
        for red_act in red_choices:
            min_red = 1e9
            for blue_act in blue_choices:
                # Build next state
                next_state = [0] * L
                # For each active player, apply their move
                for idx, player in enumerate(active):
                    if player in team_red:
                        move = red_act[red_players.index(player)]
                    else:
                        move = blue_act[blue_players.index(player)]
                    if move == -1:
                        nxt = (player - 1) % L
                    elif move == 1:
                        nxt = (player + 1) % L
                    else:  # stay
                        nxt = player
                    next_state[nxt] += state_tuple[player]  # which is 1
                # Convert to tuple
                val = dp(tuple(next_state), rounds_left - 1)
                if val < min_red:
                    min_red = val
            if min_red > best_red:
                best_red = min_red
        return best_red

    # We need to define team_red. Let's try both possibilities.
    # Possibility 1: Red team = odd indices (1,3,5,...) 0-indexed.
    # Possibility 2: Red team = even indices (0,2,4,...)

    # We'll test both against the known test cases.

    def compute_score(n, k, s, red_is_odd):
        L = 2 * n
        init = tuple(1 if ch == '1' else 0 for ch in s)
        nonlocal team_red
        if red_is_odd:
            team_red = set(i for i in range(L) if i % 2 == 1)
        else:
            team_red = set(i for i in range(L) if i % 2 == 0)
        # Clear cache for dp because team_red changed
        dp.cache_clear()
        red_score = dp(init, k)
        blue_score = sum(init) - red_score
        return red_score, blue_score

    # Test cases
    tests = [
        (2,1,"1000", (1,0)),
        (2,1,"0011", (0,2)),
        (3,2,"101110", (3,1)),
        (5,100000,"1111111111", (5,5)),
        (5,100000,"0000000000", (0,0)),
        (7,4,"10011110101011", (7,2)),
    ]

    print("Testing with Red team = odd indices:")
    for n,k,s,expected in tests:
        r,b = compute_score(n,k,s, True)
        print(f"n={n} k={k} s={s}: got ({r},{b}) expected {expected} match? {r==expected[0] and b==expected[1]}")

    print("\nTesting with Red team = even indices:")
    for n,k,s,expected in tests:
        r,b = compute_score(n,k,s, False)
        print(f"n={n} k={k} s={s}: got ({r},{b}) expected {expected} match? {r==expected[0] and b==expected[1]}")

if __name__ == "__main__":
    # We'll run the simulation for small n,k to avoid too much computation.
    # For the large k cases, we cannot brute force; but we hope that for the small n,k cases we can verify.
    # We'll only run the test cases with small k.
    small_tests = [t for t in tests if t[1] <= 2]  # only k<=2
    # Actually we have (2,1), (2,1), (3,2) are small.
    # We'll adjust the code to only run those.
    # Let's rewrite inside main.
    pass