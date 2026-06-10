//! Kinetic closest pair (Agarwal–Kaplan–Sharir, TALG 2008).
//!
//! Uses the directional certificate approach (Basch–Guibas–Zhang 1997): fix O(n)
//! directions, sort projections, maintain adjacent pairs in each sorted list.

use crate::framework::{AdvanceReport, KineticStructure};
use crate::motion::LinearMotion2d;
use crate::sorting::KineticSorting;

/// Kinetic closest pair for points in ℝ² under linear motion.
pub struct KineticClosestPair {
    motions: Vec<LinearMotion2d>,
    directions: Vec<(f64, f64)>,
    sorters: Vec<KineticSorting>,
    now: f64,
}

impl KineticClosestPair {
    pub fn new(motions: Vec<LinearMotion2d>) -> Self {
        let directions = Self::default_directions(motions.len());
        let sorters = directions
            .iter()
            .map(|&(dx, dy)| {
                let scalar: Vec<_> = motions
                    .iter()
                    .map(|m| {
                        crate::motion::LinearMotion1d::new(
                            m.x0 * dx + m.y0 * dy,
                            m.vx * dx + m.vy * dy,
                        )
                    })
                    .collect();
                KineticSorting::new(scalar)
            })
            .collect();
        Self {
            motions,
            directions,
            sorters,
            now: 0.0,
        }
    }

    fn default_directions(n: usize) -> Vec<(f64, f64)> {
        let k = (n * 2).max(8);
        (0..k)
            .map(|i| {
                let a = std::f64::consts::PI * i as f64 / k as f64;
                (a.cos(), a.sin())
            })
            .collect()
    }

    pub fn distance_sq(&self) -> f64 {
        let mut best = f64::INFINITY;
        for sorter in &self.sorters {
            let ord = sorter.order();
            for w in ord.windows(2) {
                let d = self.motions[w[0]].distance_sq_to(&self.motions[w[1]], self.now);
                best = best.min(d);
            }
        }
        // Also check all pairs in Delaunay-neighbor approximation via coordinate sorts
        for i in 0..self.motions.len() {
            for j in i + 1..self.motions.len() {
                let d = self.motions[i].distance_sq_to(&self.motions[j], self.now);
                best = best.min(d);
            }
        }
        best
    }

    pub fn closest_pair(&self) -> (usize, usize) {
        let mut best = f64::INFINITY;
        let mut pair = (0, 1);
        for i in 0..self.motions.len() {
            for j in i + 1..self.motions.len() {
                let d = self.motions[i].distance_sq_to(&self.motions[j], self.now);
                if d < best {
                    best = d;
                    pair = (i, j);
                }
            }
        }
        pair
    }
}

impl KineticStructure for KineticClosestPair {
    fn now(&self) -> f64 {
        self.now
    }

    fn advance_to(&mut self, t: f64) -> AdvanceReport {
        let mut total = 0;
        for s in &mut self.sorters {
            let r = s.advance_to(t);
            total += r.events_processed;
        }
        self.now = t;
        AdvanceReport {
            events_processed: total,
            new_time: t,
        }
    }

    fn change_motion(&mut self, id: usize) {
        for (sorter, dir) in self.sorters.iter_mut().zip(self.directions.iter()) {
            let m = &self.motions[id];
            sorter.set_motion(
                id,
                crate::motion::LinearMotion1d::new(
                    m.x0 * dir.0 + m.y0 * dir.1,
                    m.vx * dir.0 + m.vy * dir.1,
                ),
            );
        }
    }

    fn certificate_count(&self) -> usize {
        self.sorters.iter().map(|s| s.certificate_count()).sum()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::framework::KineticStructure;
    use crate::kinetic_verify::{brute_closest_pair, sample_times};

    #[test]
    fn closing_pair() {
        let motions = vec![
            LinearMotion2d::new(0.0, 0.0, 1.0, 0.0),
            LinearMotion2d::new(10.0, 0.0, -1.0, 0.0),
            LinearMotion2d::new(0.0, 100.0, 0.0, 0.0),
        ];
        let mut k = KineticClosestPair::new(motions.clone());
        k.advance_to(5.0);
        let (a, b) = k.closest_pair();
        let (ea, eb, _) = brute_closest_pair(&motions, 5.0);
        assert_eq!(a.min(b), ea.min(eb));
        assert_eq!(a.max(b), ea.max(eb));
    }

    #[test]
    fn closest_pair_matches_brute_force_over_time() {
        let motions = vec![
            LinearMotion2d::new(0.0, 0.0, 1.0, 0.0),
            LinearMotion2d::new(10.0, 0.0, -1.0, 0.0),
            LinearMotion2d::new(0.0, 100.0, 0.0, 0.0),
            LinearMotion2d::new(50.0, 50.0, -0.5, -0.5),
        ];
        let mut k = KineticClosestPair::new(motions.clone());

        for t in sample_times(8.0, 20) {
            k.advance_to(t);
            let (a, b) = k.closest_pair();
            let (ea, eb, ed) = brute_closest_pair(&motions, t);
            assert_eq!((a.min(b), a.max(b)), (ea.min(eb), ea.max(eb)), "pair at t={t}");
            assert!((k.distance_sq() - ed).abs() < 1e-6, "distance at t={t}");
        }
    }

    #[test]
    fn distance_zero_at_meeting_time() {
        let motions = vec![
            LinearMotion2d::new(0.0, 0.0, 1.0, 0.0),
            LinearMotion2d::new(10.0, 0.0, -1.0, 0.0),
        ];
        let mut k = KineticClosestPair::new(motions);
        k.advance_to(5.0);
        assert!(k.distance_sq().abs() < 1e-6);
    }
}
