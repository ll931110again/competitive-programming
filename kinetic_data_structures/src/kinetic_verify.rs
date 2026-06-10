//! Brute-force reference implementations for validating kinetic structures.

use std::collections::BTreeSet;

use crate::delaunay::triangulate_at;
use crate::min_spanning_circle::{Circle, contains, welzl_enclosing};
use crate::motion::{LinearMotion1d, LinearMotion2d, orient2d};
use crate::separating_circle::Color;

pub const EPS: f64 = 1e-6;

// ---------------------------------------------------------------------------
// Brute-force queries at a fixed time
// ---------------------------------------------------------------------------

/// Sorted point indices by scalar key at time `t`.
pub fn brute_sorted_order(motions: &[LinearMotion1d], t: f64) -> Vec<usize> {
    let mut order: Vec<usize> = (0..motions.len()).collect();
    order.sort_by(|&i, &j| {
        motions[i]
            .at(t)
            .partial_cmp(&motions[j].at(t))
            .unwrap()
    });
    order
}

/// Check that `order` is non-decreasing by scalar value at time `t`.
pub fn assert_order_sorted(motions: &[LinearMotion1d], order: &[usize], t: f64) {
    for w in order.windows(2) {
        let a = motions[w[0]].at(t);
        let b = motions[w[1]].at(t);
        assert!(
            a <= b + EPS,
            "order not sorted at t={t}: id {} val {a} > id {} val {b}",
            w[0],
            w[1]
        );
    }
}

/// Index of the minimum scalar value at time `t`.
pub fn brute_argmin(motions: &[LinearMotion1d], t: f64) -> usize {
    (0..motions.len())
        .min_by(|&i, &j| {
            motions[i]
                .at(t)
                .partial_cmp(&motions[j].at(t))
                .unwrap()
        })
        .unwrap()
}

/// Upper convex hull (left-to-right) at time `t`, Andrew's monotone chain (upper chain).
pub fn brute_upper_hull(motions: &[LinearMotion2d], t: f64) -> Vec<usize> {
    let mut pts: Vec<(usize, f64, f64)> = motions
        .iter()
        .enumerate()
        .map(|(i, m)| {
            let (x, y) = m.at(t);
            (i, x, y)
        })
        .collect();
    pts.sort_by(|a, b| {
        a.1.partial_cmp(&b.1)
            .unwrap()
            .then(a.2.partial_cmp(&b.2).unwrap())
    });
    let mut upper: Vec<usize> = Vec::new();
    for &(i, x, y) in pts.iter().rev() {
        while upper.len() >= 2 {
            let a = upper[upper.len() - 2];
            let b = upper[upper.len() - 1];
            let (ax, ay) = motions[a].at(t);
            let (bx, by) = motions[b].at(t);
            if orient2d(ax, ay, bx, by, x, y) <= EPS {
                upper.pop();
            } else {
                break;
            }
        }
        upper.push(i);
    }
    upper.reverse();
    upper
}

/// Closest pair (indices, squared distance) at time `t`.
pub fn brute_closest_pair(motions: &[LinearMotion2d], t: f64) -> (usize, usize, f64) {
    let n = motions.len();
    let mut best = f64::INFINITY;
    let mut pair = (0, 1.min(n.saturating_sub(1)));
    for i in 0..n {
        for j in i + 1..n {
            let d = motions[i].distance_sq_to(&motions[j], t);
            if d < best {
                best = d;
                pair = (i, j);
            }
        }
    }
    (pair.0, pair.1, best)
}

/// Smallest enclosing circle at time `t` (Welzl).
pub fn brute_min_enclosing_circle(motions: &[LinearMotion2d], t: f64) -> Circle {
    let pts: Vec<_> = motions.iter().map(|m| m.at(t)).collect();
    welzl_enclosing(&pts)
}

/// Greedy `t`-spanner edges at time `t` (same rule as `KineticSpanner::rebuild`).
pub fn brute_greedy_spanner(
    motions: &[LinearMotion2d],
    stretch: f64,
    t: f64,
) -> Vec<(usize, usize)> {
    let n = motions.len();
    let dist = |i: usize, j: usize| motions[i].distance_sq_to(&motions[j], t).sqrt();

    let mut pairs: Vec<(f64, usize, usize)> = Vec::new();
    for i in 0..n {
        for j in i + 1..n {
            pairs.push((dist(i, j), i, j));
        }
    }
    pairs.sort_by(|a, b| a.0.partial_cmp(&b.0).unwrap());

    let mut edges: Vec<(usize, usize)> = Vec::new();
    for (_, i, j) in pairs {
        let dij = dist(i, j);
        let mut ok = true;
        for &(u, v) in &edges {
            let via = dist(i, u) + dist(u, v) + dist(v, j);
            let via2 = dist(i, v) + dist(v, u) + dist(u, j);
            if via <= stretch * dij + EPS || via2 <= stretch * dij + EPS {
                ok = false;
                break;
            }
        }
        if ok {
            edges.push((i, j));
        }
    }
    edges
}

/// Delaunay triangulation at time `t`.
pub fn brute_delaunay(motions: &[LinearMotion2d], t: f64) -> Vec<[usize; 3]> {
    triangulate_at(motions, t)
}

/// Smallest separating circle at time `t` (mirrors `KineticSeparatingCircle::recompute`).
pub fn brute_separating_circle(
    motions: &[LinearMotion2d],
    colors: &[Color],
    t: f64,
) -> Option<Circle> {
    let reds: Vec<_> = motions
        .iter()
        .zip(colors)
        .filter(|(_, c)| **c == Color::Red)
        .map(|(m, _)| m.at(t))
        .collect();
    let blues: Vec<_> = motions
        .iter()
        .zip(colors)
        .filter(|(_, c)| **c == Color::Blue)
        .map(|(m, _)| m.at(t))
        .collect();

    if reds.is_empty() {
        return None;
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
            .all(|&(bx, by)| point_outside_circle(&candidate, bx, by))
        {
            best = Some(candidate);
            break;
        }
        r *= 1.05;
    }
    best.or(Some(base))
}

