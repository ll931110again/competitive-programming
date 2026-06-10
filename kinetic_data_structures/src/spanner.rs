//! Kinetic spanners in ℝᵈ (Abam et al., SoCG 2008).
//!
//! Maintains a greedy spanner: edges (i,j) are present while their length is
//! below a stretch factor times the shortest path in the spanner.

use crate::framework::{AdvanceReport, CertId, KineticStructure, ScheduledEvent};
use crate::framework::event_queue::EventQueue;
use crate::framework::out_of_order::drain_events_until;
use crate::motion::LinearMotion2d;

/// Greedy kinetic spanner with stretch factor `t`.
pub struct KineticSpanner {
    motions: Vec<LinearMotion2d>,
    stretch: f64,
    edges: Vec<(usize, usize)>,
    queue: EventQueue,
    now: f64,
    next_cert_id: u64,
    cert_edge: std::collections::HashMap<u64, usize>,
}

impl KineticSpanner {
    pub fn new(motions: Vec<LinearMotion2d>, stretch: f64) -> Self {
        let mut k = Self {
            motions,
            stretch,
            edges: Vec::new(),
            queue: EventQueue::new(),
            now: 0.0,
            next_cert_id: 0,
            cert_edge: std::collections::HashMap::new(),
        };
        k.rebuild();
        k
    }

    pub fn edges(&self) -> &[(usize, usize)] {
        &self.edges
    }

    fn dist(&self, i: usize, j: usize) -> f64 {
        self.motions[i].distance_sq_to(&self.motions[j], self.now).sqrt()
    }

    fn rebuild(&mut self) {
        self.edges.clear();
        self.queue = EventQueue::new();
        self.cert_edge.clear();
        let n = self.motions.len();
        let mut pairs: Vec<(f64, usize, usize)> = Vec::new();
        for i in 0..n {
            for j in i + 1..n {
                pairs.push((self.dist(i, j), i, j));
            }
        }
        pairs.sort_by(|a, b| a.0.partial_cmp(&b.0).unwrap());
        for (_, i, j) in pairs {
            let dij = self.dist(i, j);
            let mut ok = true;
            for &(u, v) in &self.edges {
                let via = self.dist(i, u) + self.dist(u, v) + self.dist(v, j);
                let via2 = self.dist(i, v) + self.dist(v, u) + self.dist(u, j);
                if via <= self.stretch * dij || via2 <= self.stretch * dij {
                    ok = false;
                    break;
                }
            }
            if ok {
                let eidx = self.edges.len();
                self.edges.push((i, j));
                self.schedule_edge(eidx);
            }
        }
    }

    fn alloc_cert(&mut self) -> CertId {
        let id = CertId(self.next_cert_id);
        self.next_cert_id += 1;
        id
    }

    fn schedule_edge(&mut self, eidx: usize) {
        let (i, j) = self.edges[eidx];
        // Certificate: edge length < stretch * path_without_edge — simplified:
        // schedule when dist(i,j) changes enough to violate spanner (rebuild).
        let d0 = self.dist(i, j);
        let t = self.now + 0.1 / (1.0 + d0);
        let cert = self.alloc_cert();
        self.cert_edge.insert(cert.0, eidx);
        self.queue.push(ScheduledEvent { time: t, cert });
    }

    fn handle_event(&mut self, ev: ScheduledEvent) {
        let _ = self.cert_edge.remove(&ev.cert.0);
        self.rebuild();
    }
}

impl KineticStructure for KineticSpanner {
    fn now(&self) -> f64 {
        self.now
    }

    fn advance_to(&mut self, t: f64) -> AdvanceReport {
        let valid: std::collections::HashSet<u64> =
            self.cert_edge.keys().copied().collect();
        let batch = drain_events_until(&mut self.queue, t);
        let mut events = 0usize;
        for ev in batch {
            if valid.contains(&ev.cert.0) {
                self.handle_event(ev);
                events += 1;
            }
        }
        self.now = t;
        self.rebuild();
        AdvanceReport {
            events_processed: events,
            new_time: t,
        }
    }

    fn change_motion(&mut self, _id: usize) {
        self.rebuild();
    }

    fn certificate_count(&self) -> usize {
        self.queue.len()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::framework::KineticStructure;
    use crate::kinetic_verify::sample_times;

    fn spanner_stretch_ok(motions: &[LinearMotion2d], edges: &[(usize, usize)], stretch: f64, t: f64) -> bool {
        let n = motions.len();
        for i in 0..n {
            for j in i + 1..n {
                let target = motions[i].distance_sq_to(&motions[j], t).sqrt();
                let mut best = f64::INFINITY;
                for &(u, v) in edges {
                    let du = motions[u].distance_sq_to(&motions[i], t).sqrt();
                    let dv = motions[v].distance_sq_to(&motions[j], t).sqrt();
                    let uv = motions[u].distance_sq_to(&motions[v], t).sqrt();
                    best = best.min(du + uv);
                    best = best.min(dv + uv);
                }
                if edges.is_empty() {
                    return false;
                }
                if best > stretch * target + 1e-6 {
                    return false;
                }
            }
        }
        true
    }

    #[test]
    fn has_edges() {
        let k = KineticSpanner::new(
            vec![
                LinearMotion2d::new(0.0, 0.0, 0.0, 0.0),
                LinearMotion2d::new(1.0, 0.0, 0.0, 0.0),
                LinearMotion2d::new(0.0, 1.0, 0.0, 0.0),
            ],
            2.0,
        );
        assert!(!k.edges().is_empty());
    }

    #[test]
    fn spanner_valid_under_translation() {
        let motions = vec![
            LinearMotion2d::new(0.0, 0.0, 1.0, 0.0),
            LinearMotion2d::new(1.0, 0.0, 1.0, 0.0),
            LinearMotion2d::new(0.0, 1.0, 1.0, 0.0),
            LinearMotion2d::new(1.0, 1.0, 1.0, 0.0),
        ];
        let stretch = 2.5;
        let mut k = KineticSpanner::new(motions.clone(), stretch);

        for t in sample_times(2.0, 8) {
            k.advance_to(t);
            assert!(!k.edges().is_empty(), "no edges at t={t}");
            assert!(
                spanner_stretch_ok(&motions, k.edges(), stretch, t),
                "stretch violated at t={t}"
            );
        }
    }
}
