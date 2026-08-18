# Huawei 2026 Challenge - Edge–Cloud Collaborative Scheduling

This is a baseline solution for the Huawei 2026 Challenge problem "Edge–Cloud Collaborative Scheduling".

## Approach

The baseline solution implements a simple reactive scheduler that:
1. Assigns each request to a remote computer in round-robin fashion (request i assigned to remote i % K).
2. Processes each request sequentially through its stages, using:
   - One full input-stage piece (i.e., P PROC processes all layers [0, num_layers) at once).
   - Groups of size 1 for output steps (i.e., each D PRE, D PROC, D POST handles exactly one request).
3. The scheduler is purely reactive: it only schedules tasks when their predecessors have completed, as reported by TDN and XDN events.
4. It does not attempt to overlap or pipeline requests; it waits for a request to complete before starting the next one on the same resources. However, because we have multiple remote computers and round-robin assignment, different requests can be processed in parallel on different remotes.

## Correctness

The scheduler ensures that:
- Each task is assigned only to a free computer.
- Each task's predecessors have occurred (tracked via state per request).
- No request is included in more than one task at a time.
- Input-stage pieces are not split (we use the full range [0, num_layers)).
- Output groups have size 1, so no grouping complexities.
- The remote computer assignment is fixed per request.

## Limitations

This baseline does not optimize for:
- Overlapping input-stage transfers with computation.
- Pipelining multiple requests (though round-robin assignment allows some parallelism on different remotes).
- Grouping multiple requests in output steps to better utilize the local computer.
- Splitting input-stage pieces to allow other remote work to interleave.

Despite these limitations, the scheduler is legal and should finish all requests, thus avoiding a score of 0.

## Building

To compile the solution:

```bash
g++ -std=c++17 -O2 -pipe -static -s solution.cpp -o solution
```

## Running

The solution reads from standard input and writes to standard output, following the interactive protocol.

## Notes

- The solution ignores the task-time table and scoring parameters; it uses fixed execution assumptions (full pieces, size-1 groups).
- Schedule cost `S` is not used in the scheduler's decisions (it is accounted for by the interactor).
- The solution does not use the provided bandwidth, latency, etc., for transfer time calculations; it relies entirely on the interactor's event timing.