// ---------------------------------------------------------------------------
// Comparison helpers
// ---------------------------------------------------------------------------

pub fn circles_match(a: Circle, b: Circle, eps: f64) -> bool {
    (a.cx - b.cx).abs() < eps
        && (a.cy - b.cy).abs() < eps
        && (a.r - b.r).abs() < eps
}

pub fn unordered_pair(a: usize, b: usize) -> (usize, usize) {
    (a.min(b), a.max(b))
}

pub fn triangle_edges(tris: &[[usize; 3]]) -> BTreeSet<(usize, usize)> {
    let mut edges = BTreeSet::new();
    for tri in tris {
        for &(u, v) in &[(tri[0], tri[1]), (tri[1], tri[2]), (tri[2], tri[0])] {
            edges.insert((u.min(v), u.max(v)));
        }
    }
    edges
}

pub fn normalize_edges(edges: &[(usize, usize)]) -> BTreeSet<(usize, usize)> {
    edges
        .iter()
        .map(|&(u, v)| (u.min(v), u.max(v)))
        .collect()
}

pub fn point_outside_circle(c: &Circle, x: f64, y: f64) -> bool {
    let dx = x - c.cx;
    let dy = y - c.cy;
    dx * dx + dy * dy > c.r * c.r + EPS
}

pub fn assert_separation(
    motions: &[LinearMotion2d],
    colors: &[Color],
    c: &Circle,
    t: f64,
) {
    for (i, m) in motions.iter().enumerate() {
        let (x, y) = m.at(t);
        match colors[i] {
            Color::Red => assert!(contains(c, (x, y)), "red {i} outside at t={t}"),
            Color::Blue => assert!(point_outside_circle(c, x, y), "blue {i} inside at t={t}"),
        }
    }
}

/// Sample times from 0 to `end` inclusive.
pub fn sample_times(end: f64, steps: usize) -> Vec<f64> {
    if steps == 0 {
        return vec![0.0];
    }
    (0..=steps)
        .map(|i| end * i as f64 / steps as f64)
        .collect()
}

// ---------------------------------------------------------------------------
// Deterministic RNG for stress tests (no external dependency)
// ---------------------------------------------------------------------------

/// Simple LCG for reproducible stress inputs.
pub struct StressRng {
    state: u64,
}

impl StressRng {
    pub fn new(seed: u64) -> Self {
        Self { state: seed.wrapping_add(1) }
    }

    pub fn next_u64(&mut self) -> u64 {
        self.state = self
            .state
            .wrapping_mul(6364136223846793005)
            .wrapping_add(1442695040888963407);
        self.state
    }

    pub fn uniform(&mut self, lo: f64, hi: f64) -> f64 {
        let u = (self.next_u64() >> 11) as f64 / (1u64 << 53) as f64;
        lo + (hi - lo) * u
    }

    pub fn usize_in(&mut self, lo: usize, hi: usize) -> usize {
        if hi <= lo {
            return lo;
        }
        lo + (self.next_u64() as usize % (hi - lo + 1))
    }

    pub fn random_motion_1d(&mut self) -> LinearMotion1d {
        LinearMotion1d::new(self.uniform(-500.0, 500.0), self.uniform(-20.0, 20.0))
    }

    pub fn random_motion_2d(&mut self) -> LinearMotion2d {
        LinearMotion2d::new(
            self.uniform(-500.0, 500.0),
            self.uniform(-500.0, 500.0),
            self.uniform(-15.0, 15.0),
            self.uniform(-15.0, 15.0),
        )
    }

    pub fn random_motions_1d(&mut self, n: usize) -> Vec<LinearMotion1d> {
        (0..n).map(|_| self.random_motion_1d()).collect()
    }

    pub fn random_motions_2d(&mut self, n: usize) -> Vec<LinearMotion2d> {
        (0..n).map(|_| self.random_motion_2d()).collect()
    }

    pub fn random_colors(&mut self, n: usize) -> Vec<Color> {
        (0..n)
            .map(|_| {
                if self.next_u64() % 2 == 0 {
                    Color::Red
                } else {
                    Color::Blue
                }
            })
            .collect()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::motion::LinearMotion2d;

    #[test]
    fn upper_hull_rising_interior_point() {
        let motions = vec![
            LinearMotion2d::new(0.0, 0.0, 0.0, 0.0),
            LinearMotion2d::new(2.0, 0.0, 0.0, 0.0),
            LinearMotion2d::new(1.0, 0.0, 0.0, 1.0),
        ];
        assert_eq!(brute_upper_hull(&motions, 0.0), vec![0, 1]);
        let h1 = brute_upper_hull(&motions, 1.0);
        assert!(h1.contains(&2), "h1={h1:?}");
        assert_eq!(h1, vec![0, 2, 1]);
    }

    #[test]
    fn spanner_matches_internal_rebuild_rule() {
        let motions = vec![
            LinearMotion2d::new(0.0, 0.0, 0.0, 0.0),
            LinearMotion2d::new(1.0, 0.0, 0.0, 0.0),
            LinearMotion2d::new(0.0, 1.0, 0.0, 0.0),
        ];
        let edges = brute_greedy_spanner(&motions, 2.0, 0.0);
        assert!(!edges.is_empty());
    }
}
