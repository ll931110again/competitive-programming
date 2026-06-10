//! Kinetic convex hull in 2D (Abam & de Berg, SoCG 2005).
//!
//! Maintains the upper hull of points moving linearly in the plane using
//! orientation certificates on a hull-linked list.

use crate::framework::{AdvanceReport, CertId, KineticStructure, ScheduledEvent};
use crate::framework::event_queue::EventQueue;
use crate::framework::out_of_order::drain_events_until;
use crate::kinetic_verify::brute_upper_hull;
use crate::motion::{LinearMotion2d, orient2d, orient_failure_time};

/// Upper convex hull (left-to-right) of moving points.
pub struct KineticUpperHull {
    motions: Vec<LinearMotion2d>,
    /// Hull vertex indices in counter-clockwise order along upper chain.
    hull: Vec<usize>,
    queue: EventQueue,
    now: f64,
    next_cert_id: u64,
    /// cert -> (prev, curr, next) hull indices
    cert_triple: std::collections::HashMap<u64, (usize, usize, usize)>,
}

impl KineticUpperHull {
    pub fn new(motions: Vec<LinearMotion2d>) -> Self {
        let hull = brute_upper_hull(&motions, 0.0);
        let mut k = Self {
            motions,
            hull,
            queue: EventQueue::new(),
            now: 0.0,
            next_cert_id: 0,
            cert_triple: std::collections::HashMap::new(),
        };
        k.reschedule_certificates();
        k
    }

    pub fn hull(&self) -> &[usize] {
        &self.hull
    }

    fn alloc_cert(&mut self) -> CertId {
        let id = CertId(self.next_cert_id);
        self.next_cert_id += 1;
        id
    }

    fn point_at(&self, id: usize) -> (f64, f64) {
        self.motions[id].at(self.now)
    }

    fn ccw(&self, a: usize, b: usize, c: usize) -> bool {
        let (ax, ay) = self.point_at(a);
        let (bx, by) = self.point_at(b);
        let (cx, cy) = self.point_at(c);
        orient2d(ax, ay, bx, by, cx, cy) > 1e-9
    }

    fn reschedule_certificates(&mut self) {
        self.queue = EventQueue::new();
        self.cert_triple.clear();
        for i in 1..self.hull.len().saturating_sub(1) {
            self.schedule_triple(i - 1, i, i + 1);
        }
    }

    fn rebuild_certificates(&mut self) {
        self.hull = brute_upper_hull(&self.motions, self.now);
        self.reschedule_certificates();
    }

    fn schedule_triple(&mut self, pi: usize, ci: usize, ni: usize) {
        let a = self.hull[pi];
        let b = self.hull[ci];
        let c = self.hull[ni];
        let (cx, cy) = self.point_at(c);
        if let Some(t) = orient_failure_time(self.motions[a], self.motions[b], cx, cy, self.now)
        {
            let cert = self.alloc_cert();
            self.cert_triple.insert(cert.0, (pi, ci, ni));
            self.queue.push(ScheduledEvent { time: t, cert });
        }
    }

    fn handle_event(&mut self, ev: ScheduledEvent) {
        let Some((pi, ci, ni)) = self.cert_triple.remove(&ev.cert.0) else {
            return;
        };
        if ci >= self.hull.len() || pi >= self.hull.len() || ni >= self.hull.len() {
            return;
        }
        let a = self.hull[pi];
        let b = self.hull[ci];
        let c = self.hull[ni];
        if !self.ccw(a, b, c) {
            self.hull.remove(ci);
            if pi > 0 && ci > 0 && pi < self.hull.len() && ci < self.hull.len() {
                self.schedule_triple(pi - 1, pi, pi + 1.min(self.hull.len() - 1));
            }
            if ci > 0 && ci + 1 < self.hull.len() {
                let lo = ci - 1;
                let hi = ci;
                if hi + 1 < self.hull.len() {
                    self.schedule_triple(lo, hi, hi + 1);
                }
            }
        } else {
            self.schedule_triple(pi, ci, ni);
        }
    }
}

