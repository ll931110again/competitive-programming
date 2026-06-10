//! Out-of-order event processing (Abam et al., ESA 2006).
//!
//! When simulation samples time in steps, multiple certificates may fail between
//! samples. This module collects failures and processes them in chronological order.

use super::ScheduledEvent;
use super::event_queue::EventQueue;

/// Collects certificate failures that occurred in (last_time, target_time]
/// and applies repairs in sorted order.
pub struct OutOfOrderProcessor {
    pending: Vec<ScheduledEvent>,
}

impl OutOfOrderProcessor {
    pub fn new() -> Self {
        Self { pending: Vec::new() }
    }

    /// Drain all events from `queue` with time <= `target`, including stale ones.
    pub fn collect_until(&mut self, queue: &mut EventQueue, target: f64) {
        while let Some(t) = queue.peek_time() {
            if t > target + 1e-12 {
                break;
            }
            if let Some(ev) = queue.pop() {
                self.pending.push(ev);
            }
        }
        self.pending.sort_by(|a, b| a.time.partial_cmp(&b.time).unwrap());
    }

    /// Process collected events using `repair`; returns count processed.
    pub fn process_all<F>(&mut self, mut repair: F) -> usize
    where
        F: FnMut(ScheduledEvent) -> bool,
    {
        let mut count = 0;
        let events: Vec<_> = self.pending.drain(..).collect();
        for ev in events {
            if repair(ev) {
                count += 1;
            }
        }
        count
    }

    pub fn pending_len(&self) -> usize {
        self.pending.len()
    }
}

/// Drain all events with time <= `target` from the queue (chronological order).
pub fn drain_events_until(queue: &mut EventQueue, target: f64) -> Vec<ScheduledEvent> {
    let mut batch = Vec::new();
    while let Some(t) = queue.peek_time() {
        if t > target + 1e-12 {
            break;
        }
        batch.push(queue.pop().unwrap());
    }
    batch
}

#[cfg(test)]
mod tests {
    use super::*;
    use super::super::CertId;

    #[test]
    fn chronological_repair() {
        let mut q = EventQueue::new();
        q.push(ScheduledEvent { time: 3.0, cert: CertId(2) });
        q.push(ScheduledEvent { time: 1.0, cert: CertId(0) });
        q.push(ScheduledEvent { time: 2.0, cert: CertId(1) });

        let batch = drain_events_until(&mut q, 5.0);
        let order: Vec<_> = batch.into_iter().map(|ev| ev.cert.0).collect();
        assert_eq!(order, vec![0, 1, 2]);
    }
}
