import sys

def solve():
    data = sys.stdin.readline().strip()
    if not data:
        return
    # first line: K S latency_in_ms bandwidth_gbps bytes_per_token num_layers
    parts = data.split()
    K = int(parts[0])
    S = float(parts[1])  # schedule cost, not used in baseline
    latency_in_ms = float(parts[2])
    bandwidth_gbps = float(parts[3])
    bytes_per_token = int(parts[4])
    num_layers = int(parts[5])

    # second line: scoring parameters (ignore)
    sys.stdin.readline()
    # third line: N
    line = sys.stdin.readline().strip()
    if not line:
        return
    N = int(line)
    # read N rows of task-time table (ignore for baseline)
    for _ in range(N):
        sys.stdin.readline()

    # state
    local_free = True
    remote_free = [True] * K

    class RequestState:
        __slots__ = ('rid', 'assigned', 'stage',
                     'waiting_up', 'waiting_down',
                     'next_stage_after_up', 'next_stage_after_down',
                     'tokens_produced', 'finished')
        def __init__(self, rid):
            self.rid = rid
            self.assigned = rid % K  # simple round-robin assignment
            self.stage = 0  # 0: need P_PRE, 1: need P_PROC, 2: need P_POST, 3: need D_PRE, 4: need D_PROC, 5: need D_POST
            self.waiting_up = False
            self.waiting_down = False
            self.next_stage_after_up = None
            self.next_stage_after_down = None
            self.tokens_produced = 0
            self.finished = False

    requests = {}  # rid -> RequestState

    def schedule_p_pre(req):
        nonlocal local_free
        if not local_free:
            return False
        # task: P PRE <remote> <rid>
        print(f"E P PRE {req.assigned} {req.rid}")
        local_free = False
        return True

    def schedule_p_proc(req):
        nonlocal remote_free
        if not remote_free[req.assigned]:
            return False
        # task: P PROC 0 <num_layers> <remote> <rid>
        print(f"C{req.assigned} P PROC 0 {num_layers} {req.assigned} {req.rid}")
        remote_free[req.assigned] = False
        return True

    def schedule_p_post(req):
        nonlocal local_free
        if not local_free:
            return False
        # task: P POST <remote> <rid>
        print(f"E P POST {req.assigned} {req.rid}")
        local_free = False
        return True

    def schedule_d_pre(req):
        nonlocal local_free
        if not local_free:
            return False
        # task: D PRE -1 1 <rid>
        print(f"E D PRE -1 1 {req.rid}")
        local_free = False
        return True

    def schedule_d_proc(req):
        nonlocal remote_free
        if not remote_free[req.assigned]:
            return False
        # task: D PROC <remote> 1 <rid>
        print(f"C{req.assigned} D PROC {req.assigned} 1 {req.rid}")
        remote_free[req.assigned] = False
        return True

    def schedule_d_post(req):
        nonlocal local_free
        if not local_free:
            return False
        # task: D POST -1 1 <rid>
        print(f"E D POST -1 1 {req.rid}")
        local_free = False
        return True

    def handle_tdn(server, task_spec):
        parts = task_spec.split()
        if parts[0] == 'P':
            if parts[1] == 'PRE':
                remote = int(parts[2])
                rid = int(parts[3])
                req = requests.get(rid)
                if req and not req.finished:
                    # after P PRE TDN: queued UP transfer
                    req.waiting_up = True
                    req.next_stage_after_up = 1  # after UP, can do P_PROC
            elif parts[1] == 'PROC':
                # ls le remote rid
                rid = int(parts[5])
                req = requests.get(rid)
                if req and not req.finished:
                    # after P PROC TDN: queued DOWN transfer
                    req.waiting_down = True
                    req.next_stage_after_down = 2  # after DOWN, can do P_POST
            elif parts[1] == 'POST':
                remote = int(parts[2])
                rid = int(parts[3])
                req = requests.get(rid)
                if req and not req.finished:
                    # after P POST TDN: ready for D_PRE
                    req.stage = 3
                    req.waiting_up = False
                    req.waiting_down = False
                    req.next_stage_after_up = None
                    req.next_stage_after_down = None
        elif parts[0] == 'D':
            if parts[1] == 'PRE':
                # D PRE -1 m rid...
                m = int(parts[3])
                rids = list(map(int, parts[4:4+m]))
                for rid in rids:
                    req = requests.get(rid)
                    if req and not req.finished:
                        # after D PRE TDN: queued UP transfer(s)
                        req.waiting_up = True
                        req.next_stage_after_up = 4  # after UP, can do D_PROC
            elif parts[1] == 'PROC':
                remote = int(parts[2])
                m = int(parts[3])
                rids = list(map(int, parts[4:4+m]))
                for rid in rids:
                    req = requests.get(rid)
                    if req and not req.finished:
                        # after D PROC TDN: queued DOWN transfer
                        req.waiting_down = True
                        req.next_stage_after_down = 5  # after DOWN, can do D_POST
            elif parts[1] == 'POST':
                m = int(parts[3])
                rids = list(map(int, parts[4:4+m]))
                for rid in rids:
                    req = requests.get(rid)
                    if req and not req.finished:
                        # after D POST TDN: token produced
                        req.tokens_produced += 1
                        # ready for another D_PRE (assuming more tokens)
                        req.stage = 3
                        req.waiting_up = False
                        req.waiting_down = False
                        req.next_stage_after_up = None
                        req.next_stage_after_down = None
                        # Note: we do not know if more tokens; we will rely on FIN to stop

    def handle_xdn(direction, remote, size, typ, m, rids):
        # direction: 'UP' or 'DOWN'
        # typ: 'PRE' or 'DEC'
        for rid in rids:
            req = requests.get(rid)
            if not req or req.finished:
                continue
            if typ == 'PRE':
                if direction == 'UP':
                    # input-stage UP transfer completed
                    if req.waiting_up:
                        req.waiting_up = False
                        req.stage = req.next_stage_after_up
                        req.next_stage_after_up = None
                else:  # DOWN
                    if req.waiting_down:
                        req.waiting_down = False
                        req.stage = req.next_stage_after_down
                        req.next_stage_after_down = None
            else:  # DEC
                if direction == 'UP':
                    # output-step UP transfer completed
                    if req.waiting_up:
                        req.waiting_up = False
                        req.stage = req.next_stage_after_up
                        req.next_stage_after_up = None
                else:  # DOWN
                    if req.waiting_down:
                        req.waiting_down = False
                        req.stage = req.next_stage_after_down
                        req.next_stage_after_down = None

    def handle_fin(rid):
        req = requests.get(rid)
        if req:
            req.finished = True
            # optionally clean up

    def try_schedule():
        nonlocal local_free, remote_free
        scheduled_any = False
        # local computer
        if local_free:
            for rid, req in list(requests.items()):
                if req.finished:
                    continue
                if req.waiting_up or req.waiting_down:
                    continue
                if req.stage == 0:  # need P_PRE
                    if schedule_p_pre(req):
                        scheduled_any = True
                        break
                elif req.stage == 2:  # need P_POST
                    if schedule_p_post(req):
                        scheduled_any = True
                        break
                elif req.stage == 3:  # need D_PRE
                    if schedule_d_pre(req):
                        scheduled_any = True
                        break
                elif req.stage == 5:  # need D_POST
                    if schedule_d_post(req):
                        scheduled_any = True
                        break
        # remote computers
        for k in range(K):
            if remote_free[k]:
                for rid, req in list(requests.items()):
                    if req.finished:
                        continue
                    if req.waiting_up or req.waiting_down:
                        continue
                    if req.assigned != k:
                        continue
                    if req.stage == 1:  # need P_PROC
                        if schedule_p_proc(req):
                            scheduled_any = True
                            break
                    elif req.stage == 4:  # need D_PROC
                        if schedule_d_proc(req):
                            scheduled_any = True
                            break
        return scheduled_any

    # main loop
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        line = line.strip()
        if line == 'END':
            break
        # timestamp line (ignore)
        t = line
        # event count line
        line = sys.stdin.readline()
        if not line:
            break
        e = int(line.strip())
        events = []
        for _ in range(e):
            events.append(sys.stdin.readline().rstrip('\n'))
        # process events
        for ev in events:
            if not ev:
                continue
            if ev.startswith('ARR'):
                # ARR rid L_in
                _, rid_str, lin_str = ev.split()
                rid = int(rid_str)
                # L_in not needed
                if rid not in requests:
                    requests[rid] = RequestState(rid)
                else:
                    # should not happen
                    pass
            elif ev.startswith('TDN'):
                # TDN server task_spec dur
                # split into three parts: server, task_spec, dur
                # but task_spec may contain spaces, so split max 2
                parts = ev.split(' ', 2)
                if len(parts) < 3:
                    continue
                server = parts[0]
                task_spec = parts[1]
                # dur = parts[2]  # ignore
                handle_tdn(server, task_spec)
            elif ev.startswith('XDN'):
                # XDN UP/DOWN remote size PRE/DEC m rid...
                parts = ev.split()
                if len(parts) < 6:
                    continue
                direction = parts[0]
                remote = int(parts[1])
                size = int(parts[2])  # bytes, not used
                typ = parts[3]
                m = int(parts[4])
                rids = list(map(int, parts[5:5+m]))
                handle_xdn(direction, remote, size, typ, m, rids)
            elif ev.startswith('FIN'):
                # FIN rid
                _, rid_str = ev.split()
                rid = int(rid_str)
                handle_fin(rid)
            else:
                # unknown, ignore
                pass
        # after processing events, try to schedule tasks
        # we may be able to schedule multiple tasks in same frame (different resources)
        # keep trying until no more can be scheduled
        while True:
            if not try_schedule():
                break
        sys.stdout.flush()

if __name__ == "__main__":
    solve()