//! Black-box kinetic model (de Berg–Roeloffzen–Speckmann, SoCG 2011).
//!
//! Wraps any kinetic structure that exposes only black-box geometric primitives:
//! the wrapper resamples object positions and triggers rebuilds when the black-box
//! prediction interval expires.

use crate::framework::{AdvanceReport, KineticStructure};

/// Prediction interval for black-box motion oracles.
#[derive(Clone, Copy, Debug)]
pub struct PredictionInterval {
    pub valid_until: f64,
}

/// Trait for black-box oracles (positions only, no explicit trajectory).
pub trait BlackBoxOracle {
    fn position(&self, id: usize, t: f64) -> (f64, f64);
    fn prediction_interval(&self, id: usize, from: f64) -> PredictionInterval;
}

/// Wraps a kinetic structure with periodic black-box resampling.
pub struct BlackBoxKds<K> {
    inner: K,
    oracle: Box<dyn BlackBoxOracle>,
    n: usize,
    intervals: Vec<PredictionInterval>,
    now: f64,
}

impl<K: KineticStructure> BlackBoxKds<K> {
    pub fn new(inner: K, oracle: Box<dyn BlackBoxOracle>, n: usize) -> Self {
        let intervals = vec![PredictionInterval { valid_until: 0.0 }; n];
        Self {
            inner,
            oracle,
            n,
            intervals,
            now: 0.0,
        }
    }

    pub fn inner(&self) -> &K {
        &self.inner
    }

    pub fn inner_mut(&mut self) -> &mut K {
        &mut self.inner
    }

    fn refresh_expired(&mut self) {
        for id in 0..self.n {
            if self.intervals[id].valid_until <= self.now + 1e-12 {
                self.inner.change_motion(id);
                self.intervals[id] = self.oracle.prediction_interval(id, self.now);
            }
        }
    }
}

impl<K: KineticStructure> KineticStructure for BlackBoxKds<K> {
    fn now(&self) -> f64 {
        self.now
    }

    fn advance_to(&mut self, t: f64) -> AdvanceReport {
        self.refresh_expired();
        let mut report = self.inner.advance_to(t);
        self.now = t;
        self.refresh_expired();
        report.new_time = t;
        report
    }

    fn change_motion(&mut self, id: usize) {
        self.inner.change_motion(id);
        self.intervals[id] = self.oracle.prediction_interval(id, self.now);
    }

    fn certificate_count(&self) -> usize {
        self.inner.certificate_count()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::convex_hull::KineticUpperHull;
    use crate::motion::LinearMotion2d;

    struct LinearOracle {
        motions: Vec<LinearMotion2d>,
    }

    impl BlackBoxOracle for LinearOracle {
        fn position(&self, id: usize, t: f64) -> (f64, f64) {
            self.motions[id].at(t)
        }

        fn prediction_interval(&self, _id: usize, from: f64) -> PredictionInterval {
            PredictionInterval {
                valid_until: from + 1.0,
            }
        }
    }

    #[test]
    fn wraps_hull() {
        let motions = vec![
            LinearMotion2d::new(0.0, 0.0, 1.0, 0.0),
            LinearMotion2d::new(2.0, 0.0, 1.0, 0.0),
        ];
        let hull = KineticUpperHull::new(motions.clone());
        let oracle = Box::new(LinearOracle { motions: motions.clone() });
        let mut bb = BlackBoxKds::new(hull, oracle, 2);
        bb.advance_to(0.5);
        assert_eq!(bb.inner().hull().len(), 2);
    }

    #[test]
    fn black_box_tracks_moving_hull() {
        use crate::framework::KineticStructure;
        use crate::kinetic_verify::{brute_upper_hull, sample_times};

        let motions = vec![
            LinearMotion2d::new(0.0, 0.0, 1.0, 0.0),
            LinearMotion2d::new(2.0, 1.0, 1.0, 0.0),
            LinearMotion2d::new(4.0, 0.0, 1.0, 0.0),
        ];
        let hull = KineticUpperHull::new(motions.clone());
        let oracle = Box::new(LinearOracle {
            motions: motions.clone(),
        });
        let mut bb = BlackBoxKds::new(hull, oracle, motions.len());

        for t in sample_times(2.0, 6) {
            bb.advance_to(t);
            assert_eq!(
                bb.inner().hull(),
                brute_upper_hull(&motions, t).as_slice(),
                "black-box hull mismatch at t={t}"
            );
        }
    }
}
