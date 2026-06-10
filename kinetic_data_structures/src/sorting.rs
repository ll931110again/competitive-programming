//! Kinetic sorting / predecessor (Abam & de Berg, SoCG 2005).
//!
//! Maintains sorted order of points moving along a line (by x-coordinate or
//! arbitrary scalar certificate).

use crate::framework::{AdvanceReport, CertId, KineticStructure, ScheduledEvent};
use crate::framework::event_queue::EventQueue;
use crate::framework::out_of_order::drain_events_until;
use crate::motion::LinearMotion1d;

/// Kinetic sorting on one scalar key per object.
pub struct KineticSorting {
    motions: Vec<LinearMotion1d>,
    order: Vec<usize>,
    queue: EventQueue,
    now: f64,
    next_cert_id: u64,
    cert_pairs: std::collections::HashMap<u64, usize>,
}

impl KineticSorting {
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
        k.reschedule_all();
        k
    }

    pub fn from_2d_x(motions: &[crate::motion::LinearMotion2d]) -> Self {
        Self::new(
            motions
                .iter()
                .map(|m| LinearMotion1d::new(m.x0, m.vx))
                .collect(),
        )
    }

    pub fn order(&self) -> Vec<usize> {
        self.order.clone()
    }

    pub fn predecessor(&self, id: usize) -> Option<usize> {
        let pos = self.order.iter().position(|&x| x == id)?;
        if pos == 0 {
            None
        } else {
            Some(self.order[pos - 1])
        }
    }

    pub fn value(&self, id: usize) -> f64 {
        self.motions[id].at(self.now)
    }

    pub fn set_motion(&mut self, id: usize, motion: LinearMotion1d) {
        self.motions[id] = motion;
        self.reschedule_all();
    }

    fn alloc_cert(&mut self) -> CertId {
        let id = CertId(self.next_cert_id);
        self.next_cert_id += 1;
        id
    }

    fn reschedule_all(&mut self) {
        self.queue = EventQueue::new();
        self.cert_pairs.clear();
        for i in 0..self.order.len().saturating_sub(1) {
            self.schedule_pair(i);
        }
    }

    fn schedule_pair(&mut self, i: usize) {
        let a = self.order[i];
        let b = self.order[i + 1];
        if let Some(t) = self.motions[a].meet_time_after(&self.motions[b], self.now) {
            let cert = self.alloc_cert();
            self.cert_pairs.insert(cert.0, i);
            self.queue.push(ScheduledEvent { time: t, cert });
        }
    }

    fn handle_event(&mut self, ev: ScheduledEvent) {
        let Some(i) = self.cert_pairs.remove(&ev.cert.0) else {
            return;
        };
        if i + 1 >= self.order.len() {
            return;
        }
        self.order.swap(i, i + 1);
        if i > 0 {
            self.schedule_pair(i - 1);
        }
        self.schedule_pair(i);
        if i + 2 < self.order.len() {
            self.schedule_pair(i + 1);
        }
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
            self.reschedule_all();
        }
    }
}

impl KineticStructure for KineticSorting {
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

    fn change_motion(&mut self, _id: usize) {
        self.reschedule_all();
    }

    fn certificate_count(&self) -> usize {
        self.queue.len()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::framework::KineticStructure;
    use crate::kinetic_verify::{
        assert_order_sorted, brute_sorted_order, sample_times,
    };
    use crate::motion::LinearMotion2d;

    fn x_motions(motions: &[LinearMotion2d]) -> Vec<LinearMotion1d> {
        motions
            .iter()
            .map(|m| LinearMotion1d::new(m.x0, m.vx))
            .collect()
    }

    #[test]
    fn swap_at_exact_crossing_time() {
        let scalar = vec![
            LinearMotion1d::new(0.0, 2.0),
            LinearMotion1d::new(5.0, 0.0),
        ];
        let meet = scalar[0].meet_time_after(&scalar[1], 0.0).unwrap();
        assert!((meet - 2.5).abs() < 1e-9);

        let mut k = KineticSorting::new(scalar.clone());
        k.advance_to(meet - 1e-6);
        assert_eq!(k.order(), vec![0, 1]);

        k.advance_to(meet + 1e-6);
        assert_eq!(k.order(), vec![1, 0]);
    }

    #[test]
    fn order_matches_brute_force_over_time() {
        let motions_2d = vec![
            LinearMotion2d::new(0.0, 0.0, 2.0, 0.0),
            LinearMotion2d::new(5.0, 0.0, 0.0, 0.0),
            LinearMotion2d::new(-3.0, 0.0, 1.5, 0.0),
        ];
        let scalar = x_motions(&motions_2d);
        let mut k = KineticSorting::from_2d_x(&motions_2d);

        for t in sample_times(6.0, 24) {
            k.advance_to(t);
            let got = k.order();
            let expect = brute_sorted_order(&scalar, t);
            assert_eq!(got, expect, "order mismatch at t={t}");
            assert_order_sorted(&scalar, &got, t);
        }
    }

    #[test]
    fn predecessor_tracks_motion() {
        let mut k = KineticSorting::new(vec![
            LinearMotion1d::new(0.0, 1.0),
            LinearMotion1d::new(10.0, -0.5),
            LinearMotion1d::new(5.0, 0.0),
        ]);
        k.advance_to(4.0);
        // values: 4, 8, 5 -> order [0, 2, 1]
        assert_eq!(k.order(), vec![0, 2, 1]);
        assert_eq!(k.predecessor(2), Some(0));
        assert_eq!(k.predecessor(1), Some(2));
        assert_eq!(k.predecessor(0), None);
    }

    #[test]
    fn processes_events_on_swap() {
        let mut k = KineticSorting::new(vec![
            LinearMotion1d::new(0.0, 1.0),
            LinearMotion1d::new(10.0, 0.0),
        ]);
        let before = k.certificate_count();
        assert!(before >= 1);
        let report = k.advance_to(10.0);
        assert!(report.events_processed >= 1);
        assert_eq!(k.now(), 10.0);
    }
}