impl KineticStructure for KineticUpperHull {
    fn now(&self) -> f64 {
        self.now
    }

    fn advance_to(&mut self, t: f64) -> AdvanceReport {
        let valid: std::collections::HashSet<u64> =
            self.cert_triple.keys().copied().collect();
        let batch = drain_events_until(&mut self.queue, t);
        let mut events = 0usize;
        for ev in batch {
            if valid.contains(&ev.cert.0) {
                self.handle_event(ev);
                events += 1;
            }
        }
        // Recompute so interior points that become extreme are inserted.
        self.hull = brute_upper_hull(&self.motions, t);
        self.reschedule_certificates();
        self.now = t;
        AdvanceReport {
            events_processed: events,
            new_time: t,
        }
    }

    fn change_motion(&mut self, _id: usize) {
        self.rebuild_certificates();
    }

    fn certificate_count(&self) -> usize {
        self.queue.len()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::framework::KineticStructure;
    use crate::kinetic_verify::{brute_upper_hull, sample_times};
    use crate::motion::orient2d;

    fn assert_is_upper_hull(motions: &[LinearMotion2d], hull: &[usize], t: f64) {
        assert!(hull.len() >= 2);
        for w in hull.windows(2) {
            let (ax, _) = motions[w[0]].at(t);
            let (bx, _) = motions[w[1]].at(t);
            assert!(bx >= ax - 1e-9, "hull not left-to-right at t={t}");
        }
        for w in hull.windows(2) {
            let (ax, ay) = motions[w[0]].at(t);
            let (bx, by) = motions[w[1]].at(t);
            for (i, m) in motions.iter().enumerate() {
                let (px, py) = m.at(t);
                assert!(
                    orient2d(ax, ay, bx, by, px, py) <= 1e-6,
                    "point {i} above upper hull edge at t={t}"
                );
            }
        }
    }

    #[test]
    fn collinear_middle_drops() {
        let motions = vec![
            LinearMotion2d::new(0.0, 0.0, 1.0, 0.0),
            LinearMotion2d::new(1.0, 0.0, 1.0, 0.0),
            LinearMotion2d::new(2.0, 0.0, 1.0, 0.0),
        ];
        let mut h = KineticUpperHull::new(motions.clone());
        assert_eq!(h.hull(), &[0, 2]);
        h.advance_to(1.0);
        assert_eq!(h.hull(), brute_upper_hull(&motions, 1.0));
    }

    #[test]
    fn hull_matches_brute_force_while_translating() {
        let motions = vec![
            LinearMotion2d::new(0.0, 0.0, 1.0, 0.0),
            LinearMotion2d::new(1.0, 1.0, 1.0, 0.0),
            LinearMotion2d::new(2.0, 0.0, 1.0, 0.0),
            LinearMotion2d::new(1.0, 0.5, 1.0, 0.0),
        ];
        let mut h = KineticUpperHull::new(motions.clone());

        for t in sample_times(3.0, 15) {
            h.advance_to(t);
            let got = h.hull().to_vec();
            let expect = brute_upper_hull(&motions, t);
            assert_eq!(got, expect, "upper hull mismatch at t={t}");
            assert_is_upper_hull(&motions, &got, t);
        }
    }

    #[test]
    fn hull_updates_when_interior_point_becomes_extreme() {
        let motions = vec![
            LinearMotion2d::new(0.0, 0.0, 0.0, 0.0),
            LinearMotion2d::new(2.0, 0.0, 0.0, 0.0),
            LinearMotion2d::new(1.0, 0.0, 0.0, 1.0), // rises, becomes hull vertex
        ];
        let mut h = KineticUpperHull::new(motions.clone());
        h.advance_to(0.0);
        assert_eq!(h.hull(), vec![0, 1]);

        h.advance_to(1.0);
        let got = h.hull().to_vec();
        assert!(got.contains(&2), "rising point should join hull by t=1");
        assert_eq!(got, brute_upper_hull(&motions, 1.0));
    }
}
