from functools import lru_cache
from itertools import product

def optimal_red_potatoes(n, k, s):
    """Return optimal final number of potatoes on Red positions (even indices) after k rounds,
    with Red minimizing and Blue maximizing."""
    L = 2 * n
    init = tuple(1 if ch == '1' else 0 for ch in s)

    @lru_cache(None)
    def dp(state, rounds_left):
        if rounds_left == 0:
            # count potatoes on even indices (Red positions)
            return sum(state[i] for i in range(L) if i % 2 == 0)
        # Determine movable potatoes: i where state[i]==1 and state[(i+1)%L]==0
        movable = []
        for i in range(L):
            if state[i] == 1 and state[(i+1)%L] == 0:
                movable.append(i)
        red_mov = [i for i in movable if i % 2 == 0]   # Red-held potatoes that can move
        blu_mov = [i for i in movable if i % 2 == 1]   # Blue-held potatoes that can move
        if not red_mov and not blu_mov:
            return dp(state, rounds_left - 1)
        best = 1e9  # Red minimizes red potatoes
        from itertools import product
        red_choices = list(product([0,1], repeat=len(red_mov)))  # 0=stay,1=move
        blue_choices = list(product([0,1], repeat=len(blu_mov)))
        for rch in red_choices:
            max_val = -1e9  # Blue maximizes red potatoes given red's choice
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
                if val > max_val:
                    max_val = val
            if max_val < best:
                best = max_val
        return best

    red_final = dp(init, k)
    return red_final

# test examples
tests = [
    (2,1,"1000", 1),  # red score? Wait we need red final potatoes? Let's compute from given scores.
    # Given example output: Red score, Blue score.
    # Red score = potatoes on Blue positions after k rounds.
    # Blue score = potatoes on Red positions after k rounds.
    # So if output is (R_score, B_score) then:
    # B_score = potatoes on Red positions = R_final
    # R_score = potatoes on Blue positions = P - R_final
    # So R_final = B_score.
    # Let's verify with first example: n=2,k=1,s="1000" output "1 0"
    # So Red score=1, Blue score=0.
    # Then potatoes on Red positions = Blue score = 0? That seems off.
    # Let's compute manually: s="1000" positions 0:1,1:0,2:0,3:0.
    # Red positions: 0,2 (even indices). Initially potatoes at pos0 (Red) and none at pos2.
    # Blue positions:1,3. Initially none.
    # After optimal play, they said Red score=1 (eliminated Blue leprechauns) meaning Blue holds a potato after rounds? Actually Red score = number of eliminated leprechauns on other team = number of Blue leprechauns holding potato after rounds.
    # So Red score = potatoes on Blue positions after rounds.
    # Blue score = potatoes on Red positions after rounds.
    # Output "1 0" => potatoes on Blue positions =1, potatoes on Red positions=0.
    # Let's simulate: If leprechaun1 (pos0) passes to leprechaun2 (pos1), then pos0 empty, pos1 potato (Blue holds). So potatoes on Red positions=0, on Blue positions=1. matches.
    # So indeed Red final potatoes (R_final) = potatoes on Red positions = Blue score.
    # Blue final potatoes (B_final) = potatoes on Blue positions = Red score.
    # So we can compute R_final = Blue score from output.
    # Let's compute for each test.
]

def test_given():
    given = [
        (2,1,"1000", (1,0)),   # (RedScore, BlueScore)
        (2,1,"0011", (0,2)),
        (3,2,"101110", (3,1)),
        (5,100000,"1111111111", (5,5)),
        (5,100000,"0000000000", (0,0)),
        (7,4,"10011110101011", (7,2)),
    ]
    for n,k,s,(red_score,blue_score) in given:
        R_final_opt = blue_score   # potatoes on Red positions after k rounds
        R_final_comp = optimal_red_potatoes(n,k,s)
        print(f"n={n} k={k} s={s} -> RedScore={red_score} BlueScore={blue_score} => R_final expected {R_final_opt} computed {R_final_comp}")
        assert R_final_comp == R_final_opt, f"Mismatch"
    print("All given tests passed")

if __name__=="__main__":
    test_given()