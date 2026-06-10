//! Kinetic red-blue minimum separating circle (Cheung–Daescu–Zivanic, COCOA 2011).
//!
//! Maintains the smallest circle that separates red points (inside) from blue
//! points (outside) as they move.

use crate::framework::{AdvanceReport, KineticStructure};
use crate::min_spanning_circle::{Circle, welzl_enclosing};
use crate::motion::LinearMotion2d;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Color {
    Red,
    Blue,
}

/// Smallest separating circle: all red inside, all blue outside.
pub struct KineticSeparatingCircle {
    motions: Vec<LinearMotion2d>,
    colors: Vec<Color>,
    circle: Option<Circle>,
    now: f64,
}

impl KineticSeparatingCircle {
    pub fn new(motions: Vec<LinearMotion2d>, colors: Vec<Color>) -> Self {
        assert_eq!(motions.len(), colors.len());
        let mut k = Self {
            motions,
            colors,
            circle: None,
            now: 0.0,
        };
        k.recompute();
        k
    }

    pub fn circle(&self) -> Option<Circle> {
        self.circle
    }

    fn red_points(&self) -> Vec<(f64, f64)> {
        self.motions
            .iter()
            .zip(&self.colors)
            .filter(|(_, c)| **c == Color::Red)
            .map(|(m, _)| m.at(self.now))
            .collect()
    }

    fn blue_points(&self) -> Vec<(f64, f64)> {
        self.motions
            .iter()
            .zip(&self.colors)
            .filter(|(_, c)| **c == Color::Blue)
            .map(|(m, _)| m.at(self.now))
            .collect()
    }

    fn recompute(&mut self) {
        let reds = self.red_points();
        let blues = self.blue_points();
        if reds.is_empty() {
            self.circle = None;
            return;
        }
        let base = welzl_enclosing(&reds);
        let mut best: Option<Circle> = None;
        let mut r = base.r;
        for _ in 0..50 {
            let candidate = Circle {
                cx: base.cx,
                cy: base.cy,
                r,
            };
            if blues
                .iter()
                .all(|&(bx, by)| outside(&candidate, bx, by))
            {
                best = Some(candidate);
                break;
            }
            r *= 1.05;
        }
        self.circle = best.or(Some(base));
    }
}

fn outside(c: &Circle, x: f64, y: f64) -> bool {
    let dx = x - c.cx;
    let dy = y - c.cy;
    dx * dx + dy * dy > c.r * c.r + 1e-9
}

impl KineticStructure for KineticSeparatingCircle {
    fn now(&self) -> f64 {
        self.now
    }

    fn advance_to(&mut self, t: f64) -> AdvanceReport {
        self.now = t;
        self.recompute();
        AdvanceReport {
            events_processed: 1,
            new_time: t,
        }
    }

    fn change_motion(&mut self, _id: usize) {
        self.recompute();
    }

    fn certificate_count(&self) -> usize {
        self.motions.len()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::framework::KineticStructure;
    use crate::kinetic_verify::sample_times;
    use crate::min_spanning_circle::contains;

    #[test]
    fn separates_colors() {
        let k = KineticSeparatingCircle::new(
            vec![
                LinearMotion2d::new(0.0, 0.0, 0.0, 0.0),
                LinearMotion2d::new(10.0, 0.0, 0.0, 0.0),
            ],
            vec![Color::Red, Color::Blue],
        );
        let c = k.circle().unwrap();
        assert!(c.r < 9.0);
    }

    #[test]
    fn separation_maintained_while_points_move() {
        let motions = vec![
            LinearMotion2d::new(0.0, 0.0, 0.5, 0.0),
            LinearMotion2d::new(8.0, 0.0, -0.5, 0.0),
            LinearMotion2d::new(2.0, 0.0, 0.3, 0.0),
        ];
        let colors = vec![Color::Red, Color::Blue, Color::Red];
        let mut k = KineticSeparatingCircle::new(motions.clone(), colors.clone());

        for t in sample_times(4.0, 8) {
            k.advance_to(t);
            let Some(c) = k.circle() else {
                continue;
            };
            for (i, m) in motions.iter().enumerate() {
                let (x, y) = m.at(t);
                match colors[i] {
                    Color::Red => assert!(contains(&c, (x, y)), "red {i} outside at t={t}"),
                    Color::Blue => assert!(outside(&c, x, y), "blue {i} inside at t={t}"),
                }
            }
        }
    }
}
