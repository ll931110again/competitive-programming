//! Trajectory models for kinetic data structures.

/// One-dimensional affine motion f(t) = position + velocity * t.
#[derive(Clone, Copy, Debug, PartialEq)]
pub struct LinearMotion1d {
    pub position: f64,
    pub velocity: f64,
}

impl LinearMotion1d {
    pub const fn new(position: f64, velocity: f64) -> Self {
        Self { position, velocity }
    }

    pub fn at(&self, t: f64) -> f64 {
        self.position + self.velocity * t
    }

    /// Earliest t >= `from` where `self(t) == other(t)`, if any.
    pub fn meet_time_after(&self, other: &Self, from: f64) -> Option<f64> {
        let dv = self.velocity - other.velocity;
        let dp = other.position - self.position;
        if dv.abs() < 1e-12 {
            return None;
        }
        let t = dp / dv;
        if t >= from - 1e-12 {
            Some(t.max(from))
        } else {
            None
        }
    }
}

/// Two-dimensional linear motion.
#[derive(Clone, Copy, Debug, PartialEq)]
pub struct LinearMotion2d {
    pub x0: f64,
    pub y0: f64,
    pub vx: f64,
    pub vy: f64,
}

impl LinearMotion2d {
    pub const fn new(x0: f64, y0: f64, vx: f64, vy: f64) -> Self {
        Self { x0, y0, vx, vy }
    }

    pub fn at(&self, t: f64) -> (f64, f64) {
        (self.x0 + self.vx * t, self.y0 + self.vy * t)
    }

    pub fn distance_sq_to(&self, other: &Self, t: f64) -> f64 {
        let (x1, y1) = self.at(t);
        let (x2, y2) = other.at(t);
        let dx = x1 - x2;
        let dy = y1 - y2;
        dx * dx + dy * dy
    }
}

/// Counter-clockwise orientation: positive if c is left of directed line a -> b.
pub fn orient2d(ax: f64, ay: f64, bx: f64, by: f64, cx: f64, cy: f64) -> f64 {
    (bx - ax) * (cy - ay) - (by - ay) * (cx - ax)
}

/// Solve for earliest t >= `from` where orient2d(a(t), b(t), c) = 0 with linear motions.
/// c is fixed; a and b move linearly.
pub fn orient_failure_time(
    a: LinearMotion2d,
    b: LinearMotion2d,
    cx: f64,
    cy: f64,
    from: f64,
) -> Option<f64> {
    // orient(t) = (b(t)-a(t)) x (c-a(t)) is quadratic in t for linear motions.
    let (ax0, ay0) = a.at(0.0);
    let (bx0, by0) = b.at(0.0);
    let b0x = bx0 - ax0;
    let b0y = by0 - ay0;
    let b1x = b.vx - a.vx;
    let b1y = b.vy - a.vy;
    let c0x = cx - ax0;
    let c0y = cy - ay0;
    let c1x = -a.vx;
    let c1y = -a.vy;

    let p0 = b0x * c0y - b0y * c0x;
    let p1 = b0x * c1y - b0y * c1x + b1x * c0y - b1y * c0x;
    let p2 = b1x * c1y - b1y * c1x;

    solve_quadratic_earliest(p2, p1, p0, from)
}

fn solve_quadratic_earliest(a: f64, b: f64, c: f64, from: f64) -> Option<f64> {
    let mut candidates = Vec::new();
    if a.abs() < 1e-12 {
        if b.abs() > 1e-12 {
            let t = -c / b;
            if t >= from - 1e-9 {
                candidates.push(t);
            }
        }
    } else {
        let disc = b * b - 4.0 * a * c;
        if disc >= 0.0 {
            let s = disc.sqrt();
            let t1 = (-b - s) / (2.0 * a);
            let t2 = (-b + s) / (2.0 * a);
            if t1 >= from - 1e-9 {
                candidates.push(t1);
            }
            if t2 >= from - 1e-9 {
                candidates.push(t2);
            }
        }
    }
    candidates.into_iter().min_by(|x, y| x.partial_cmp(y).unwrap())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn linear_position_over_time() {
        let m = LinearMotion1d::new(3.0, -2.0);
        assert!((m.at(0.0) - 3.0).abs() < 1e-12);
        assert!((m.at(2.5) - (-2.0)).abs() < 1e-12);
    }

    #[test]
    fn linear_2d_position_over_time() {
        let m = LinearMotion2d::new(1.0, 2.0, 3.0, -1.0);
        let (x, y) = m.at(4.0);
        assert!((x - 13.0).abs() < 1e-12);
        assert!((y - (-2.0)).abs() < 1e-12);
    }

    #[test]
    fn meet_time() {
        let a = LinearMotion1d::new(0.0, 1.0);
        let b = LinearMotion1d::new(10.0, 0.0);
        assert!((a.meet_time_after(&b, 0.0).unwrap() - 10.0).abs() < 1e-9);
    }

    #[test]
    fn parallel_lines_never_meet() {
        let a = LinearMotion1d::new(0.0, 1.0);
        let b = LinearMotion1d::new(5.0, 1.0);
        assert!(a.meet_time_after(&b, 0.0).is_none());
    }

    #[test]
    fn meet_time_respects_from_bound() {
        let a = LinearMotion1d::new(0.0, 1.0);
        let b = LinearMotion1d::new(10.0, 0.0);
        // crossing at t=10; querying from t=15 should return None
        assert!(a.meet_time_after(&b, 15.0).is_none());
        assert!((a.meet_time_after(&b, 10.0).unwrap() - 10.0).abs() < 1e-9);
    }

    #[test]
    fn distance_sq_shrinks_as_points_approach() {
        let a = LinearMotion2d::new(0.0, 0.0, 1.0, 0.0);
        let b = LinearMotion2d::new(10.0, 0.0, -1.0, 0.0);
        let d0 = a.distance_sq_to(&b, 0.0);
        let d5 = a.distance_sq_to(&b, 5.0);
        assert!(d5 < d0);
        assert!(d5.abs() < 1e-9); // meet at t=5
    }

    #[test]
    fn orient_collinear_midpoint() {
        let o = orient2d(0.0, 0.0, 2.0, 0.0, 1.0, 0.0);
        assert!(o.abs() < 1e-9);
    }

    #[test]
    fn orient_failure_when_segment_rotates_past_fixed_point() {
        let a = LinearMotion2d::new(0.0, 0.0, 0.0, 0.0);
        let b = LinearMotion2d::new(2.0, 0.0, 0.0, 1.0);
        let t = orient_failure_time(a, b, 1.0, 1.0, 0.0).unwrap();
        // b moves up; orientation of (a,b,c) becomes zero when b reaches (2,1) level with c
        assert!(t > 0.0);
        let (ax, ay) = a.at(t);
        let (bx, by) = b.at(t);
        assert!(orient2d(ax, ay, bx, by, 1.0, 1.0).abs() < 1e-6);
    }
}
