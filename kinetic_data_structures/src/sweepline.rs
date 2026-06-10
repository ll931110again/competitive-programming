//! Kinetic tournament tree and sweepline primitives.
//!
//! Russel, Karavelas, Guibas (2007): building blocks for kinetic maintenance via
//! certificate failure scheduling on sorted lists.

use crate::framework::{AdvanceReport, CertId, KineticStructure, ScheduledEvent};
use crate::framework::event_queue::EventQueue;
use crate::framework::out_of_order::drain_events_until;
use crate::motion::LinearMotion1d;

/// Kinetic tournament: maintains the minimum of values f_i(t) under linear motion.
pub struct KineticTournament {
    motions: Vec<LinearMotion1d>,
    order: Vec<usize>,
    queue: EventQueue,
    now: f64,
    next_cert_id: u64,
    /// cert_id -> (left_index_in_order, right_index_in_order)
    cert_pairs: std::collections::HashMap<u64, (usize, usize)>,
}

impl KineticTournament {
    pub fn new(motions: Vec<LinearMotion1d>) -> Self {
        let n = motions.len();
        let mut order: Vec<usize> = (0..n).collect();
        order.sort_by(|&i, &j| {
            motions[i]
                .at(0.0)
                .partial_cmp(&motions[j].at(0.0))
                .unwrap()
        });
        let mut k = Self {
            motions,
            order,
            queue: EventQueue::new(),
            now: 0.0,
            next_cert_id: 0,
            cert_pairs: std::collections::HashMap::new(),
        };
        k.reschedule_all_adjacent();
        k
    }

    pub fn winner(&self) -> usize {
        self.order[0]
    }

    pub fn winner_value(&self) -> f64 {
        self.motions[self.winner()].at(self.now)
    }

    fn alloc_cert(&mut self) -> CertId {
        let id = CertId(self.next_cert_id);
        self.next_cert_id += 1;
        id
    }

    fn reschedule_all_adjacent(&mut self) {
        self.queue = EventQueue::new();
        self.cert_pairs.clear();
        for i in 0..self.order.len().saturating_sub(1) {
            self.schedule_adjacent(i);
        }
    }

    fn schedule_adjacent(&mut self, i: usize) {
        let left = self.order[i];
        let right = self.order[i + 1];
        if let Some(t) = self
            .motions[left]
            .meet_time_after(&self.motions[right], self.now)
        {
            let cert = self.alloc_cert();
            self.cert_pairs.insert(cert.0, (i, i + 1));
            self.queue.push(ScheduledEvent { time: t, cert });
        }
    }

    fn swap_adjacent(&mut self, i: usize) {
        self.order.swap(i, i + 1);
    }

    /// Reconcile order after simultaneous swaps at the same time.
    fn sync_order_at(&mut self, t: f64) {
        let mut sorted: Vec<usize> = (0..self.motions.len()).collect();
        sorted.sort_by(|&i, &j| {
            self.motions[i]
                .at(t)
                .partial_cmp(&self.motions[j].at(t))
                .unwrap()
        });
        if self.order != sorted {
            self.order = sorted;
            self.reschedule_all_adjacent();
        }
    }
}

impl KineticStructure for KineticTournament {
    fn now(&self) -> f64 {
        self.now
    }

    fn advance_to(&mut self, t: f64) -> AdvanceReport {
        let valid: std::collections::HashSet<u64> =
            self.cert_pairs.keys().copied().collect();
        let batch = drain_events_until(&mut self.queue, t);
        let mut events = 0usize;
        for ev in batch {
            if valid.contains(&ev.cert.0) {
                self.handle_event(ev);
                events += 1;
            }
        }
        self.sync_order_at(t);
        self.now = t;
        AdvanceReport {
            events_processed: events,
            new_time: t,
        }
    }

    fn change_motion(&mut self, id: usize) {
        assert!(id < self.motions.len());
        self.reschedule_all_adjacent();
    }

    fn certificate_count(&self) -> usize {
        self.queue.len()
    }
}

impl KineticTournament {
    fn handle_event(&mut self, ev: ScheduledEvent) {
        let Some((i, j)) = self.cert_pairs.remove(&ev.cert.0) else {
            return;
        };
        if i + 1 != j || j >= self.order.len() {
            return;
        }
        self.swap_adjacent(i);
        if i > 0 {
            self.schedule_adjacent(i - 1);
        }
        self.schedule_adjacent(i);
        if i + 2 < self.order.len() {
            self.schedule_adjacent(i + 1);
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::framework::KineticStructure;
    use crate::kinetic_verify::{brute_argmin, sample_times};

    #[test]
    fn tracks_min() {
        let motions = vec![
            LinearMotion1d::new(10.0, -1.0),
            LinearMotion1d::new(0.0, 2.0),
        ];
        let mut kt = KineticTournament::new(motions.clone());
        kt.advance_to(3.0);
        assert_eq!(kt.winner(), 1);
        kt.advance_to(5.0);
        assert_eq!(kt.winner(), 0);
    }

    #[test]
    fn winner_matches_brute_argmin_over_time() {
        let motions = vec![
            LinearMotion1d::new(10.0, -1.0),
            LinearMotion1d::new(0.0, 2.0),
            LinearMotion1d::new(5.0, 0.5),
        ];
        let mut kt = KineticTournament::new(motions.clone());

        for t in sample_times(12.0, 30) {
            kt.advance_to(t);
            let winner = kt.winner();
            let expect = brute_argmin(&motions, t);
            assert_eq!(winner, expect, "wrong min at t={t}");
            let wval = kt.winner_value();
            let eval = motions[expect].at(t);
            assert!((wval - eval).abs() < 1e-9);
            for (i, m) in motions.iter().enumerate() {
                assert!(wval <= m.at(t) + 1e-9, "winner not min at t={t}, beat by id {i}");
            }
        }
    }

    #[test]
    fn swap_at_crossing_changes_winner() {
        let motions = vec![
            LinearMotion1d::new(10.0, -1.0),
            LinearMotion1d::new(0.0, 2.0),
        ];
        let meet = motions[0].meet_time_after(&motions[1], 0.0).unwrap();
        assert!((meet - 10.0 / 3.0).abs() < 1e-9);

        let mut kt = KineticTournament::new(motions);
        kt.advance_to(meet - 1e-6);
        assert_eq!(kt.winner(), 1);
        kt.advance_to(meet + 1e-6);
        assert_eq!(kt.winner(), 0);
    }
}
