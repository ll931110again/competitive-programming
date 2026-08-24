def simulate_det(n, k, s):
    L = 2 * n
    arr = [int(ch) for ch in s]
    for _ in range(k):
        # compute moves based on start of round
        move = [False] * L
        for i in range(L):
            if arr[i] == 1:
                nxt = (i + 1) % L
                if arr[nxt] == 0:
                    move[i] = True
        # apply moves simultaneously
        new_arr = [0] * L
        for i in range(L):
            if move[i]:
                nxt = (i + 1) % L
                new_arr[nxt] = 1
            else:
                if arr[i] == 1:
                    new_arr[i] = 1
        arr = new_arr
    # compute scores
    red = 0  # red team score = #blue-held potatoes
    blue = 0 # blue team score = #red-held potatoes
    for idx, val in enumerate(arr):
        if val:
            if idx % 2 == 0:  # even index -> leprechaun odd? Wait index0 -> leprechaun1 odd -> Red? Let's recompute:
                # leprechaun number = idx+1
                # Red if (idx+1) odd => idx even
                # Blue if (idx+1) even => idx odd
                # So Red holds if idx even -> contributes to Blue score
                # Blue holds if idx odd -> contributes to Red score
                blue += 1  # red-held
            else:
                red += 1   # blue-held
    return red, blue

# test examples
print("Test1:", simulate_det(2,1,"1000"))  # expected 1 0
print("Test2:", simulate_det(2,1,"0011"))  # expected 0 2
print("Test3:", simulate_det(3,2,"101110"))  # expected 3 1
print("Test4:", simulate_det(5,100000,"1111111111"))  # expected 5 5
print("Test5:", simulate_det(5,100000,"0000000000"))  # expected 0 0
print("Test6:", simulate_det(7,4,"10011110101011"))  # expected 7 2