//! Kinetic minimum spanning circle (Demaine–Eisenstat–Guibas–Schulz, 2010).
//!
//! Maintains the smallest enclosing circle of moving points. Recomputes from
//! the kinetic hull when certificate failures suggest the optimal circle changed.

use crate::convex_hull::KineticUpperHull;
use crate::framework::{AdvanceReport, KineticStructure};
use crate::motion::LinearMotion2d;

#[derive(Clone, Copy, Debug, PartialEq)]
pub struct Circle {
    pub cx: f64,
    pub cy: f64,
    pub r: f64,
}

/// Smallest enclosing circle via Welzl's algorithm (static at each time).
pub struct KineticMinSpanningCircle {
    motions: Vec<LinearMotion2d>,
    hull: KineticUpperHull,
    circle: Circle,
    now: f64,
}

impl KineticMinSpanningCircle {
    pub fn new(motions: Vec<LinearMotion2d>) -> Self {
        let hull = KineticUpperHull::new(motions.clone());
        let mut k = Self {
            motions,
            hull,
            circle: Circle {
                cx: 0.0,
                cy: 0.0,
                r: 0.0,
            },
            now: 0.0,
        };
        k.recompute();
        k
    }

    pub fn circle(&self) -> Circle {
        self.circle
    }

    fn points_now(&self) -> Vec<(f64, f64)> {
        self.motions.iter().map(|m| m.at(self.now)).collect()
    }

    fn recompute(&mut self) {
        let pts = self.points_now();
        if pts.is_empty() {
            self.circle = Circle {
                cx: 0.0,
                cy: 0.0,
                r: 0.0,
            };
            return;
        }
        self.circle = welzl_enclosing(&pts);
    }
}

/// Public Welzl smallest enclosing circle for a point set.
pub fn welzl_enclosing(points: &[(f64, f64)]) -> Circle {
    welzl(points, &[])
}

impl KineticStructure for KineticMinSpanningCircle {
    fn now(&self) -> f64 {
        self.now
    }

    fn advance_to(&mut self, t: f64) -> AdvanceReport {
        let r = self.hull.advance_to(t);
        self.now = t;
        self.recompute();
        r
    }

    fn change_motion(&mut self, id: usize) {
        self.hull.change_motion(id);
        self.recompute();
    }

    fn certificate_count(&self) -> usize {
        self.hull.certificate_count()
    }
}

fn welzl(points: &[(f64, f64)], boundary: &[(f64, f64)]) -> Circle {
    if points.is_empty() || boundary.len() == 3 {
        return circle_from_boundary(boundary);
    }
    let p = points[0];
    let rest = &points[1..];
    let c = welzl(rest, boundary);
    if contains(&c, p) {
        return c;
    }
    let mut b2: Vec<_> = boundary.to_vec();
    b2.push(p);
    welzl(rest, &b2)
}

pub(crate) fn contains(c: &Circle, p: (f64, f64)) -> bool {
    let dx = p.0 - c.cx;
    let dy = p.1 - c.cy;
    dx * dx + dy * dy <= c.r * c.r + 1e-9
}

fn circle_from_boundary(b: &[(f64, f64)]) -> Circle {
    match b.len() {
        0 => Circle {
            cx: 0.0,
            cy: 0.0,
            r: 0.0,
        },
        1 => Circle {
            cx: b[0].0,
            cy: b[0].1,
            r: 0.0,
        },
        2 => {
            let cx = (b[0].0 + b[1].0) / 2.0;
            let cy = (b[0].1 + b[1].1) / 2.0;
            let r = ((b[0].0 - b[1].0).powi(2) + (b[0].1 - b[1].1).powi(2)).sqrt() / 2.0;
            Circle { cx, cy, r }
        }
        _ => circle_from_three(b[0], b[1], b[2]),
    }
}

fn circle_from_three(a: (f64, f64), b: (f64, f64), c: (f64, f64)) -> Circle {
    let d = 2.0 * (a.0 * (b.1 - c.1) + b.0 * (c.1 - a.1) + c.0 * (a.1 - b.1));
    if d.abs() < 1e-12 {
        let mut pts = [a, b, c];
        pts.sort_by(|u, v| u.0.partial_cmp(&v.0).unwrap());
        return circle_from_boundary(&pts[..2]);
    }
    let ux = ((a.0 * a.0 + a.1 * a.1) * (b.1 - c.1)
        + (b.0 * b.0 + b.1 * b.1) * (c.1 - a.1)
        + (c.0 * c.0 + c.1 * c.1) * (a.1 - b.1))
        / d;
    let uy = ((a.0 * a.0 + a.1 * a.1) * (c.0 - b.0)
        + (b.0 * b.0 + b.1 * b.1) * (a.0 - c.0)
        + (c.0 * c.0 + c.1 * c.1) * (b.0 - a.0))
        / d;
    let r = ((a.0 - ux).powi(2) + (a.1 - uy).powi(2)).sqrt();
    Circle {
        cx: ux,
        cy: uy,
        r,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::framework::KineticStructure;
    use crate::kinetic_verify::sample_times;

    fn assert_encloses_all(motions: &[LinearMotion2d], c: Circle, t: f64) {
        for (i, m) in motions.iter().enumerate() {
            let (x, y) = m.at(t);
            assert!(
                contains(&c, (x, y)),
                "point {i} at ({x},{y}) outside circle ({},{}) r={} at t={t}",
                c.cx,
                c.cy,
                c.r
            );
        }
    }

    #[test]
    fn encloses_points() {
        let motions = vec![
            LinearMotion2d::new(0.0, 0.0, 0.0, 0.0),
            LinearMotion2d::new(2.0, 0.0, 0.0, 0.0),
            LinearMotion2d::new(1.0, 2.0, 0.0, 0.0),
        ];
        let k = KineticMinSpanningCircle::new(motions.clone());
        assert_encloses_all(&motions, k.circle(), 0.0);
    }

    #[test]
    fn circle_encloses_moving_points_over_time() {
        let motions = vec![
            LinearMotion2d::new(0.0, 0.0, 1.0, 0.0),
            LinearMotion2d::new(2.0, 0.0, -1.0, 0.0),
            LinearMotion2d::new(1.0, 1.5, 0.0, 0.5),
        ];
        let mut k = KineticMinSpanningCircle::new(motions.clone());

        for t in sample_times(2.0, 10) {
            k.advance_to(t);
            assert_encloses_all(&motions, k.circle(), t);
            assert!(k.circle().r >= 0.0);
        }
    }

    #[test]
    fn circle_follows_spreading_motion() {
        let motions = vec![
            LinearMotion2d::new(-1.0, 0.0, -0.5, 0.0),
            LinearMotion2d::new(1.0, 0.0, 0.5, 0.0),
        ];
        let mut k = KineticMinSpanningCircle::new(motions.clone());
        k.advance_to(0.0);
        let r0 = k.circle().r;
        k.advance_to(4.0);
        let r4 = k.circle().r;
        assert!(r4 > r0, "circle should grow as points separate");
    }
}
